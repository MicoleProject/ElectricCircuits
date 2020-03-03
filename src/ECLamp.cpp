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
#include "ECLamp.h"
#include "ECManager.h"

#define CYLINDERS_RADIUS 0.006
#define CYLINDERS_HEIGHT 0.002
#define LAMP_RADIUS 0.005

const Interface ECLamp::interface(
	"Lamp", typeid( ECLamp ),
	Create<ECLamp>::create,
	&ECLine::interface
);

ECLamp::ECLamp()
{
	sh_cross1 = new Shape;
	sh_cross2 = new Shape;
	sh_sph = new Shape;
	Appearance * app_sph = new Appearance;
	Appearance * app_cyl1 = new Appearance;
	Appearance * app_cyl2 = new Appearance;
	Material * mat_sph = new Material;
	Material * mat_cyl1 = new Material;
	Material * mat_cyl2 = new Material;
	Cylinder * cyl1 = new Cylinder;
	Cylinder * cyl2 = new Cylinder;
	Sphere * sph = new Sphere;
	Transform * pos_cross1 = new Transform;

	Transform * pos_cross2 = new Transform;

	cyl1->radius->set(CYLINDERS_RADIUS);
	cyl1->height->set(CYLINDERS_HEIGHT);
	cyl2->radius->set(CYLINDERS_RADIUS);
	cyl2->height->set(CYLINDERS_HEIGHT);
	sph->radius->set(LAMP_RADIUS);
	mat_cyl1->diffuse_color->set(RGB(0.1, 0.1, 0.2));
	mat_cyl1->specular_color->set(RGB(0.2, 0.2, 0.5));
	mat_cyl1->ambient_intensity->set(0);
	mat_cyl1->shininess->set(0.6);
	mat_cyl2->diffuse_color->set(RGB(0.1, 0.1, 0.2));
	mat_cyl2->specular_color->set(RGB(0.2, 0.2, 0.5));
	mat_cyl2->ambient_intensity->set(0);
	mat_cyl2->shininess->set(0.6);
	mat_sph->diffuse_color->set(RGB(0.8, 0.8, 0.0));
	mat_sph->specular_color->set(RGB(0.9, 0.9, 0.2));
	mat_sph->ambient_intensity->set(0.5);
	mat_sph->shininess->set(1.0);

	app_sph->material->set(mat_sph);
	app_cyl1->material->set(mat_cyl1);
	app_cyl2->material->set(mat_cyl2);

	sh_cross1->appearance->set(app_cyl1);
	sh_cross1->geometry->set(cyl1);
	sh_cross2->appearance->set(app_cyl2);
	sh_cross2->geometry->set(cyl2);
	sh_sph->appearance->set(app_sph);
	sh_sph->geometry->set(sph);

	pos_cross1->rotation->set(Rotation(0, 0, 1, 0.785));

	pos_cross2->rotation->set(Rotation(0, 0, -1, 0.785));

	pos_cross1->children->add(sh_cross1);
	pos_cross2->children->add(sh_cross2);
	children->add(pos_cross1);
	children->add(pos_cross2);
	children->add(sh_sph);
	
	//ECManager::getInstance()->addLine(this);
}

void ECLamp::subSetActif(bool a)
{
	if (a)
		sh_sph->appearance->get()->material->get()->diffuse_color->set(RGB(0.9, 0.8, 0.6));
	else
	{
		if (_alreadyGoneInto)
			sh_sph->appearance->get()->material->get()->diffuse_color->set(RGB(0.6, 0.4, 0.2));
		else
			sh_sph->appearance->get()->material->get()->diffuse_color->set(RGB(0.9, 0.7, 0.5));
	}
}

void ECLamp::initialize()
{
}

string ECLamp::getEffectMessage() const
{
	if (_begin_n == NULL || _end_n == NULL)
		return "ERROR";
	//we invert y and z to have the circuit on the horizontal plane
	Vec3f rotatedbegin = unrotateTranslatePart(_begin_n->getCoords());
	Vec3f rotatedend = unrotateTranslatePart(_end_n->getCoords());
	MicoleStringStream s;
	s << "EC LAMP : seg=((" << \
		rotatedbegin.x << ", " << rotatedbegin.y << ", " << rotatedbegin.z << "),(" << \
		rotatedend.x << ", " << rotatedend.y << ", " << rotatedend.z << "));";
	return s.str();
}
