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

#include "ECRecord.h"
#include "common.h"

#include "MicoleStringStream.h"

ECRecord *ECRecord::_ecrInstance = NULL;

ECRecord::ECRecord()
:_recording(false), _recordFile(NULL)
{
	bindMessage("^EC CURSOR : pos=\\((.*), (.*), (.*)\\);$", 
		BUS_CALLBACK_OF(ECRecord, handleCursor));
	bindMessage("^EC CURSOR : pos=\\((.*), (.*), (.*)\\); special=yes;$", 
		BUS_CALLBACK_OF(ECRecord, handleSpecialCursor));
}

ECRecord::~ECRecord(void)
{
}

void ECRecord::onKey(int keycode)
{
	// key R
	if (keycode==114)
	{
		sendMessage("OUT FF3D GUIDE : none;");
		startRecord();
	}
	// key S
	if (keycode==115)
	{
		stopRecord();
	}
}

void ECRecord::startRecord()
{
	if (!_recording)
	{
		if (!fopen_s(&_recordFile, "record", "w"))
			_recording = true;
	}	
}

void ECRecord::stopRecord()
{
	if (_recording)
	{
		_recording = false;
		fclose(_recordFile);
	}	
}

//void ECRecord::onInput()
void ECRecord::handleCursor(MicoleBus *app, int argc, const char **argv)
{
	if (argc < 3)
		return;
	Vec3f cursorpos(unrotateTranslatePart(atof(argv[0]), atof(argv[1]), atof(argv[2])));

	if (_recording)
	{
		MicoleStringStream s;
		s << cursorpos.x << " " << cursorpos.y << " " << cursorpos.z << "\n";
		fputs(s.str().c_str(), _recordFile);
	}	
}

//void ECRecord::onInput()
void ECRecord::handleSpecialCursor(MicoleBus *app, int argc, const char **argv)
{
	if (argc < 3)
		return;
	Vec3f cursorpos(unrotateTranslatePart(atof(argv[0]), atof(argv[1]), atof(argv[2])));

	if (_recording)
	{
		MicoleStringStream s;
		s << cursorpos.x << " " << cursorpos.y << " " << cursorpos.z << " *\n";
		fputs(s.str().c_str(), _recordFile);
	}	
}
