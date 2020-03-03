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

#ifndef __ECMOUSEMANAGER__
#define __ECMOUSEMANAGER__

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x05000
#endif

#include <Transform.h>

#include <MouseInputAgent.h>

#include "ECManager.h"

class ECMouseManager: public ECManager, public MouseInputAgent, public Transform
{
	public:
		ECMouseManager();
		~ECMouseManager();
		//attract on the nearest
		void onMove(const float &x, const float &y);
		//void onEvt(string);

		static ECMouseManager * getInstance()
		{
			if (_ecmmInstance == NULL)
				_ecmmInstance = new ECMouseManager;
			return _ecmmInstance;
		}
		
		ECObject *getLastObject();
		
	private:
		static ECMouseManager *_ecmmInstance;
		ECObject *_lastObject;
		Vec3f _lastWayPoint;
};

#endif
