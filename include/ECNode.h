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

#ifndef __ECNODE__
#define __ECNODE__

#include <Vrml.h>
#include <Transform.h>

#include <list>
using namespace std;

#include "ECObject.h"

#define SPHERE_RADIUS 0.004

using namespace Reachin;

class ECLine;

class ECNode: public ECObject
{
	public:
		ECNode();
		~ECNode();
		
		//add a line to the node's neighbours
		void addNeighbour(ECLine *l);
		
		//ECObject virtual functions
		float distance(const Vec3f & phantomPos) const;
		Vec3f projection(const Vec3f & phantomPos) const;
		ECObject *getNearest(const Vec3f & phantomPos);
		virtual string getEffectMessage() const;
		virtual string getPICOBgMessage(const Vec3f & phantomPos);
		
		void setActif(bool a);
		void subSetActif(bool a);
		
		//accessor
		Vec3f getCoords() const;
		
		static const Interface interface;
		
	protected:
		virtual void initialize();
		Vec3f _center;
		list<ECLine *> _listLines;
};

#endif
