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

#ifndef __ECPHANTOMMANAGER__
#define __ECPHANTOMMANAGER__

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x05000
#endif

#include "common.h"

#include <FF3DDeviceInputAgent.h>

#include "ECManager.h"
#include "ECNode.h"
#include "ECLine.h"

class ECPhantomManager: public ECManager, public FF3DDeviceInputAgent
{
	public:
		ECPhantomManager();
		ECPhantomManager(DEFMap *sceneMap);
		~ECPhantomManager();
		//attract ont the nearest adjacent to the previous one
		void onInput();
		//attract on the nearest
		void setNearest();
		//send the effect message, moves the corsor, etc.
		void activateLastObject(const Vec3f &position, const bool &newobject);
		//start PICOBg
		void onButtonDown();
		//stop PICOBg
		void onButtonUp();

		ECObject *getLastObject();
		
		static ECPhantomManager * getInstance()
		{
			if (_ecpmInstance == NULL)
				_ecpmInstance = new ECPhantomManager;
			return _ecpmInstance;
		}
		static ECPhantomManager * createInstance(DEFMap *sceneMap)
		{
			if (_ecpmInstance == NULL)
				_ecpmInstance = new ECPhantomManager(sceneMap);
			return _ecpmInstance;
		}

		
	private:
		ECObject *_lastObject;
		bool _readingPICOBg;
		Vec3f _lastWayPoint;

		static ECPhantomManager *_ecpmInstance;
};

#endif
