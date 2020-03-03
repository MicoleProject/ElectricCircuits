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

#ifndef __ECMANAGER__
#define __ECMANAGER__

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x05000
#endif

#include "common.h"
#include <map>
#include <list>
using namespace std;

#include "ECObject.h"
#include "ECNode.h"
#include "ECLine.h"

class ECManager
{
	public:
		ECManager();
		ECManager(DEFMap *sceneMap);
		~ECManager();

		//static void addLine(ECLine *l);
		static ECNode *findNode(const char *s);
		static void addNode(const string &s, ECNode *n);
		static void addLine(ECLine *l);
		
		static void updateLines();
		
		ECObject *projectPosition(const Vec3f &pos);
		virtual ECObject *getLastObject() = 0;
		
	private:
		static map<string, ECNode *> _nodesList;
		static list<ECLine *> _linesList;
		static DEFMap *_sceneMap;
};

#endif
