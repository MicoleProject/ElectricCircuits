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

#ifndef __ECPHTMAGENT__
#define __ECPHTMAGENT__

#include "common.h"
#include <MicoleAgent.h>

class ECPhantomAgent:MicoleAgent
{
	public:
		~ECPhantomAgent();
		void handleSuspend(MicoleBus *app, int argc, const char **argv);
		void handleResume(MicoleBus *app, int argc, const char **argv);
		
		//EC LINE : seg=((x, y, z),(x, y, z));
		void handleOnALine( MicoleBus *app, int argc, const char **argv);
		//EC RES : seg=((x, y, z),(x, y, z));
		void handleOnAResistor( MicoleBus *app, int argc, const char **argv);
		//EC LAMP : seg=((x, y, z),(x, y, z));
		void handleOnALamp( MicoleBus *app, int argc, const char **argv);
		//EC CAP : seg=((x, y, z),(x, y, z));
		void handleOnACapacitor( MicoleBus *app, int argc, const char **argv);
		//EC BAT : seg=((x, y, z),(x, y, z));
		void handleOnABattery( MicoleBus *app, int argc, const char **argv);
		//EC NODE : pos=(x, y, z);
		void handleOnANode( MicoleBus *app, int argc, const char **argv);
		//EC PICOBG : pos=(x, y, z); bumps=((x1, y1, z1),(x2, y2, z2),...,(xn, yn, zn));
		void handlePicob( MicoleBus *app, int argc, const char **argv);
		//EC NOTHING;
		void handleNothing(MicoleBus *app, int argc, const char **argv);
		
		void handleFollowStart(MicoleBus *app, int argc, const char **argv);
		void handleFollowStop(MicoleBus *app, int argc, const char **argv);
		
		static ECPhantomAgent * getInstance();
		
	private:
		ECPhantomAgent();
		static ECPhantomAgent *_ecpInstance;
		bool _suspended, _following;
};

#endif
