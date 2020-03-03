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

#ifndef __ECLINE__
#define __ECLINE__

#include <Vrml.h>
#include <Transform.h>
using namespace Reachin;

#include "ECNode.h"

#define CYLINDER_RADIUS 0.0025

class ECLine: public ECObject
{
	public:
		ECLine( Defined< SFString > begin = 0, Defined< SFString > end = 0 );
		virtual ~ECLine();
		
		//needed to place the visual cylinder
		virtual void updateEnds();
		
		//ECObject virtual functions
		float distance(const Vec3f & phantomPos) const;
		Vec3f projection(const Vec3f & phantomPos) const;
		ECObject *getNearest(const Vec3f & phantomPos);
		virtual string getEffectMessage() const;
		virtual string getPICOBgMessage(const Vec3f & phantomPos);
		
		void setActif(bool a);
		void subSetActif(bool a);
		
		//accessors
		ECNode * beginNode() const;
		ECNode * endNode() const;
		
		static const Interface interface;
		auto_ptr< SFString > _begin;
		auto_ptr< SFString > _end;
		
	protected:
		virtual void initialize();
		ECNode * _begin_n, * _end_n;
		
	private:
		//Visual stuff
		//the scaler is used to scale the line without scaling the
		//component itself
		Transform * line_scaler;
};

#endif
