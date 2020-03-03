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

// ugly patch against the inclusion loop problem...
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x05000
#endif

//#define WIN32_LEAN_AND_MEAN

/*#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_
#include <windows.h>*/

#pragma warning (disable : 4251)

#include <ReachinMicoleApplication.h>
#include <LoggerAgent.h>
#include <FF3DDeviceSenderAgent.h>
#include <FF3DDeviceOutputAgent.h>
#include <VTPlayerInputAgent.h>
#include <VTPlayerOutputAgent.h>
#include <VTPlayerSenderAgent.h>
#include <KeyboardSenderAgent.h>
#include <MouseSenderAgent.h>

#include <FF3DGuidanceAgent.h>

#include "ECManager.h"
#include "ECMouseManager.h"
#include "ECPhantomManager.h"
#include "ECPhantomAgent.h"
#include "ECVisualAgent.h"

#include <Vrml.h>
#include <Display.h>
#include <Scene.h>
#include <Node.h>
#include <Field.h>

#include <Shape.h>
#include <Appearance.h>
#include <Material.h>
#include <Sphere.h>

#ifdef __VTPLAYER__
#include "ECVTPlayerAgent.h"
#endif

#include "ECRecord.h"
#include "ECReplay.h"
#include "ECVisualGuide.h"

#include <iostream>

using namespace Reachin;
using namespace std;

int main(int argc, char* argv[])
{
	if( argc < 2 )
	{
		cerr << "Usage: " << argv[0] << " circuitfile" << endl;
		exit(1);
	}

	//try
	//{
		cout << "create the logger agent" << endl;
		LoggerAgent * la = new LoggerAgent();
		//Sleep(1000);

#ifdef __VTPLAYER__
		try 
		{
			//Sleep(1000);
			cout << "create the VTPOA agent" << endl;
			VTPlayerOutputAgent * vtoa = new VTPlayerOutputAgent();

			cout << "create the VTPlayer agent" << endl;
			ECVTPlayerAgent * ecvtp = new ECVTPlayerAgent();
			
			VTPlayerSenderAgent * vtsa = new VTPlayerSenderAgent();
		} 
		catch(...)
		{
		}
#endif
		//Sleep(1000);
		cout << "create the FF3DDO agent" << endl;
		FF3DDeviceOutputAgent * oa = new FF3DDeviceOutputAgent();
		FF3DGuidanceAgent * ga = new FF3DGuidanceAgent();
		cout << "Remove the stylus" << endl;

		ReachinMicoleApplication * ama = ReachinMicoleApplication::getInstance();
		
		/*ama->sendIvyMessage("Set stiffness = 2;");
		ama->sendIvyMessage("Set d = 0.005;");*/
		
		ama->sendIvyMessage("Set stiffness = 2.5;");
		ama->sendIvyMessage("Set d = 0.003;");

		//we remove the stylus
		ama->getDisplay()->stylus->set(new Transform ());
		ama->getDisplay()->tip->set(new Transform());
		cout << "create the Phantom agent" << endl;
		ECPhantomAgent * ecp = ECPhantomAgent::getInstance();
		cout << "create the Visual agent" << endl;
		ECVisualAgent * ecv = ECVisualAgent::getInstance();

		ECVisualGuide * evg = ECVisualGuide::getInstance();

		ECRecord * rec = ECRecord::getInstance();
		ECReplay * rep = ECReplay::getInstance();

		DEFMap sceneMap;
		ECManager *man = ECPhantomManager::createInstance(&sceneMap);
		ECManager *man2 = ECMouseManager::getInstance();
		Group *g = VRML::createVrmlFromURL(argv[1], &sceneMap);
		ama->getDisplay()->children->add(g);
		ama->getDisplay()->children->add(ecv);
		ama->getDisplay()->children->add(dynamic_cast<ECMouseManager *>(man2));

		ECManager::updateLines();

		KeyboardSenderAgent *ksa = new KeyboardSenderAgent();
		FF3DDeviceSenderAgent * ffsa = new FF3DDeviceSenderAgent(16);
		MouseSenderAgent * msa = new MouseSenderAgent();

		ama->startScene();

		
	//}
	/*catch( ... )
	//{
		cerr << "Unknown error" << endl;
		system("pause");
	}*/
	return 0;
}