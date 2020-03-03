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

#ifndef __RESISTOR__
#define __RESISTOR__

#include <Vrml.h>

#include <vector>
#include "ECLine.h"

using namespace Reachin;

class ECResistor : public ECLine 
{
	public:
		ECResistor();
		
		string getEffectMessage() const;
		
		static const Interface interface;
		virtual void subSetActif(bool);

		Shape * sh;

	protected:
		virtual void initialize();
};

#endif
