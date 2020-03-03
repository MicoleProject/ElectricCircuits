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

#include "ECVTPlayerAgent.h"
//#include "ECNode.h"

#include <cmath>
#include <Vrml.h>
#include <MicoleStringStream.h>
#include <Picob.h>
#include <common.h>

#define EPSILON 0.00001
//#include <VTPMouseListenerExample.h>

using namespace Reachin;

ECVTPlayerAgent::ECVTPlayerAgent()
:/*MicoleAgent("ECVTPlayerAgent","VTPlayer"),*/ _hand(RIGHT), _rightPattern("0000000000000000"), _leftPattern("0000000000000000")
{
	bindMessage("^EC LINE : seg=\\(\\((.*), (.*), (.*)\\),\\((.*), (.*), (.*)\\)\\);$", 
		BUS_CALLBACK_OF(ECVTPlayerAgent, handleOnALine ));
	bindMessage("^EC RES : seg=\\(\\((.*), (.*), (.*)\\),\\((.*), (.*), (.*)\\)\\);$", 
		BUS_CALLBACK_OF(ECVTPlayerAgent, handleOnAResistor ));
	bindMessage("^EC LAMP : seg=\\(\\((.*), (.*), (.*)\\),\\((.*), (.*), (.*)\\)\\);$", 
		BUS_CALLBACK_OF(ECVTPlayerAgent, handleOnALamp ));
	bindMessage("^EC CAP : seg=\\(\\((.*), (.*), (.*)\\),\\((.*), (.*), (.*)\\)\\);$", 
		BUS_CALLBACK_OF(ECVTPlayerAgent, handleOnACapacitor ));
	bindMessage("^EC BAT : seg=\\(\\((.*), (.*), (.*)\\),\\((.*), (.*), (.*)\\)\\);$", 
		BUS_CALLBACK_OF(ECVTPlayerAgent, handleOnABattery ));
	bindMessage("^EC NODE : pos=\\((.*), (.*), (.*)\\);$", 
		BUS_CALLBACK_OF(ECVTPlayerAgent, handleOnANode ));
	bindMessage("^EC PICOBG : pos=.*; bumps=(.*);$", 
		BUS_CALLBACK_OF(ECVTPlayerAgent, handlePicob ));
	bindMessage("^EC PICOBG STOP;$", 
		BUS_CALLBACK_OF(ECVTPlayerAgent, handleEndPicob ));
	bindMessage("^EC NOTHING;$", BUS_CALLBACK_OF(ECVTPlayerAgent, handleNothing));
}

ECVTPlayerAgent::~ECVTPlayerAgent()
{
}

void ECVTPlayerAgent::onKey(int keycode)
{
	// key G
	if (keycode==103)
	{
		switch(_hand)
		{
			case RIGHT:
				_hand = LEFT;
				break;
			case LEFT:
				_hand = RIGHT;
				break;
		}
		string temp = _rightPattern;
		_rightPattern = _leftPattern;
		_leftPattern = temp;
		bool tmp = _rightStatic;
		_rightStatic = _leftStatic;
		_leftStatic = tmp;
		MicoleStringStream s;
		if (_leftStatic)
			s << "OUT VTP : left=(" << _leftPattern << ");";
		else
			s << "OUT VTP : left=(" << _leftPattern << "); dur=" << FRAMEDURATION << ";";
		sendMessage(s.str());
		s.flush();
		if (_rightStatic)
			s << "OUT VTP : right=(" << _rightPattern << ");";
		else
			s << "OUT VTP : right=(" << _rightPattern << "); dur=" << FRAMEDURATION << ";";
		sendMessage(s.str());
	}
}

//EC LINE : seg=((x,y,z),(x,y,z));
void ECVTPlayerAgent::handleOnALine( MicoleBus *app, int argc, const char **argv)
{
		switch(_hand)
		{
			case RIGHT:
				_rightPattern = "0000111111110000";
				sendMessage("OUT VTP : right=(0000111111110000);");
				_rightStatic = true;
				break;
			case LEFT:
				_leftPattern = "0000111111110000";
				sendMessage("OUT VTP : left=(0000111111110000);");
				_leftStatic = true;
				break;
		}
}

//EC NODE : pos=(x,y,z);
void ECVTPlayerAgent::handleOnANode( MicoleBus *app, int argc, const char **argv)
{
		switch(_hand)
		{
			case RIGHT:
				_rightPattern = "0000011001100000";
				sendMessage("OUT VTP : right=(0000011001100000);");
				_rightStatic = true;
				break;
			case LEFT:
				_leftPattern = "0000011001100000";
				sendMessage("OUT VTP : left=(0000011001100000);");
				_leftStatic = true;
				break;
		}
}

//EC PICOBG : pos=(x, y, z); bumps=((x1, y1, z1),(x2, y2, z2),...,(xn, yn, zn));
void ECVTPlayerAgent::handlePicob( MicoleBus *app, int argc, const char **argv)
{
	vector<Vec3f> bumpList;
	Picob::readPicobList(argv[0], bumpList);

	//sequence of static icons... build a dynamic icon...
	string dir;
	switch(_hand)
	{
		case RIGHT:
			dir = patternOfDirection(bumpList[0]);
			_leftPattern = dir + ",0000000000000000";
			_leftStatic = false;
			break;
		case LEFT:
			dir = patternOfDirection(bumpList[0]);
			_rightPattern = dir + ",0000000000000000";
			_rightStatic = false;
			break;
	}
	for (unsigned int i = 1 ; i < bumpList.size() ; i++)
	{
		switch(_hand)
		{
			case RIGHT:
				dir = patternOfDirection(bumpList[i]);
				_leftPattern += "," + dir + ",0000000000000000";
				break;
			case LEFT:
				dir = patternOfDirection(bumpList[i]);
				_rightPattern += "," + dir + ",0000000000000000";
				break;
		}
	}
	MicoleStringStream s;
	switch(_hand)
	{
		case RIGHT:
			s << "OUT VTP : left=(" << _leftPattern << "); dur=" << FRAMEDURATION  << " ;";
			break;
		case LEFT:
			s << "OUT VTP : right=(" << _rightPattern << "); dur=" << FRAMEDURATION  << " ;";
			break;
	}
	sendMessage(s.str());
}

string ECVTPlayerAgent::patternOfDirection(const Vec3f &dir)
{
	float yz = sqrt(dir.y * dir.y + dir.z * dir.z);
	if ((dir.y > EPSILON) ^ (dir.z > EPSILON))
		yz *= -1;
	float len = sqrt(dir.x * dir.x + yz * yz);
	double alpha;
	if (fabs(dir.x) > fabs(yz))
	{
		if (yz > - EPSILON)
			alpha = acos(dir.x / len);
		else
			alpha = - acos(dir.x / len);
	}
	else
	{
		if (dir.x > - EPSILON)
			alpha = asin(yz / len);
		else
			alpha = M_PI - asin(yz / len);
	}
	if (alpha < 0)
		alpha += 2 * M_PI;

	int a = int((4 * alpha)/M_PI - 1/2);
	if (fabs((4 * alpha)/M_PI - 1/2 - a) > 0.5 - EPSILON)
		a++;
	switch(a)
	{
		case 0: return "0001000100010001";
		case 1: return "1111000100010001";
		case 2: return "1111000000000000";
		case 3: return "1111100010001000";
		case 4: return "1000100010001000";
		case 5: return "1000100010001111";
		case 6: return "0000000000001111";
		case 7: return "0001000100011111";
		//should not happen
		default: return "0101010101010101";
	}
}

//EC PICOBG STOP;
void ECVTPlayerAgent::handleEndPicob( MicoleBus *app, int argc, const char **argv)
{
	switch(_hand)
	{
		case RIGHT:
			_leftPattern = "0000000000000000";
			sendMessage("OUT VTP : left=(0000000000000000);");
			_leftStatic = true;
			break;
		case LEFT:
			_rightPattern = "0000000000000000";
			sendMessage("OUT VTP : right=(0000000000000000);");
			_rightStatic = true;
			break;
	}
}

//EC RES : seg=((x,y,z),(x,y,z));
void ECVTPlayerAgent::handleOnAResistor( MicoleBus *app, int argc, const char **argv)
{
	switch(_hand)
	{
		case RIGHT:
			_rightPattern = "1111100110011111";
			sendMessage("OUT VTP : right=(1111100110011111);");
			_rightStatic = true;
			break;
		case LEFT:
			_leftPattern = "1111100110011111";
			sendMessage("OUT VTP : left=(1111100110011111);");
			_leftStatic = true;
			break;
	}
}

//EC CAP : seg=((x,y,z),(x,y,z));
void ECVTPlayerAgent::handleOnACapacitor(MicoleBus *app, int argc, const char **argv)
{
	switch(_hand)
	{
		case RIGHT:
			_rightPattern = "1001100110011001";
			sendMessage("OUT VTP : right=(1001100110011001);");
			_rightStatic = true;
			break;
		case LEFT:
			_leftPattern = "1001100110011001";
			sendMessage("OUT VTP : left=(1001100110011001);");
			_leftStatic = true;
			break;
	}
}

//EC BAT : seg=((x,y,z),(x,y,z));
void ECVTPlayerAgent::handleOnABattery(MicoleBus *app, int argc, const char **argv)
{
	switch(_hand)
	{
		case RIGHT:
			_rightPattern = "0001100110010001";
			sendMessage("OUT VTP : right=(0001100110010001);");
			_rightStatic = true;
			break;
		case LEFT:
			_leftPattern = "0001100110010001";
			sendMessage("OUT VTP : left=(0001100110010001);");
			_leftStatic = true;
			break;
	}
}

//EC LAMP : seg=((x,y,z),(x,y,z));
void ECVTPlayerAgent::handleOnALamp(MicoleBus *app, int argc, const char **argv)
{
	switch(_hand)
	{
		case RIGHT:
			_rightPattern = "1001011001101001";
			sendMessage("OUT VTP : right=(1001011001101001);");
			_rightStatic = true;
			break;
		case LEFT:
			_leftPattern = "1001011001101001";
			sendMessage("OUT VTP : left=(1001011001101001);");
			_leftStatic = true;
			break;
	}
}

//EC NOTHING;
void ECVTPlayerAgent::handleNothing(MicoleBus *app, int argc, const char **argv)
{
	switch(_hand)
	{
		case RIGHT:
			_rightPattern = "0000000000000000";
			sendMessage("OUT VTP : right=(0000000000000000);");
			_rightStatic = true;
			break;
		case LEFT:
			_leftPattern = "0000000000000000";
			sendMessage("OUT VTP : left=(0000000000000000);");
			_leftStatic = true;
			break;
	}
}

#undef EPSILON
