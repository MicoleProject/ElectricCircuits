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

#include "common.h"

Vec3f rotatePart(const Vec3f &pos)
{
	return Vec3f(pos.x,
		cos(SLOPEANGLE) * pos.y - sin(SLOPEANGLE) * pos.z,
		sin(SLOPEANGLE) * pos.y + cos(SLOPEANGLE) * pos.z);
}

Vec3f rotateTranslatePart(const Vec3f &pos)
{
	return Vec3f(pos.x,
		cos(SLOPEANGLE) * pos.y - sin(SLOPEANGLE) * pos.z,
		sin(SLOPEANGLE) * pos.y + cos(SLOPEANGLE) * pos.z - CIRCUITHEIGHT);
}

Vec3f rotatePart(const float &x, const float &y, const float &z)
{
	return rotatePart(Vec3f(x, y, z));
}

Vec3f rotateTranslatePart(const float &x, const float &y, const float &z)
{
	return rotateTranslatePart(Vec3f(x, y, z));
}

Vec3f unrotatePart(const Vec3f &pos)
{
	return Vec3f(pos.x,
		cos(SLOPEANGLE) * pos.y + sin(SLOPEANGLE) * pos.z,
		- sin(SLOPEANGLE) * pos.y + cos(SLOPEANGLE) * pos.z);
}

Vec3f unrotateTranslatePart(const Vec3f &pos)
{
	return Vec3f(pos.x,
		cos(SLOPEANGLE) * pos.y + sin(SLOPEANGLE) * pos.z + CIRCUITHEIGHT,
		- sin(SLOPEANGLE) * pos.y + cos(SLOPEANGLE) * pos.z);
}

Vec3f unrotatePart(const float &x, const float &y, const float &z)
{
	return unrotatePart(Vec3f(x, y, z));
}

Vec3f unrotateTranslatePart(const float &x, const float &y, const float &z)
{
	return unrotateTranslatePart(Vec3f(x, y, z));
}
