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

#include "common.h"
#include "ECResistor.h"
#include "ECManager.h"
#include "MicoleStringStream.h"

#define RESISTOR_RADIUS 0.004
#define RESISTOR_HEIGHT 0.02

const Interface ECResistor::interface(
	"Resistor", typeid( ECResistor ),
	Create<ECResistor>::create,
	&ECLine::interface
);

ECResistor::ECResistor()
{
	sh = new Shape;
	Appearance * app = new Appearance;
	Material * mat = new Material;
	Cylinder * cyl = new Cylinder;

	cyl->radius->set(RESISTOR_RADIUS);
	cyl->height->set(RESISTOR_HEIGHT);
	mat->diffuse_color->set(RGB(0.9, 0.7, 0.5));
	mat->specular_color->set(RGB(0.1, 0.1, 0.1));
	mat->ambient_intensity->set(0);
	mat->shininess->set(0.05);

	app->material->set(mat);

	sh->appearance->set(app);
	sh->geometry->set(cyl);

	children->add(sh);
	
	//ECManager::getInstance()->addLine(this);
}

void ECResistor::subSetActif(bool a)
{
	if (a)
		sh->appearance->get()->material->get()->diffuse_color->set(RGB(0.9, 0.8, 0.6));
	else
	{
		if (_alreadyGoneInto)
			sh->appearance->get()->material->get()->diffuse_color->set(RGB(0.6, 0.4, 0.2));
		else
			sh->appearance->get()->material->get()->diffuse_color->set(RGB(0.9, 0.7, 0.5));
	}
}

void ECResistor::initialize()
{
}

string ECResistor::getEffectMessage() const
{
	if (_begin_n == NULL || _end_n == NULL)
		return "ERROR";
	//we invert y and z to have the circuit on the horizontal plane
	Vec3f rotatedbegin = unrotateTranslatePart(_begin_n->getCoords());
	Vec3f rotatedend = unrotateTranslatePart(_end_n->getCoords());
	MicoleStringStream s;
	s << "EC RES : seg=((" << \
		rotatedbegin.x << ", " << rotatedbegin.y << ", " << rotatedbegin.z << "),(" << \
		rotatedend.x << ", " << rotatedend.y << ", " << rotatedend.z << "));";
	return s.str();
}
