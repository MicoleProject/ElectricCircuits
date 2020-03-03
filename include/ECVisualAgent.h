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

#ifndef __ECVISUALAGENT__
#define __ECVISUALAGENT__

#include <Transform.h>
#include <Vrml.h>
using namespace Reachin;
#include <FF3DDeviceInputAgent.h>

class ECVisualAgent: public Transform, public FF3DDeviceInputAgent
{
	public:
		~ECVisualAgent();
		//EC LINE|RES|LAMP|CAP|BAT| : seg=((x,y,z),(x,y,z));
		void handleOnALine(MicoleBus *app, int argc, const char **argv);
		//EC NODE : pos=(x,y,z);
		void handleOnANode(MicoleBus *app, int argc, const char **argv);
		//EC NOTHING;
		void handleNothing(MicoleBus *app, int argc, const char **argv);
		
		void handleMouse( MicoleBus *app, int argc, const char **argv);
		
		void onInput();
		
		static ECVisualAgent * getInstance();
		
	private:
		ECVisualAgent();
		static ECVisualAgent *_ecvInstance;
		Vec3f p1, p2, lastpos;
		Transform *_mse, *_phtm;
};

#endif
