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

#ifndef __ECOBJECT__
#define __ECOBJECT__

#include <Vrml.h>
#include <Transform.h>
#include <Shape.h>

using namespace Reachin;

class ECObject: public Transform
{
	public:
		ECObject();
		virtual ~ECObject();
		virtual float distance(const Vec3f & phantomPos) const = 0;
		virtual Vec3f projection(const Vec3f & phantomPos) const = 0;
		virtual ECObject *getNearest(const Vec3f & phantomPos) = 0;
		virtual string getEffectMessage() const = 0;
		virtual string getPICOBgMessage(const Vec3f &phantomPos) = 0;
		
		virtual void setActif(bool a) = 0;
		virtual void subSetActif(bool a) = 0;
		
		void setAlreadyGoneInto();
		virtual bool hasAlreadyGoneInto() const;
		
		//static const Interface interface;
		
	protected:
		virtual void initialize();
		bool _alreadyGoneInto;
		Shape * sh;
};

#endif
