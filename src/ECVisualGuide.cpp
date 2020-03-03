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

#include <Shape.h>
#include <Appearance.h>
#include <Material.h>
#include <Sphere.h>

#include "ECVisualGuide.h"
#include "common.h"

#include <Display.h>
#include <ReachinMicoleApplication.h>

ECVisualGuide *ECVisualGuide::_ecvInstance=NULL;

ECVisualGuide::ECVisualGuide()
:MicoleAgent("ECVisualGuide","INPUT")
{
	Shape *sh_sph = new Shape;
	Appearance * app_sph = new Appearance;
	Material * mat_sph = new Material;
	Sphere * sph = new Sphere;

	sph->radius->set(0.0045);
	mat_sph->diffuse_color->set(RGB(0.0,0.8, 0.8));
	mat_sph->specular_color->set(RGB(0.2, 0.9, 0.9));
	mat_sph->ambient_intensity->set(0.5);
	mat_sph->shininess->set(1.0);
	mat_sph->transparency->set(0.2);
	app_sph->material->set(mat_sph);

	sh_sph->appearance->set(app_sph);
	sh_sph->geometry->set(sph);
	children->add(sh_sph);

	Display * dis = static_cast<Display *>(ReachinMicoleApplication::getInstance()->getDisplay());
	
	dis->children->add(this);  
	bindMessage( "^OUT FF3D GUIDE : point=\\((.*), (.*), (.*)\\);$", BUS_CALLBACK_OF(ECVisualGuide, handleInput ));
}

void ECVisualGuide::handleInput( MicoleBus *app, int argc, const char **argv) {
	if (argc < 3)
		return;

	Vec3f pos;

	pos.x = atof(argv[0]);
	pos.y = atof(argv[1]);
	pos.z = atof(argv[2]);

	Vec3f rotatedposition = rotateTranslatePart(pos);

	translation->set(rotatedposition);
}

ECVisualGuide::~ECVisualGuide()
{
}

ECVisualGuide * ECVisualGuide::getInstance()
{
	if (_ecvInstance == NULL)
		_ecvInstance = new ECVisualGuide;
	return _ecvInstance;
}
