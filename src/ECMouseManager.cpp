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

#include "ECMouseManager.h"

#include <Vrml.h>
#include <Shape.h>
#include <Appearance.h>
#include <Material.h>
#include <Sphere.h>
using namespace Reachin;

#include <MicoleStringStream.h>
#include "common.h"

ECMouseManager *ECMouseManager::_ecmmInstance = NULL;

ECMouseManager::ECMouseManager()
:_lastObject(NULL), _lastWayPoint(100, 100, 100)
{
	Shape *sh_sph = new Shape;
	Appearance * app_sph = new Appearance;
	Material * mat_sph = new Material;
	Sphere * sph = new Sphere;

	sph->radius->set(0.002);
	mat_sph->diffuse_color->set(RGB(0.8, 0.0, 0.0));
	mat_sph->specular_color->set(RGB(0.9, 0.2, 0.2));
	mat_sph->ambient_intensity->set(0.5);
	mat_sph->shininess->set(1.0);
	mat_sph->transparency->set(0.1);
	app_sph->material->set(mat_sph);

	sh_sph->appearance->set(app_sph);
	sh_sph->geometry->set(sph);
	children->add(sh_sph);
}

ECMouseManager::~ECMouseManager()
{
}

void ECMouseManager::onMove(const float &mposx, const float &mposy)
{
	Vec3f rotatedposition = Vec3f(mposx, mposy, 0);
	translation->set(Vec3f(mposx, mposy, 0));
	bool newobject = false;

	ECObject *o = NULL;
	if (_lastObject)
		o = _lastObject->getNearest(rotatedposition);
	else
		o = ECManager::projectPosition(rotatedposition);

	if (o)
	{
		if (_lastObject != o)
			newobject = true;
		_lastObject = o;
	}

	if (_lastObject)
	{
		//if we want to remove some guiding points, we have to do
		//it here
		Vec3f cursor;
		ECNode *nd;
		// special points needs to be get closer to be passed
		bool specialpoint = false;
		if ((nd = dynamic_cast<ECNode *>(_lastObject)) != NULL)
		{
			cursor = nd->getCoords();
			specialpoint = true;
		}
		else
			cursor = _lastObject->projection(rotatedposition);
		if (newobject || (_lastWayPoint - cursor).length() >= MINRECORDINGDISTANCE)
		{
			MicoleStringStream s;
			s << "EC MSE CURSOR : pos=(" << cursor.x << ", " << cursor.y << ", " << cursor.z << ");";
			if (specialpoint)
				s << " special=yes;";
			sendMessage(s.str());
			_lastWayPoint = cursor;
		}
	}
}

ECObject *ECMouseManager::getLastObject()
{
	return _lastObject;
}
