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

#include <Vrml.h>
#include <Shape.h>
#include <Appearance.h>
#include <Material.h>
#include <Cylinder.h>
#include <Sphere.h>

#include <MicoleStringStream.h>

#include "common.h"
#include "ECBattery.h"
#include "ECManager.h"

#define BATTERY_RADIUS 0.006
#define BATTERY_HEIGHT 0.002

const Interface ECBattery::interface(
	"Battery", typeid( ECBattery ),
	Create<ECBattery>::create,
	&ECLine::interface
);

ECBattery::ECBattery()
{
	sh_cyl1 = new Shape;
	sh_cyl2 = new Shape;
	Appearance * app1 = new Appearance;
	Appearance * app2 = new Appearance;
	Material * mat1 = new Material;
	Material * mat2 = new Material;
	Cylinder * cyl = new Cylinder;
	Cylinder * cyl2 = new Cylinder;
	Transform * pos = new Transform;
	Transform * pos2 = new Transform;

	cyl->radius->set(BATTERY_RADIUS);
	cyl->height->set(BATTERY_HEIGHT);
	cyl2->radius->set(BATTERY_RADIUS * 2);
	cyl2->height->set(BATTERY_HEIGHT);
	mat1->diffuse_color->set(RGB(0.1, 0.1, 0.2));
	mat1->specular_color->set(RGB(0.2, 0.2, 0.5));
	mat1->ambient_intensity->set(0);
	mat1->shininess->set(0.6);
	mat2->diffuse_color->set(RGB(0.1, 0.1, 0.2));
	mat2->specular_color->set(RGB(0.2, 0.2, 0.5));
	mat2->ambient_intensity->set(0);
	mat2->shininess->set(0.6);

	app1->material->set(mat1);
	app2->material->set(mat2);

	sh_cyl1->appearance->set(app1);
	sh_cyl1->geometry->set(cyl);
	sh_cyl2->appearance->set(app2);
	sh_cyl2->geometry->set(cyl2);

	pos->translation->set(Vec3f(0, BATTERY_HEIGHT, 0));
	pos2->translation->set(Vec3f(0, -BATTERY_HEIGHT, 0));

	pos->children->add(sh_cyl1);
	pos2->children->add(sh_cyl2);
	children->add(pos);
	children->add(pos2);
	
	//ECManager::getInstance()->addLine(this);
}

ECBattery::ECBattery(const ECBattery &)
{
}

ECBattery::~ECBattery()
{
}

void ECBattery::subSetActif(bool a)
{
	if (a)
	{
		sh_cyl1->appearance->get()->material->get()->diffuse_color->set(RGB(0.2, 0.2, 0.4));
		sh_cyl2->appearance->get()->material->get()->diffuse_color->set(RGB(0.2, 0.2, 0.4));
	}
	else
	{
		if (_alreadyGoneInto)
		{
			sh_cyl1->appearance->get()->material->get()->diffuse_color->set(RGB(0.05, 0.05, 0.1));
			sh_cyl2->appearance->get()->material->get()->diffuse_color->set(RGB(0.05, 0.05, 0.1));
		}
		else
		{
			sh_cyl1->appearance->get()->material->get()->diffuse_color->set(RGB(0.1, 0.1, 0.2));
			sh_cyl2->appearance->get()->material->get()->diffuse_color->set(RGB(0.1, 0.1, 0.2));
		}
	}
}

void ECBattery::initialize()
{
}

string ECBattery::getEffectMessage() const
{
	if (_begin_n == NULL || _end_n == NULL)
		return "ERROR";
	//we invert y and z to have the circuit on the horizontal plane
	Vec3f rotatedbegin = unrotateTranslatePart(_begin_n->getCoords());
	Vec3f rotatedend = unrotateTranslatePart(_end_n->getCoords());
	MicoleStringStream s;
	s << "EC BAT : seg=((" << \
		rotatedbegin.x << ", " << rotatedbegin.y << ", " << rotatedbegin.z << "),(" << \
		rotatedend.x << ", " << rotatedend.y << ", " << rotatedend.z << "));";
	return s.str();
}
