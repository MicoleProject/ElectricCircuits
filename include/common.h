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

#ifndef __COMMON__
#define __COMMON__

#pragma warning (disable : 4251)

//circuit slope
//radians
//#define SLOPEANGLE 0.0
#define SLOPEANGLE (M_PI/2)
//#define SLOPEANGLE (M_PI/4)
//translation
//only with a slope of M_PI/2
//#define CIRCUITHEIGHT 0.0
#define CIRCUITHEIGHT -0.08

//PICOBhg
#define LITTLE_BUMPS_HEIGHT 0.002
#define BIG_BUMPS_HEIGHT 0.008
#define BUMPLENGTH 0.003
#define PAUSELENGTH 0.003

//PICOBg
#define LITTLE_GBUMPS_HEIGHT 0.008
#define BIG_GBUMPS_HEIGHT 0.012
#define BUMPTIME 0.6
#define PAUSETIME 0.2

#define FRAMEDURATION 400

//Lag for the visual sphere's inertia
#define VISUALLAG 0.005

#define MINRECORDINGDISTANCE 0.001
#define REPLAYDELTA 0.02
//try to use SPHERE_RADIUS from ECNode.h
#define REPLAYLITTLEDELTA 0.002
#define MINFOLLOWDINGDISTANCE 0.02
#define FOLLOWINGDELTA  0.02
//0.005
#define FOLLOWINGLITTLEDELTA 0.002
//0.0005

#include <Vrml.h>
using namespace Reachin;

// XZ => XY
Vec3f rotatePart(const Vec3f &pos);
Vec3f rotateTranslatePart(const Vec3f &pos);
Vec3f rotatePart(const float &x, const float &y, const float &z);
Vec3f rotateTranslatePart(const float &x, const float &y, const float &z);
// XY => XZ
Vec3f unrotatePart(const Vec3f &pos);
Vec3f unrotateTranslatePart(const Vec3f &pos);
Vec3f unrotatePart(const float &x, const float &y, const float &z);
Vec3f unrotateTranslatePart(const float &x, const float &y, const float &z);

#endif
