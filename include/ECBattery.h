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

#ifndef __CAPACITOR__
#define __CAPACITOR__

#include <Vrml.h>

#include <vector>
using namespace Reachin;
#include "ECLine.h"

class ECBattery : public ECLine 
{
	public:
		ECBattery();
		~ECBattery();
		
		string getEffectMessage() const;
		
		static const Interface interface;
		virtual void subSetActif(bool);

	Shape *sh_cyl1, *sh_cyl2;

	protected:
		virtual void initialize();

	private:
		ECBattery(const ECBattery &b);
};

#endif
