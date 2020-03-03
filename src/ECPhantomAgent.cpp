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

#include "ECPhantomAgent.h"

#include <Vrml.h>
using namespace Reachin;

#include <MicoleStringStream.h>
#include <ECPhantomManager.h>

#include "ECNode.h"

ECPhantomAgent *ECPhantomAgent::_ecpInstance=NULL;

ECPhantomAgent::ECPhantomAgent()
:MicoleAgent("ECPhantomAgent","Phantom"), _suspended(false), _following(false)
{
	bindMessage( "^EC LINE : seg=\\(\\((.*), (.*), (.*)\\),\\((.*), (.*), (.*)\\)\\);$", 
		BUS_CALLBACK_OF(ECPhantomAgent, handleOnALine ));
	bindMessage( "^EC RES : seg=\\(\\((.*), (.*), (.*)\\),\\((.*), (.*), (.*)\\)\\);$", 
		BUS_CALLBACK_OF(ECPhantomAgent, handleOnAResistor ));
	bindMessage( "^EC LAMP : seg=\\(\\((.*), (.*), (.*)\\),\\((.*), (.*), (.*)\\)\\);$", 
		BUS_CALLBACK_OF(ECPhantomAgent, handleOnALamp ));
	bindMessage( "^EC CAP : seg=\\(\\((.*), (.*), (.*)\\),\\((.*), (.*), (.*)\\)\\);$", 
		BUS_CALLBACK_OF(ECPhantomAgent, handleOnACapacitor ));
	bindMessage( "^EC BAT : seg=\\(\\((.*), (.*), (.*)\\),\\((.*), (.*), (.*)\\)\\);$", 
		BUS_CALLBACK_OF(ECPhantomAgent, handleOnABattery ));
	bindMessage( "^EC NODE : pos=\\((.*), (.*), (.*)\\);$", 
		BUS_CALLBACK_OF(ECPhantomAgent, handleOnANode ));
	bindMessage( "^EC PICOBG : pos=(.*); bumps=(.*);$", 
		BUS_CALLBACK_OF(ECPhantomAgent, handlePicob ));
	bindMessage("^EC NOTHING;", BUS_CALLBACK_OF(ECPhantomAgent, handleNothing));
	bindMessage("^EC SUSPEND;", BUS_CALLBACK_OF(ECPhantomAgent, handleSuspend));
	bindMessage("^EC RESUME;", BUS_CALLBACK_OF(ECPhantomAgent, handleResume));
	bindMessage("^EC FOLLOW START;", BUS_CALLBACK_OF(ECPhantomAgent, handleFollowStart));
	bindMessage("^EC FOLLOW STOP;", BUS_CALLBACK_OF(ECPhantomAgent, handleFollowStop));
}

ECPhantomAgent::~ECPhantomAgent()
{
}

void ECPhantomAgent::handleSuspend(MicoleBus *app, int argc, const char **argv)
{
	_suspended = true;
}

void ECPhantomAgent::handleResume(MicoleBus *app, int argc, const char **argv)
{
	_suspended = false;
	ECPhantomManager::getInstance()->setNearest();
/*	if (_lastObject)
	{
		_lastObject->setActif(false);
		_lastObject = NULL;
	}
	onInput();*/
}

void ECPhantomAgent::handleFollowStart(MicoleBus *app, int argc, const char **argv)
{
	_following = true;
}

void ECPhantomAgent::handleFollowStop(MicoleBus *app, int argc, const char **argv)
{
	_following = false;
}

//EC LINE : seg=((x, y, z),(x, y, z));
void ECPhantomAgent::handleOnALine( MicoleBus *app, int argc, const char **argv)
{
	if (argc < 6 || _suspended)
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

	MicoleStringStream s;
	s << "OUT FF3D : line=((" << p1.x << ", " << p1.y << ", " << p1.z << "),(" << p2.x << ", " << p2.y << ", " << p2.z << "));";
	sendMessage(s.str());
}

void ECPhantomAgent::handleOnAResistor( MicoleBus *app, int argc, const char **argv)
{
	if (argc < 6 || _suspended)
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

	MicoleStringStream s;
	if (_following)
		s << "OUT FF3D : line=((" << p1.x << ", " << p1.y << ", " << p1.z << "),(" << p2.x << ", " << p2.y << ", " << p2.z << "));";
	else
	{
		s << "OUT FF3D : picobhg=((" << \
			p1.x << ", " << p1.y << ", " << p1.z << "),(" << \
			p2.x << ", " << p2.y << ", " << p2.z << ")); ";
		//caca
		//rustine pour compenser la compensation de la reachin
		float bmpsize;
		Vec3f m = (p1 + p2)/2;
		m.y = 0;
		if (m.length() < 0.02)
			bmpsize = LITTLE_BUMPS_HEIGHT;
		else
			bmpsize = BIG_BUMPS_HEIGHT;
		if (fabs(p2.x - p1.x) < fabs(p2.z - p1.z))
			s << "bumps=((0, 0, " << bmpsize << "),(0, 0, " << bmpsize << "),(0, 0, " << bmpsize << ")); bumplength=" << \
				BUMPLENGTH << "; pauselength=" << PAUSELENGTH << ";";
		else
			s << "bumps=((" << bmpsize << ", 0, 0),(" << bmpsize << ", 0, 0),(" << bmpsize << ", 0, 0)); bumplength=" << \
				BUMPLENGTH << "; pauselength=" << PAUSELENGTH << ";";
	}
	sendMessage(s.str());
}

void ECPhantomAgent::handleOnALamp( MicoleBus *app, int argc, const char **argv)
{
	if (argc < 6 || _suspended)
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

	MicoleStringStream s;
	if (_following)
		s << "OUT FF3D : line=((" << p1.x << ", " << p1.y << ", " << p1.z << "),(" << p2.x << ", " << p2.y << ", " << p2.z << "));";
	else
	{
		s << "OUT FF3D : picobhg=((" << \
			p1.x << ", " << p1.y << ", " << p1.z << "),(" << \
			p2.x << ", " << p2.y << ", " << p2.z << "));";
		//rustine pour compenser la compensation de la reachin
		float bmpsize;
		Vec3f m = (p1 + p2)/2;
		m.y = 0;
		if (m.length() < 0.02)
			bmpsize = LITTLE_BUMPS_HEIGHT;
		else
			bmpsize = BIG_BUMPS_HEIGHT;
		if (fabs(p2.x - p1.x) < fabs(p2.z - p1.z))
			s << " bumps=((0, 0, " << bmpsize << ")); bumplength=" << BUMPLENGTH << "; pauselength=" << PAUSELENGTH << ";";
		else
			s << " bumps=((" << bmpsize << ", 0, 0)); bumplength=" << BUMPLENGTH << "; pauselength=" << PAUSELENGTH << ";";
	}
	sendMessage(s.str());
}

void ECPhantomAgent::handleOnACapacitor( MicoleBus *app, int argc, const char **argv)
{
	if (argc < 6 || _suspended)
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
	catch (...) //can happen for truncated packet or bad transmission
	{
		return;
	}

	MicoleStringStream s;
	if (_following)
		s << "OUT FF3D : line=((" << p1.x << ", " << p1.y << ", " << p1.z << "),(" << p2.x << ", " << p2.y << ", " << p2.z << "));";
	else
	{
		s << "OUT FF3D : picobhg=((" << \
			p1.x << ", " << p1.y << ", " << p1.z << "),(" << \
			p2.x << ", " << p2.y << ", " << p2.z << "));";
		//rustine pour compenser la compensation de la reachin
		float bmpsize;
		Vec3f m = (p1 + p2)/2;
		m.y = 0;
		if (m.length() < 0.02)
			bmpsize = LITTLE_BUMPS_HEIGHT;
		else
			bmpsize = BIG_BUMPS_HEIGHT;
		if (fabs(p2.x - p1.x) < fabs(p2.z - p1.z))
			s << " bumps=((0, 0, " << bmpsize << "),(0, 0, " << bmpsize << ")); bumplength=" << \
				BUMPLENGTH << "; pauselength=" << PAUSELENGTH << ";";
		else
			s << " bumps=((" << bmpsize << ", 0, 0),(" << bmpsize << ", 0, 0)); bumplength=" << \
				BUMPLENGTH << "; pauselength=" << PAUSELENGTH << ";";
	}
	sendMessage(s.str());
}

void ECPhantomAgent::handleOnABattery( MicoleBus *app, int argc, const char **argv)
{
	if (argc < 6 || _suspended)
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

	MicoleStringStream s;
	if (_following)
		s << "OUT FF3D : line=((" << p1.x << ", " << p1.y << ", " << p1.z << "),(" << p2.x << ", " << p2.y << ", " << p2.z << "));";
	else
	{
		s << "OUT FF3D : picobhg=((" << \
			p1.x << ", " << p1.y << ", " << p1.z << "),(" << \
			p2.x << ", " << p2.y << ", " << p2.z << "));";
/*		if (abs(p2.x - p1.x) < abs(p2.z - p1.z))
			s << " bumps=((0, 0, " << BIG_BUMPS_HEIGHT << "),(0, 0, " << LITTLE_BUMPS_HEIGHT << ")); bumplength=" << \
				BUMPLENGTH << "; pauselength=" << PAUSELENGTH << ";";
		else
			s << " bumps=((" << BIG_BUMPS_HEIGHT << ", 0, 0),(" << LITTLE_BUMPS_HEIGHT << ", 0, 0)); bumplength=" << \
				BUMPLENGTH << "; pauselength=" << PAUSELENGTH << ";";*/
		//rustine pour compenser la compensation de la reachin
		float bmpsize;
		Vec3f m = (p1 + p2)/2;
		m.y = 0;
		if (m.length() < 0.02)
			bmpsize = LITTLE_BUMPS_HEIGHT;
		else
			bmpsize = BIG_BUMPS_HEIGHT;
		if (fabs(p2.x - p1.x) < fabs(p2.z - p1.z))
			s << " bumps=((0, 0, " << bmpsize << "),(0, 0, " << bmpsize << ")); bumplength=" << \
				BUMPLENGTH << "; pauselength=" << PAUSELENGTH << ";";
		else
			s << " bumps=((" << bmpsize << ", 0, 0),(" << bmpsize << ", 0, 0)); bumplength=" << \
				BUMPLENGTH << "; pauselength=" << PAUSELENGTH << ";";
	}
	sendMessage(s.str());
}

//EC NODE : pos=(x,y,z);
void ECPhantomAgent::handleOnANode( MicoleBus *app, int argc, const char **argv)
{
	if (argc < 3 || _suspended)
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

	MicoleStringStream s;
	s << "OUT FF3D : disc=((" << p.x << ", " << p.y << ", " << p.z << "),(" << \
		p.x << ", " << p.y + 0.01 << ", " << p.z << ")," << SPHERE_RADIUS << ");";
	
	sendMessage(s.str());
}

//EC PICOB : pos=(x, y, z); bumps=(x1, y1, z1),(x2, y2, z2),...,(xn, yn, zn);
void ECPhantomAgent::handlePicob( MicoleBus *app, int argc, const char **argv)
{
	MicoleStringStream s;
	if (argc < 2)
		return;
	s << "OUT FF3D : picobg=" << argv[0] << "; bumps=" << argv[1] << "; bumplength=" << \
		BUMPTIME << "; pauselength=" << PAUSETIME << ";";
	sendMessage(s.str());
}

//EC NOTHING;
void ECPhantomAgent::handleNothing( MicoleBus *app, int argc, const char **argv)
{
	sendMessage("OUT FF3D : none;");
}

ECPhantomAgent * ECPhantomAgent::getInstance()
{
	if (_ecpInstance == NULL)
		_ecpInstance = new ECPhantomAgent;
	return _ecpInstance;
}
