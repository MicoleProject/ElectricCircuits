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

#ifndef ECReplay_H
#define ECReplay_H

#include <Vrml.h>
using namespace Reachin;

#include <list>
using namespace std;

#include <FF3DDeviceInputAgent.h>
#include <KeyboardInputAgent.h>

/**
 * implementation of an input agent for test purpose
 * @warning this class is not part of the api. It's only an example of MicoleApi use.
 */
class ECReplay : public FF3DDeviceInputAgent, KeyboardInputAgent
{
	public:
		virtual ~ECReplay(void);

		//virtual void onInput();
		
		void onKey(int key);
		void onInput();
		
		void handleOnALine(MicoleBus *app, int argc, const char **argv);
		void handleOnANode(MicoleBus *app, int argc, const char **argv);
		void handleMseCursor(MicoleBus *app, int argc, const char **argv);
		void handleMseSpecialCursor(MicoleBus *app, int argc, const char **argv);
		
		void startReplay();
		void stopReplay();
		bool readNextPoint();
		bool isReplaying() const;
		void guideTo();
		
		void stopGuidance();

		static ECReplay *getInstance()
		{
			if (!_ecrInstance)
				_ecrInstance = new ECReplay;
			return _ecrInstance;
		}

	private:
		ECReplay();
		bool _replay;
		CRITICAL_SECTION _replayMutex;
		FILE *_recordFile;
		Vec3f _magneticPoint;
		list<Vec3f> _wayPoints;
		Vec3f _lastNodePos;
		static ECReplay *_ecrInstance;
		bool _beginningReplay;
		bool _special;
		bool _isguided;
		Vec4f _lastMsePos;
		list<Vec4f> _guidePoints;
};

#endif
