/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: George Beinas <gbeinas@gmail.com>
 */


/*********************** MODEL DESCRIPTION ************************/


  /*
   * Scenario topology - Building Infrastructure
   * 5 eNB, and 10 HeNBs (femtocells) and 20 WiFi access points
   * xx UEs ( depends on each run )
   * 1 floor, 8 rooms
   */


  /*	    ----------------------------- 100m ------------------------------
 *
 * 		|	*****************************************************************
 * 		|	*				*				*				*				*
 *		| 	*				*				*				*				*
 *		| 	*				*				*				*				*
 *		| 	*				*				*				*				*
 *	   50m 	*****************************************************************
 *		| 	*				*				*				*				*
 *		| 	*				*				*				*				*
 *		| 	*				*				*				*				*
 *		| 	*				*				*				*				*
 *		| 	*				*				*				*				*
 *		| 	*****************************************************************
 */
 /* -------------------------------------------------------------------*/

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/lte-module.h"
#include "ns3/config-store.h"
#include <ns3/buildings-helper.h>
#include <ns3/buildings-module.h>
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/animation-interface.h"
#include "ns3/random-variable-stream.h"

#include <ns3/funcs.h>

using namespace ns3;
using namespace std;

int main (int argc, char *argv[])
{

	CommandLine cmd;
	uint32_t seed, run;
	cmd.AddValue("seed", "Define seed value", seed);
	cmd.AddValue("run", "Define run value", run);
	cmd.Parse (argc, argv);
	RngSeedManager::SetSeed (seed);
	RngSeedManager::SetRun (run);
	// Store the programm parameters during the simulation
	/*ConfigStore config;
	config.ConfigureDefaults ();*/

	/* ----------------Arithmetic Variables --------------*/
	static uint16_t nHeNbs = 10;
	static uint16_t neNbs = 4;
	static uint16_t nWiFi = 20;
	static const uint16_t nUEs = 10;

	// Create the Building Infrastructure
	Ptr<Building> build = CreateObject<Building> ();
	Ptr<MobilityBuildingInfo> mbi;
	MobilityHelper mobility;
	NetDeviceContainer enbLteDevs, ueLteDevs,HenbLteDevs;
	NodeContainer eNbsnodes, HeNbsnodes, UEsnodes, remoteHostContainer, WiFinodes;
	Ptr<ConstantPositionMobilityModel> mm0;
	Ptr<MobilityModel> mob;
	Vector pos;
	createBuilding(300,401,300,351,0,10,4,2,1,build,mbi);
	createMobility(mobility,mm0,neNbs,nHeNbs,nUEs,nWiFi,eNbsnodes,HeNbsnodes,UEsnodes,WiFinodes,remoteHostContainer,300,401,300,351);
	printTopology(mob,pos,neNbs,nHeNbs,nWiFi,eNbsnodes, HeNbsnodes,WiFinodes);

	//double rand_val;
	//rand_val = double(getRandom());
	//cout << "Random value is " << getRandom(5,2) << endl;
	Simulator::Stop (Seconds (2.00));

	/*config.ConfigureAttributes ();*/
	Simulator::Run ();

	Simulator::Destroy ();
	return 0;

}
