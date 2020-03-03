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
#include <Cylinder.h>

//Other includes
using namespace std;
#include <ReachinMicoleApplication.h>
#include <MicoleStringStream.h>

#include "ECLine.h"
#include "ECManager.h"
#include "common.h"

//Reachin stuff
const Interface ECLine::interface(
	"Line", typeid( ECLine ),
	Create<ECLine>::create,
	exposedField("begin", &ECLine::_begin) +
	exposedField("end", &ECLine::_end)
);

ECLine::ECLine(Defined< SFString > begin, Defined< SFString > end)
:_begin(begin), _end(end), _begin_n(NULL), _end_n(NULL)
{
	sh = new Shape;
	Appearance * app = new Appearance;
	Material * mat = new Material;
	Cylinder * cyl = new Cylinder;

	line_scaler = new Transform;

	cyl->radius->set(CYLINDER_RADIUS);
	cyl->height->set(1);
	mat->diffuse_color->set(RGB(0.44, 0.15, 0));
	mat->specular_color->set(RGB(0.79, 0.69, 0.4));
	mat->ambient_intensity->set(0);
	mat->shininess->set(0.12);

	app->material->set(mat);

	sh->appearance->set(app);
	sh->geometry->set(cyl);

	line_scaler->children->add(sh);

	children->add(line_scaler);

	ECManager::addLine(this);
}

ECLine::~ECLine()
{
}

void ECLine::initialize()
{
}

float ECLine::distance(const Vec3f & phantomPos) const
{
	if (_begin_n == NULL || _end_n == NULL)
		return 1000;
	Vec3f a = _begin_n->translation.get()->get();
	a.z = 0.0;
	Vec3f b = _end_n->translation.get()->get();
	b.z = 0.0;
	Vec3f m = phantomPos;
	//m.z = 0.0;
	Vec3f  ab = (b - a);
	ab.normalize();
	Vec3f  ba = (a - b);
	ba.normalize();
	
	Vec3f am = m - a, bm = m - b;
	float an = ab.dot(am);
	float bn = ba.dot(bm);

	//if we are not on the segment, consider we are 
	//very far, to force beeing on the nodes
	if (an < 0 || bn < 0)
		return 100;

	if (am.length() > bm.length())
		return sqrt(am.length() * am.length() - an*an);
	else
		return sqrt(bm.length() * bm.length() - bn*bn);
}

Vec3f ECLine::projection(const Vec3f & phantomPos) const
{
	if (_begin_n == NULL || _end_n == NULL)
		return Vec3f(0,0,0);
	Vec3f a = _begin_n->translation.get()->get();
	a.z = 0.0;
	Vec3f b = _end_n->translation.get()->get();
	b.z = 0.0;
	Vec3f m = phantomPos;
	//m.z = 0.0;
	Vec3f  ab = (b - a);
	ab.normalize();
	Vec3f  ba = (a - b);
	ba.normalize();
	
	Vec3f am = m - a, bm = m - b;
	float an = ab.dot(am);
	float bn = ba.dot(bm);

	//if we are not on the segment, consider we are 
	//very far, to force beeing on the nodes
	if (an < 0)
		return a;
	else if(bn < 0)
		return b;

	if (am.length() > bm.length())
	{
		ab.setLength(an);
		return a + ab;
	}
	else
	{
		ba.setLength(bn);
		return b + ba;
	}
}

ECObject *ECLine::getNearest(const Vec3f & phantomPos)
{
	if (_begin_n == NULL || _end_n == NULL)
		return NULL;
	mgFloat d1 = distance(phantomPos);
	mgFloat d2 = _begin_n->distance(phantomPos)+0.01;
	mgFloat d3 = _end_n->distance(phantomPos)+0.01;

	if (d1 < d2)
	{
		if (d1 < d3)
			return NULL;
		else
			return _end_n;
	}
	//return NULL;
	else
	{
		if (d2 < d3)
			return _begin_n;
		else
			return _end_n;
	}
}

string ECLine::getEffectMessage() const
{
	if (_begin_n == NULL || _end_n == NULL)
		return "ERROR";
	//we invert y and z to have the circuit on the horizontal plane
	Vec3f rotatedbegin = unrotateTranslatePart(_begin_n->getCoords());
	Vec3f rotatedend = unrotateTranslatePart(_end_n->getCoords());

	MicoleStringStream s;
	s << "EC LINE : seg=((" << \
		rotatedbegin.x << ", " << rotatedbegin.y << ", " << rotatedbegin.z << "),(" << \
		rotatedend.x << ", " << rotatedend.y << ", " << rotatedend.z << "));";
	return s.str();
}

string ECLine::getPICOBgMessage(const Vec3f &phantomPos)
{
	if (_begin_n == NULL || _end_n == NULL)
		return "ERROR";
	//we invert y and z to have the circuit on the horizontal plane
	Vec3f dir1 = unrotatePart(_begin_n->getCoords() - _end_n->getCoords());
	dir1.setLength(LITTLE_GBUMPS_HEIGHT);
	Vec3f dir2 = unrotatePart(_end_n->getCoords() - _begin_n->getCoords());
	dir2.setLength(LITTLE_GBUMPS_HEIGHT);
	MicoleStringStream s;
	s << "EC PICOBG : pos=(" << phantomPos.x << ", " << phantomPos.y + CIRCUITHEIGHT << ", " << phantomPos.z << "); bumps=((" << \
		dir1.x << ", " << dir1.y << ", " << dir1.z << "),(" << dir2.x << ", " << dir2.y << ", " << dir2.z << "));";
	return s.str();
}

ECNode *ECLine::beginNode() const
{
	return _begin_n;
}

ECNode *ECLine::endNode() const
{
	return _end_n;
}

void ECLine::updateEnds()
{
	try
	{
		_begin_n = ECManager::findNode(_begin.get()->get());
		_end_n = ECManager::findNode(_end.get()->get());
/*		sceneMap.find(_begin.get()->get(), _begin_n);
		sceneMap.find(_end.get()->get(), _end_n);*/
		if (_begin_n)
			_begin_n->addNeighbour(this);
		if (_end_n)
			_end_n->addNeighbour(this);
		
		//adds the nodes to the ECM nodeslist
		//will be used for a pathfinding for guidance
		ECManager::addNode(_begin.get()->get(), _begin_n);
		ECManager::addNode(_end.get()->get(), _end_n);
	}
	catch(...)
	{
		return;
	}
	
	//we compute the euclidian norma
	Vec3f vec_line = _begin_n->translation.get()->get() - _end_n->translation.get()->get();
	line_scaler->scale->set( Vec3f(1, vec_line.length(), 1) );

	//we compute the rotation
	//there is a bug when the component is vertical...
	rotation->set( Rotation( Vec3f(vec_line.z, 0, -vec_line.x), acos( vec_line.y / vec_line.length() ) ) );

	//we compute the translation
	translation->set( (_begin_n->translation.get()->get() + _end_n->translation.get()->get()) /2 ); 
}

void ECLine::setActif(bool a)
{
	if (a)
		sh->appearance->get()->material->get()->diffuse_color->set(RGB(0.94, 0.15, 0));
	else
	{
		if (_alreadyGoneInto)
			sh->appearance->get()->material->get()->diffuse_color->set(RGB(0.10, 0.05, 0));
		else
			sh->appearance->get()->material->get()->diffuse_color->set(RGB(0.44, 0.15, 0));
	}
	subSetActif(a);
}

void ECLine::subSetActif(bool a)
{
}
