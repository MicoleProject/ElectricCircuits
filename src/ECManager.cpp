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

#include "ECManager.h"
#include "ECReplay.h"
#include "MicoleStringStream.h"

map<string, ECNode *> ECManager::_nodesList;
list<ECLine *> ECManager::_linesList;
DEFMap *ECManager::_sceneMap = NULL;

ECManager::ECManager()
{
}

ECManager::ECManager(DEFMap *sceneMap)
{
	if (!_sceneMap)
		_sceneMap = sceneMap;
}

ECManager::~ECManager()
{
}

ECNode *ECManager::findNode(const char *s)
{
	ECNode *result;
	try
	{
		_sceneMap->find(s, result);
	}
	catch(...)
	{
		return NULL;
	}
	return result;
}

void ECManager::addLine(ECLine *l)
{
	//should not happen, but this can prevent unwanted problems
	if (l)
		_linesList.push_back(l);
}

void ECManager::addNode(const string &s, ECNode *n)
{
	if (n && !_nodesList[s])
		_nodesList[s] = n;
}

void ECManager::updateLines()
{
	for (list<ECLine *>::iterator i = _linesList.begin(); i != _linesList.end(); i++)
		(*i)->updateEnds();
}

ECObject *ECManager::projectPosition(const Vec3f &pos)
{
	ECObject *o = NULL;
	//params : i:rotatedposition, o:o
	float mindis = 1000, tempdis;
	for (list<ECLine *>::iterator i = _linesList.begin(); i != _linesList.end(); i++)
	{
		if ((tempdis = (*i)->distance(pos)) < mindis)
		{
			mindis = tempdis;
			o = *i;
		}
	}
	for (map<string, ECNode *>::iterator i = _nodesList.begin(); i != _nodesList.end(); i++)
	{
		if ((tempdis = (i->second)->distance(pos)) < mindis)
		{
			mindis = tempdis;
			o = i->second;
		}
	}
	return o;
}

/*
void ECManager::onInput()
{
	Vec3f rotatedposition = rotateTranslatePart(_position);
	bool newobject = false;

	if (_readingPICOBg || _suspended)
		return;
	_lastObject->setActif(false);
	
	ECObject *o = NULL;
	if (_lastObject)
	{
		_lastObject->setActif(false);
		o = _lastObject->getNearest(rotatedposition);
	}
	else
	{
		// projection
		//params : i:rotatedposition, o:o
		float mindis = 1000, tempdis;
		for (list<ECLine *>::iterator i = _linesList.begin(); i != _linesList.end(); i++)
		{
			if ((tempdis = (*i)->distance(rotatedposition)) < mindis)
			{
				mindis = tempdis;
				o = *i;
			}
		}
		for (map<string, ECNode *>::iterator i = _nodesList.begin(); i != _nodesList.end(); i++)
		{
			if ((tempdis = (i->second)->distance(rotatedposition)) < mindis)
			{
				mindis = tempdis;
				o = i->second;
			}
		}
	}
	if (o)
	{
		if (_lastObject != o)
			newobject = true;
		_lastObject = o;
		_lastObject->setAlreadyGoneInto();
		sendMessage(_lastObject->getEffectMessage());
	}
	if (_lastObject)
	{
		_lastObject->setActif(true);
		
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
			s << "EC CURSOR : pos=" << "(" << cursor.x << ", " << cursor.y << ", " << cursor.z << ");";
			if (specialpoint)
				s << " special=yes;";
			sendMessage(s.str());
			_lastWayPoint = cursor;
		}
	}
}*/
