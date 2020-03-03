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

#include "ECVisualAgent.h"
#include "common.h"

ECVisualAgent *ECVisualAgent::_ecvInstance=NULL;

ECVisualAgent::ECVisualAgent()
:lastpos(0, 0, 0), _mse(new Transform), _phtm(new Transform), p1(0,0,0), p2(0,0,0)
{
	Shape *sh_sph = new Shape;
	Appearance * app_sph = new Appearance;
	Material * mat_sph = new Material;
	Sphere * sph = new Sphere;

	sph->radius->set(0.005);
	mat_sph->diffuse_color->set(RGB(0.8, 0.8, 0.0));
	mat_sph->specular_color->set(RGB(0.9, 0.9, 0.2));
	mat_sph->ambient_intensity->set(0.5);
	mat_sph->shininess->set(1.0);
	mat_sph->transparency->set(0.2);
	app_sph->material->set(mat_sph);

	sh_sph->appearance->set(app_sph);
	sh_sph->geometry->set(sph);
	_phtm->children->add(sh_sph);

	Shape *sh_sph2 = new Shape;
	Appearance * app_sph2 = new Appearance;
	Material * mat_sph2 = new Material;
	Sphere * sph2 = new Sphere;

	sph2->radius->set(0.0047);
	mat_sph2->diffuse_color->set(RGB(0.8, 0.0, 0.0));
	mat_sph2->specular_color->set(RGB(0.9, 0.2, 0.2));
	mat_sph2->ambient_intensity->set(0.5);
	mat_sph2->shininess->set(1.0);
	mat_sph2->transparency->set(0.2);
	app_sph2->material->set(mat_sph2);

	sh_sph2->appearance->set(app_sph2);
	sh_sph2->geometry->set(sph2);
	_mse->children->add(sh_sph2);
	
	children->add(_phtm);
	children->add(_mse);

	bindMessage("^EC LINE : seg=\\(\\((.*), (.*), (.*)\\),\\((.*), (.*), (.*)\\)\\);$", 
		BUS_CALLBACK_OF(ECVisualAgent, handleOnALine));
	bindMessage("^EC RES : seg=\\(\\((.*), (.*), (.*)\\),\\((.*), (.*), (.*)\\)\\);$", 
		BUS_CALLBACK_OF(ECVisualAgent, handleOnALine));
	bindMessage("^EC LAMP : seg=\\(\\((.*), (.*), (.*)\\),\\((.*), (.*), (.*)\\)\\);$", 
		BUS_CALLBACK_OF(ECVisualAgent, handleOnALine));
	bindMessage("^EC CAP : seg=\\(\\((.*), (.*), (.*)\\),\\((.*), (.*), (.*)\\)\\);$", 
		BUS_CALLBACK_OF(ECVisualAgent, handleOnALine));
	bindMessage("^EC BAT : seg=\\(\\((.*), (.*), (.*)\\),\\((.*), (.*), (.*)\\)\\);$", 
		BUS_CALLBACK_OF(ECVisualAgent, handleOnALine));
	bindMessage("^EC NODE : pos=\\((.*), (.*), (.*)\\);$", 
		BUS_CALLBACK_OF(ECVisualAgent, handleOnANode));
	bindMessage("^EC NOTHING;$", BUS_CALLBACK_OF(ECVisualAgent, handleNothing));
	bindMessage("^EC MSE CURSOR : pos=\\((.*), (.*), (.*)\\);", BUS_CALLBACK_OF(ECVisualAgent, handleMouse));
}

ECVisualAgent::~ECVisualAgent()
{
}

//EC LINE|RES|CAP|BAT|LAMP : seg=((x,y,z),(x,y,z));
void ECVisualAgent::handleOnALine( MicoleBus *app, int argc, const char **argv)
{
	if (argc < 6)
		return;
	//Vec3f p1, p2;
	try
	{
		p1.x = atof(argv[0]);
		p1.y = atof(argv[1]);
		p1.z = atof(argv[2]);
		p2.x = atof(argv[3]);
		p2.y = atof(argv[4]);
		p2.z = atof(argv[5]);
	}
	catch (...) //can append for truncated packet or bad transmission
	{
		return;
	}
}

void ECVisualAgent::onInput()
{
	Vec3f am = _position - p1, bm = _position - p2;
	Vec3f saveab = p2 - p1;
	Vec3f pos;
	if (saveab.length() < 0.00001)
	{
		pos = rotateTranslatePart(p1);
		_phtm->translation->set(pos);
		lastpos = pos;
		return;
	}
	if (am.length() > bm.length())
	{
		saveab.setLength(saveab.dot(am)/saveab.length());
		if (saveab.length() > (p2 - p1).length())
			pos = p2;
		else
			pos = p1 + saveab;
	}
	else
	{
		saveab.setLength(saveab.dot(bm)/saveab.length());
		if (saveab.length() > (p1 - p2).length())
			pos = p1;
		else
			pos = p2 + saveab;
	}
	pos = rotateTranslatePart(pos);
	if ((pos - lastpos).length() > VISUALLAG)
	{
		Vec3f shift(pos - lastpos);
		shift.setLength(VISUALLAG);
		lastpos += shift;
		_phtm->translation->set(lastpos);
	}
	else
	{
		_phtm->translation->set(pos);
		lastpos = pos;
	}
}

//EC MSE CURSOR : pos=(x,y,z)
void ECVisualAgent::handleMouse(MicoleBus *app, int argc, const char **argv)
{
	if (argc < 3)
		return;
	Vec3f mp;
	try
	{
		mp.x = atof(argv[0]);
		mp.y = atof(argv[1]);
		mp.z = atof(argv[2]);
	}
	catch (...) //can append for truncated packet or bad transmission
	{
		return;
	}
	_mse->translation->set(mp);//rotateTranslatePart(mp));
}

//EC NODE : pos=(x,y,z);
void ECVisualAgent::handleOnANode( MicoleBus *app, int argc, const char **argv)
{
	if (argc < 3)
		return;
	//Vec3f p;
	try
	{
		p1.x = atof(argv[0]);
		p1.y = atof(argv[1]);
		p1.z = atof(argv[2]);
		p2.x = atof(argv[0]);
		p2.y = atof(argv[1]);
		p2.z = atof(argv[2]);
	}
	catch (...) //can append for truncated packet or bad transmission
	{
		return;
	}
//	translation->set(p);
}

//EC NOTHING;
void ECVisualAgent::handleNothing( MicoleBus *app, int argc, const char **argv)
{
	p1.x = 100.0;
	p1.y = 100.0;
	p1.z = 100.0;
	p2.x = 100.0;
	p2.y = 100.0;
	p2.z = 100.0;
}

ECVisualAgent * ECVisualAgent::getInstance()
{
	if (_ecvInstance == NULL)
		_ecvInstance = new ECVisualAgent;
	return _ecvInstance;
}
