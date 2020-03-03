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

#include "ECPhantomManager.h"

#include "ECReplay.h"
#include "MicoleStringStream.h"

ECPhantomManager *ECPhantomManager::_ecpmInstance = NULL;

ECPhantomManager::ECPhantomManager()
:_lastObject(NULL), _readingPICOBg(false)
{
}

ECPhantomManager::ECPhantomManager(DEFMap *sceneMap)
:ECManager(sceneMap), _lastObject(NULL), _readingPICOBg(false)
{
}

ECPhantomManager::~ECPhantomManager()
{
}

void ECPhantomManager::onInput()
{
/*
	//version on the horizontal plane
	Vec3f rotatedposition = _position;
	float temp = rotatedposition.y;
	rotatedposition.y = - rotatedposition.z;
	rotatedposition.z = temp - CIRCUITHEIGHT;*/
	
	Vec3f rotatedposition = rotateTranslatePart(_position);

	bool newobject = false;
	
	if (_readingPICOBg)// || _suspended)
		return;
		
	ECObject *o = NULL;
	if (_lastObject)
	{
		_lastObject->setActif(false);
		o = _lastObject->getNearest(rotatedposition);
	}
	else
		o = ECManager::projectPosition(rotatedposition);

	if (o)
	{
		if (_lastObject != o)
			newobject = true;
		_lastObject = o;
		_lastObject->setAlreadyGoneInto();
		sendMessage(_lastObject->getEffectMessage());
	}

	activateLastObject(rotatedposition, newobject);
}

void ECPhantomManager::activateLastObject(const Vec3f &position, const bool &newobject)
{
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
			cursor = _lastObject->projection(position);
		if (newobject || (_lastWayPoint - cursor).length() >= MINRECORDINGDISTANCE)
		{
			MicoleStringStream s;
			s << "EC CURSOR : pos=(" << cursor.x << ", " << cursor.y << ", " << cursor.z << ");";
			if (specialpoint)
				s << " special=yes;";
			sendMessage(s.str());
			_lastWayPoint = cursor;
		}
	}
}

void ECPhantomManager::setNearest()
{
	Vec3f rotatedposition = rotateTranslatePart(_position);
	if (_lastObject)
		_lastObject->setActif(false);
	_lastObject = projectPosition(rotatedposition);
	_lastObject->setAlreadyGoneInto();
	sendMessage(_lastObject->getEffectMessage());
	activateLastObject(rotatedposition, true);
}

void ECPhantomManager::onButtonDown()
{
	ECReplay *ecr = ECReplay::getInstance();
	if (ecr->isReplaying())
	{
		_readingPICOBg = true;
		ecr->guideTo();
	}
	else if (_lastObject)
	{
		_readingPICOBg = true;
		Vec3f movedposition = _position;
		movedposition.y = _position.y - CIRCUITHEIGHT;
		movedposition.z = _position.z;
		sendMessage(_lastObject->getPICOBgMessage(movedposition));//_position));//rotatedposition));
	}
}

void ECPhantomManager::onButtonUp()
{
	sendMessage("EC PICOBG STOP;");
	_readingPICOBg = false;
	if (_lastObject)
		sendMessage(_lastObject->getEffectMessage());
}

ECObject *ECPhantomManager::getLastObject()
{
	return _lastObject;
}
