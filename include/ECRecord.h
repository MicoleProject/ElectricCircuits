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

 #ifndef ECRecord_H
#define ECRecord_H

#include <Vrml.h>

#include "KeyboardInputAgent.h"

using namespace Reachin;

/**
 * implementation of an input agent for test purpose
 * @warning this class is not part of the api. It's only an example of MicoleApi use.
 */
class ECRecord : public KeyboardInputAgent
{
	public:
		virtual ~ECRecord();

		//virtual void onInput();

		virtual void onKey(int key);
		virtual void handleCursor(MicoleBus *app, int argc, const char **argv);
		virtual void handleSpecialCursor(MicoleBus *app, int argc, const char **argv);
		
		void startRecord();
		void stopRecord();
		
		static ECRecord *getInstance()
		{
			if (!_ecrInstance)
				_ecrInstance = new ECRecord;
			return _ecrInstance;
		}
	
	private:
		ECRecord();
		
		//Vec3f _grab;
		bool _recording;
		FILE *_recordFile;
		static ECRecord *_ecrInstance;
};

#endif
