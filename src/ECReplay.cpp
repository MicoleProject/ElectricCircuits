/*
 * This file is part of Electric Circuits
 *
 * Copyright (C) 2007 Micole partners
 *
 * Electric Circuits is free software: you can redistribute it 
 * and/or modify it under the terms of the GNU Lesser General 
 * Public License as published by the Free Software Foundation, 
 * either version 3 of the License, or (at your option) any 
 * later version.
 *
 * Electric Circuits is distributed in the hope that it will be 
 * useful, * but WITHOUT ANY WARRANTY; without even the implied 
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU Lesser General Public License for more 
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Electric Circuits.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "ECReplay.h"

/*#define WIN32_LEAN_AND_MEAN
#include <windows.h>
*/
#include "common.h"
#include "MicoleStringStream.h"
#include <ReachinMicoleApplication.h>

#define EPSILON 0.00001

ECReplay *ECReplay::_ecrInstance = NULL;

ECReplay::ECReplay()
:_replay(false), _beginningReplay(false), _isguided(false), _magneticPoint(123, 123, 123), _lastNodePos(135, 135, 135), _lastMsePos(124, 124, 124, 124), _recordFile(NULL), _special(false)
{
	::InitializeCriticalSection(&_replayMutex);
	FF3DDeviceInputAgent::bindMessage( "^EC LINE : seg=\\(\\((.*), (.*), (.*)\\),\\((.*), (.*), (.*)\\)\\);$", 
		BUS_CALLBACK_OF(ECReplay, handleOnALine ));
	FF3DDeviceInputAgent::bindMessage( "^EC RES : seg=\\(\\((.*), (.*), (.*)\\),\\((.*), (.*), (.*)\\)\\);$", 
		BUS_CALLBACK_OF(ECReplay, handleOnALine ));
	FF3DDeviceInputAgent::bindMessage( "^EC LAMP : seg=\\(\\((.*), (.*), (.*)\\),\\((.*), (.*), (.*)\\)\\);$", 
		BUS_CALLBACK_OF(ECReplay, handleOnALine ));
	FF3DDeviceInputAgent::bindMessage( "^EC CAP : seg=\\(\\((.*), (.*), (.*)\\),\\((.*), (.*), (.*)\\)\\);$", 
		BUS_CALLBACK_OF(ECReplay, handleOnALine ));
	FF3DDeviceInputAgent::bindMessage( "^EC BAT : seg=\\(\\((.*), (.*), (.*)\\),\\((.*), (.*), (.*)\\)\\);$", 
		BUS_CALLBACK_OF(ECReplay, handleOnALine ));
	FF3DDeviceInputAgent::bindMessage( "^EC NODE : pos=\\((.*), (.*), (.*)\\);$", 
		BUS_CALLBACK_OF(ECReplay, handleOnANode ));
//	FF3DDeviceInputAgent::bindMessage( "^EC PICOBG : pos=(.*); bumps=(.*);$", 
//		BUS_CALLBACK_OF(ECReplay, handlePicob ));
	FF3DDeviceInputAgent::bindMessage("^EC MSE CURSOR : pos=\\((.*), (.*), (.*)\\);$", 
		BUS_CALLBACK_OF(ECReplay, handleMseCursor));
	FF3DDeviceInputAgent::bindMessage("^EC MSE CURSOR : pos=\\((.*), (.*), (.*)\\); special=yes;$", 
		BUS_CALLBACK_OF(ECReplay, handleMseSpecialCursor));
}

ECReplay::~ECReplay()
{
	::DeleteCriticalSection(&_replayMutex);
}

/*
	On keystroke, start or stop the replay
*/
void ECReplay::onKey(int keycode)
{
	// key P
	if (keycode==112)
		startReplay();
	// key S
	else if (keycode==115)
		stopReplay();
	// key F
	else if (keycode==102)
	{
		_beginningReplay = true;
		_isguided = true;
		FF3DDeviceInputAgent::sendMessage("EC NOTHING;");
		FF3DDeviceInputAgent::sendMessage("EC SUSPEND;");
		FF3DDeviceInputAgent::sendMessage("EC FOLLOW START;");
		_magneticPoint.x = _lastMsePos.x;
		_magneticPoint.y = _lastMsePos.y;
		_magneticPoint.z = _lastMsePos.z;
		_special = (_lastMsePos.w == 0.0);
		_guidePoints.clear();
		_wayPoints.clear();
		MicoleStringStream s;
		s << "OUT FF3D GUIDE : point=(" << _magneticPoint.x << ", " << _magneticPoint.y << ", " << _magneticPoint.z << ");";
		FF3DDeviceInputAgent::sendMessage(s.str());
	}
}

/*
	Check if we guide to the first point, if we have to go to the next waypoint, if the next waypoint is a node, ...
*/
void ECReplay::onInput()
{
	::EnterCriticalSection(&_replayMutex);
	if (_beginningReplay)
	{
		MicoleStringStream s;
		s << "OUT FF3D GUIDE : point=(" << _magneticPoint.x << ", " << _magneticPoint.y << ", " << _magneticPoint.z << ");";
		FF3DDeviceInputAgent::sendMessage(s.str());
		
		Vec3f dis(_magneticPoint - _position);
		dis.y = 0;
		if (dis.length() < REPLAYLITTLEDELTA)
		{
			_beginningReplay = false;
			//wake up the ECMAnager
			FF3DDeviceInputAgent::sendMessage("EC RESUME;");
/*			if (_isguided)
				_guidePoints.push_back(_lastMsePos);*/
		}
	}
	else if (_replay)
	{
		Vec3f dis(_magneticPoint - _position);
		dis.y = 0;
		float l = dis.length();
		if (! _wayPoints.empty())
		{
			Vec3f pt = _wayPoints.front();
			MicoleStringStream s;
			s << "OUT FF3D GUIDE : point=(" << pt.x << ", " << pt.y << ", " << pt.z << ");";
			FF3DDeviceInputAgent::sendMessage(s.str());
		}
		else if (
			(((!_special) && (l <= REPLAYDELTA)) ||		//it's a normal point : we go forward if we are nearer than REPLAYDELTA
			(_special && (l <= REPLAYLITTLEDELTA))) &&	//it's a node : we go forward if we are nearer than REPLAYLITTLEDELTA
			readNextPoint())
		{
			MicoleStringStream s;
			s << "OUT FF3D GUIDE : point=(" << _magneticPoint.x << ", " << _magneticPoint.y << ", " << _magneticPoint.z << ");";
			FF3DDeviceInputAgent::sendMessage(s.str());
		}
	}
	else if (_isguided)
	{
		Vec3f dis(_magneticPoint - _position);
		dis.y = 0;
		float l = dis.length();
		if (! _wayPoints.empty())
		{
			Vec3f pt = _wayPoints.front();
			MicoleStringStream s;
			ReachinMicoleApplication * ama = ReachinMicoleApplication::getInstance();
			ama->sendIvyMessage("Set stiffness = 4;");
			ama->sendIvyMessage("Set d = 0.002;");
			s << "OUT FF3D GUIDE : point=(" << pt.x << ", " << pt.y << ", " << pt.z << ");";
			FF3DDeviceInputAgent::sendMessage(s.str());
		}
		else if (
			(((!_special) && (l <= FOLLOWINGDELTA)) ||		//it's a normal point : we go forward if we are nearer than REPLAYDELTA
			(_special && (l <= FOLLOWINGLITTLEDELTA))) &&	//it's a node : we go forward if we are nearer than REPLAYLITTLEDELTA
			!_guidePoints.empty())
		{
			Vec4f pt = _guidePoints.front();
			_guidePoints.pop_front();
			_magneticPoint.x = pt.x;
			_magneticPoint.y = pt.y;
			_magneticPoint.z = pt.z;
			ReachinMicoleApplication * ama = ReachinMicoleApplication::getInstance();
			_special = (pt.w == 1.0);
			if (_special)
			{
				ama->sendIvyMessage("Set stiffness = 4;");
				ama->sendIvyMessage("Set d = 0.002;");
			}
			else
			{
				ama->sendIvyMessage("Set stiffness = 2.5;");
				ama->sendIvyMessage("Set d = 0.006;");
			}
			MicoleStringStream s;
			s << "OUT FF3D GUIDE : point=(" << _magneticPoint.x << ", " << _magneticPoint.y << ", " << _magneticPoint.z << ");";
			FF3DDeviceInputAgent::sendMessage(s.str());
		}
	}
	::LeaveCriticalSection(&_replayMutex);
}

/*
	If we are leaving the correct path, put down a little stone to be able to retreive the path
*/
void ECReplay::handleOnALine( MicoleBus *app, int argc, const char **argv)
{
	if (!isReplaying())
		return;

	if (argc < 6)
		return;
	Vec3f p1, p2;
	try
	{
		p1.x = atof(argv[0]);
		p1.y = atof(argv[1]);
		p1.z = atof(argv[2]);
		p2.x = atof(argv[3]);
		p2.y = atof(argv[4]);
		p2.z = atof(argv[5]);
	}
	catch (...) //can append for truncated packet or bad transmission
	{
		return;
	}

	Vec3f l1 = p2 - p1;
	Vec3f l2 = _magneticPoint - p1;
	Vec3f p12 = l1.cross(l2);
	//l == 0 if the magnetic point is on the line
	float l = p12.length()/(l1.length() * l2.length());
	
	//if we are on the good line, there's nothing to do
	if (_wayPoints.empty() && l < EPSILON)
	{
		return;
	}
	
	//if we are on another line, but we come back to the proxy
	//we change the last checkpoint
	if(! _wayPoints.empty())
	{
		Vec3f top = _wayPoints.front();
		if (((top - p1).length() < EPSILON && (_lastNodePos - p2).length() < EPSILON) || 
			((top - p2).length() < EPSILON && (_lastNodePos - p1).length() < EPSILON))
		{
			_lastNodePos = _wayPoints.front();
			return;
		}
	}
	if (_lastNodePos.x < 50)
		_wayPoints.push_front(_lastNodePos);
	return;
}

/*
	Store the point. if we are "lost" in the circuit, pickup the little stone
*/
void ECReplay::handleOnANode( MicoleBus *app, int argc, const char **argv)
{
	if (!isReplaying() || argc < 3)
		return;
	Vec3f p;
	try
	{
		p.x = atof(argv[0]);
		p.y = atof(argv[1]);
		p.z = atof(argv[2]);
	}
	catch (...) //can append for truncated packet or bad transmission
	{
		return;
	}
	
	if (! _wayPoints.empty() && (_wayPoints.front() - p).length() < EPSILON)
	{
		Vec3f fr = _wayPoints.front();
		_wayPoints.pop_front();
		if (_wayPoints.empty())
		{
			MicoleStringStream s;
			s << "OUT FF3D GUIDE : point=(" << _magneticPoint.x << ", " << _magneticPoint.y << ", " << _magneticPoint.z << ");";
			FF3DDeviceInputAgent::sendMessage(s.str());
			_lastNodePos = Vec3f(100, 100, 100);
		}
	}
	else
		_lastNodePos = p;
}

void ECReplay::handleMseCursor(MicoleBus *app, int argc, const char **argv)
{
	Vec4f p;
	try
	{
		Vec3f cursorpos(unrotateTranslatePart(atof(argv[0]), atof(argv[1]), atof(argv[2])));
		_lastMsePos.x = cursorpos.x;
		_lastMsePos.y = cursorpos.y;
		_lastMsePos.z = cursorpos.z;
		_lastMsePos.w = 0.0;
	}
	catch (...) //can append for truncated packet or bad transmission
	{
		return;
	}
	if (! _isguided || _beginningReplay)
		return;
	Vec3f mp(_lastMsePos.x, _lastMsePos.y, _lastMsePos.z);
	if (_guidePoints.empty())
		_guidePoints.push_back(_lastMsePos);
	else
	{
		Vec3f gp(_guidePoints.back().x, _guidePoints.back().y, _guidePoints.back().z);
		if ((gp - mp).length() >= MINFOLLOWDINGDISTANCE)
			_guidePoints.push_back(_lastMsePos);
	}
}

void ECReplay::handleMseSpecialCursor(MicoleBus *app, int argc, const char **argv)
{
	try
	{
		Vec3f cursorpos(unrotateTranslatePart(atof(argv[0]), atof(argv[1]), atof(argv[2])));
		_lastMsePos.x = cursorpos.x;
		_lastMsePos.y = cursorpos.y;
		_lastMsePos.z = cursorpos.z;
		//1.0 = special point
		_lastMsePos.w = 1.0;
	}
	catch (...) //can append for truncated packet or bad transmission
	{
		return;
	}
	if (! _isguided || _beginningReplay)
		return;
	_guidePoints.push_back(_lastMsePos);
}

/*
	starts the replay, open the file, read the first point, guide to it, ...
*/
void ECReplay::startReplay()
{
	::EnterCriticalSection(&_replayMutex);
	if (!_replay)
	{
		if (!fopen_s(&_recordFile,"record", "r"))
		{
			_beginningReplay = true;
			_replay = true;
			_guidePoints.clear();
			FF3DDeviceInputAgent::sendMessage("EC NOTHING;");
			FF3DDeviceInputAgent::sendMessage("EC SUSPEND;");
			if (readNextPoint())
			{
				MicoleStringStream s;
				s << "OUT FF3D GUIDE : point=(" << _magneticPoint.x << ", " << _magneticPoint.y << ", " << _magneticPoint.z << ");";
				FF3DDeviceInputAgent::sendMessage(s.str());
			}
		}
	}
	::LeaveCriticalSection(&_replayMutex);
}

/*
	Stops the replat, closes the file, etc.
*/
void ECReplay::stopReplay()
{
	::EnterCriticalSection(&_replayMutex);
	if (_replay)
	{
		fclose(_recordFile);
		_beginningReplay = false;
		_replay = false;
		_special = false;
		_wayPoints.clear();
		_lastNodePos = Vec3f(100, 100, 100);
		_magneticPoint = Vec3f(100, 100, 100);
		FF3DDeviceInputAgent::sendMessage("EC RESUME;");
		FF3DDeviceInputAgent::sendMessage("OUT FF3D GUIDE : none;");
	}
	else if (_isguided)
	{
		_isguided = false;
		_beginningReplay = false;
		_magneticPoint = Vec3f(100, 100, 100);
		_guidePoints.clear();
		_wayPoints.clear();
		FF3DDeviceInputAgent::sendMessage("EC RESUME;");
		FF3DDeviceInputAgent::sendMessage("EC FOLLOW STOP;");
		FF3DDeviceInputAgent::sendMessage("OUT FF3D GUIDE : none;");
	}
	::LeaveCriticalSection(&_replayMutex);
}

/*
	Goes forward in the replay
*/
bool ECReplay::readNextPoint()
{
	char buffer[1024], nimp[1024];
	float a, b, c;
	int nbscanned;
	if (fgets(buffer, 1023, _recordFile) == NULL || (nbscanned = sscanf_s(buffer, "%f %f %f %s", &a, &b, &c, nimp)) < 3)
	{
		::LeaveCriticalSection(&_replayMutex);
		stopReplay();
		::EnterCriticalSection(&_replayMutex);
		return false;
	}
	_special = (nbscanned == 4);
	_magneticPoint = Vec3f(a, b, c);
	return true;
}

bool ECReplay::isReplaying() const
{
	return (_replay || _isguided) && !_beginningReplay;
}

/*
	Indicates where is the attraction point
*/
void ECReplay::guideTo()
{
	Vec3f dir = _magneticPoint - _position;
	dir.setLength(BIG_GBUMPS_HEIGHT);
	
	MicoleStringStream s;
	s << "EC PICOBG : pos=(" << _position.x << ", " << _position.y << ", " << _position.z << "); bumps=((" << \
		dir.x << ", " << dir.y << ", " << dir.z << "));";
	FF3DDeviceInputAgent::sendMessage(s.str());
}

#undef EPSILON
