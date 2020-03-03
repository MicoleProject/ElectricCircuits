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

#ifndef __ECVTPAGENT__
#define __ECVTPAGENT__

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

/*#define WIN32_LEAN_AND_MEAN
#include <windows.h>*/
#include <string>
using namespace std;

#pragma warning (disable : 4251)

#include <Vrml.h>
using namespace Reachin;

#include <MicoleAgent.h>
#include <VTPMouseController.h>
#include <VTPStaticIcon.h>
#include "KeyboardInputAgent.h"

enum Hand { RIGHT, LEFT };

class ECVTPlayerAgent: public KeyboardInputAgent
{
	public:
		ECVTPlayerAgent();
		~ECVTPlayerAgent();
		//EC LINE : seg=((x,y,z),(x,y,z));
		void handleOnALine( MicoleBus *app, int argc, const char **argv);
		//EC RES : seg=((x,y,z),(x,y,z));
		void handleOnAResistor( MicoleBus *app, int argc, const char **argv);
		//EC LAMP : seg=((x,y,z),(x,y,z));
		void handleOnALamp( MicoleBus *app, int argc, const char **argv);
		//EC CAP : seg=((x,y,z),(x,y,z));
		void handleOnACapacitor( MicoleBus *app, int argc, const char **argv);
		//EC BAT : seg=((x,y,z),(x,y,z));
		void handleOnABattery( MicoleBus *app, int argc, const char **argv);
		//EC NODE : pos=(x,y,z);
		void handleOnANode( MicoleBus *app, int argc, const char **argv);
		//EC PICOBG : pos=(x, y, z); bumps=((x1, y1, z1),(x2, y2, z2),...,(xn, yn, zn));
		void handlePicob( MicoleBus *app, int argc, const char **argv);
		//EC PICOBG STOP;
		void handleEndPicob( MicoleBus *app, int argc, const char **argv);
		//EC NOTHING;
		void handleNothing(MicoleBus *app, int argc, const char **argv);
		
		void onKey(int key);
		
	private:
		VTPMouseController * _mouseCtrl;
		VTPStaticIcon *_iconNode, *_iconLine, *_iconResistor, *_iconCapacitor, *_iconLamp, *_iconBattery;
		Hand _hand;
		string _rightPattern, _leftPattern;
		bool _rightStatic, _leftStatic;

		string patternOfDirection(const Vec3f &dir);
};

#endif
