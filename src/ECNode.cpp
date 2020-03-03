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

//Reachin Includes
#include <Vrml.h>
#include <Shape.h>
#include <Appearance.h>
#include <Material.h>
#include <Sphere.h>

//Other includes
//#include <sstream>
using namespace std;

#include <ReachinMicoleApplication.h>
#include <MicoleStringStream.h>

#include "ECNode.h"
#include "ECLine.h"
#include "common.h"

//Reachin stuff
const Interface ECNode::interface(
	"Node", typeid( ECNode ),
	Create<ECNode>::create,
	exposedField("position", &Transform::translation)
);

ECNode::ECNode()
{
	sh = new Shape;
	Appearance * app = new Appearance;
	Material * mat = new Material;
	Sphere * sph = new Sphere;

	sph->radius->set(SPHERE_RADIUS);
	mat->diffuse_color->set(RGB(0.15,0.15,0.15));
	mat->specular_color->set(RGB(0.92, 0.91, 0.89));
	mat->ambient_intensity->set(0);
	mat->shininess->set(0.06);

	app->material->set(mat);

	sh->appearance->set(app);
	sh->geometry->set(sph);

	children->add( sh );
}

ECNode::~ECNode()
{
}

void ECNode::initialize()
{
	_center = translation.get()->get();
}

void ECNode::addNeighbour(ECLine *l)
{
	if (l)
		_listLines.push_back(l);
}

float ECNode::distance(const Vec3f & phantomPos) const
{
	return (_center - phantomPos).length() - SPHERE_RADIUS;
}

Vec3f ECNode::projection(const Vec3f & phantomPos) const
{
	Vec3f result(phantomPos - _center);
	result.setLength(SPHERE_RADIUS);
	return _center + result;
}

ECObject *ECNode::getNearest(const Vec3f & phantomPos)
{
	float mindistance = distance(phantomPos) + SPHERE_RADIUS/2, tempdis;
	ECObject *result = NULL;

	for (list<ECLine *>::iterator i = _listLines.begin(); i != _listLines.end(); i++)
	{
		if ((tempdis = (*i)->distance(phantomPos)) < mindistance)
		{
			mindistance = tempdis;
			result = *i;
		}
	}
	return result;
}

string ECNode::getEffectMessage() const
{
	//we invert y and z to have the circuit on the horizontal plane
	Vec3f rotatedpos = unrotateTranslatePart(_center);
	MicoleStringStream s;
	s << "EC NODE : pos=(" << rotatedpos.x << ", " << rotatedpos.y << ", " << rotatedpos.z << ");";
	return s.str();
}

string ECNode::getPICOBgMessage(const Vec3f & phantomPos)
{
	//we invert y and z to have the circuit on the horizontal plane
	Vec3f rotatedpos = unrotateTranslatePart(_center);
	MicoleStringStream s;
	s << "EC PICOBG : pos=(" << rotatedpos.x << ", " << rotatedpos.y << ", " << rotatedpos.z << "); bumps=(";
	bool first = true;

	for (list<ECLine *>::iterator i = _listLines.begin() ; i != _listLines.end(); i++)
	{
		if (!first)
			s << ",";
		first = false;
		
		Vec3f temp = ((*i)->endNode()->getCoords() - (*i)->beginNode()->getCoords());
		if ((*i)->hasAlreadyGoneInto())
			temp.setLength(LITTLE_GBUMPS_HEIGHT);
		else
			temp.setLength(BIG_GBUMPS_HEIGHT);
		Vec3f rotatedpos;
		if ((*i)->beginNode() == this)
			rotatedpos = unrotatePart(temp);
		else if ((*i)->endNode() == this)
			rotatedpos = - unrotatePart(temp);
		s << "(" << rotatedpos.x << ", " << rotatedpos.y << ", " << rotatedpos.z << ")";
	}
	s << ");";
	return s.str();
}

Vec3f ECNode::getCoords() const
{
	return _center;
}

void ECNode::setActif(bool a)
{
	if (a)
		sh->appearance->get()->material->get()->diffuse_color->set(RGB(0.15, 0, 0.94));
	else
	{
		if (_alreadyGoneInto)
			sh->appearance->get()->material->get()->diffuse_color->set(RGB(0.05, 0.05, 0.05));
		else
			sh->appearance->get()->material->get()->diffuse_color->set(RGB(0.15,0.15,0.15));
	}
	subSetActif(a);
}

void ECNode::subSetActif(bool a)
{
}
