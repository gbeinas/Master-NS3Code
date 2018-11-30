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
#include "ns3/config-store.h"
#include <ns3/buildings-helper.h>
#include <ns3/buildings-module.h>
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/animation-interface.h"
#include "ns3/random-variable-stream.h"
#include "ns3/lte-helper.h"
#include "ns3/epc-helper.h"
#include "ns3/lte-module.h"

#include <ns3/funcs.h>

using namespace ns3;
using namespace std;


int main (int argc, char *argv[])
{
	/* ----------------Command-Line Variables --------------*/
	Time::SetResolution(Time::NS);
	cmd.AddValue("useCa", "Whether to use carrier aggregation.", useCa);
	cmd.AddValue("rng","The value of RNG",RngSize);
	cmd.AddValue("simTime"," Simulation Duration ",simTime);
	cmd.AddValue("interval","Interval time between two packets",packet_interval);
	cmd.AddValue("voice_packet_size"," Packet Size ",voice_packet_size);
	cmd.AddValue("voip_packet_size"," Packet Size ",voip_packet_size);
	cmd.Parse(argc, argv);
	cmd.Parse (argc, argv);
	RngSeedManager::SetRun (RngSize);
	if (useCa)
	{
		 Config::SetDefault ("ns3::LteHelper::UseCa", BooleanValue (useCa));
		 Config::SetDefault ("ns3::LteHelper::NumberOfComponentCarriers", UintegerValue (2));
		 Config::SetDefault ("ns3::LteHelper::EnbComponentCarrierManager", StringValue ("ns3::RrComponentCarrierManager"));
	}

	/*--------------- Various Variables Declaration-----------*/
	endTimeVoice = CreateObject<NormalRandomVariable> ();
	endTimeVideo = CreateObject<NormalRandomVariable> ();
	endTimeVoip = CreateObject<NormalRandomVariable> ();
	sTypes = CreateObject<UniformRandomVariable> ();
	sTypes->SetAttribute ("Min", DoubleValue (0.0));
	sTypes->SetAttribute ("Max", DoubleValue (2.99));

	ueIds = new ueInformation[nUEs_Array];
	CellMap = new int [nUEs_Array];
	Initialise_Arrays();

	// TODO Add comments on this section
	createBuilding(300,401,300,351,0,10,4,2,1,build,mbi);
	createMobility(mobility,mm0,neNbs,nHeNbs,nUEs,nWiFi,eNbsnodes,HeNbsnodes,UEsnodes,WiFinodes,remoteHostContainer,300,401,300,351);
	printTopology(mob,pos,neNbs,nHeNbs,nWiFi,eNbsnodes, HeNbsnodes,WiFinodes);

	/*------------------------------Declaration of Simulation's LTE-Attributes-------------------------*/
	lteHelper = CreateObject<LteHelper>();
	epcHelper= CreateObject<EpcHelper>();
	lteHelper->SetSchedulerType("ns3::RrFfMacScheduler");
	//lteHelper->SetSchedulerAttribute("HarqEnabled",BooleanValue(true));
	//lteHelper->SetSchedulerAttribute("CqiTimerThreshold",UintegerValue(1000));
	lteHelper->SetEpcHelper(epcHelper);
	Ptr<Node> pgw = epcHelper->GetPgwNode ();

	enbLteDevs = accessNetworkSetUp(NodeContainer(eNbsnodes.Get(0)), 23.0, arg, 2.5, 5, 8, 100.0, 18000.0, eNbDLBandwidth, eNbULBandwidth);
	for (int i=1;i<nHeNbs;i++)
	{
		enbLteDevs.Add(accessNetworkSetUp(NodeContainer(eNbsnodes.Get(i)), -96.0, arg, 2.5, 5, 8, 200.0, 18000.0, eNbDLBandwidth, eNbULBandwidth));
	}




	ueIpIface = epcHelper->AssignUeIpv4Address(NetDeviceContainer(ueLteDevs));

	// TODO Add Application setup before start simulation
	for (uint32_t i=0; i<nUEs; i++)
	{
		TrafficTimeline (sTypes, i, simTime);
		std::cout << "Finished settin up traffic for UE\t" << (i+1) << "\n" << std::endl;
	}
	//double rand_val;
	//rand_val = double(getRandom());
	//cout << "Random value is " << getRandom(5,2) << endl;
	Simulator::Stop (Seconds (2.00));

	/*config.ConfigureAttributes ();*/
	Simulator::Run ();

	Simulator::Destroy ();
	return 0;

}
