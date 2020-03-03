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

#ifndef __ECVisualGuide__
#define __ECVisualGuide__

#include <Transform.h>
#include <Vrml.h>
using namespace Reachin;
#include <FF3DDeviceInputAgent.h>

class ECVisualGuide: public Transform, public MicoleAgent
{
	public:
		~ECVisualGuide();
		void handleInput( MicoleBus *app, int argc, const char **argv);
		
		static ECVisualGuide * getInstance();
		
	private:
		ECVisualGuide();
		static ECVisualGuide *_ecvInstance;
		Vec3f p1, p2, lastpos;
};

#endif
