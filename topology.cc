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
   * 5 eNB, and 10 HeNBs (femtocells) and 20 WiFi access pointg
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

using namespace ns3;
using namespace std;

int main (int argc, char *argv[])
{
	double useCa;

	CommandLine cmd;
	cmd.AddValue("useCa", "test string", useCa);
	cmd.Parse (argc, argv);

	/*Create the Building Infrastructure*/
	Ptr<Building> build = CreateObject<Building> ();
	Ptr<MobilityBuildingInfo> mbi;
	cout <<" ******* Creating Building ******* "<< endl;
	build->SetBoundaries (Box (0,101,0,51,0,10));
	build->SetBuildingType (Building::Commercial);
	build->SetExtWallsType(Building::ConcreteWithWindows);
	build->SetNFloors(1);
	build->SetNRoomsX (4);
	build->SetNRoomsY (2);
	mbi = CreateObject<MobilityBuildingInfo> ();
	cout<< "The boundaries of the building is: " << build->GetBoundaries() << " ,number of floors is: " << build->GetNFloors() << ", the number of rooms in X is : " << build->GetNRoomsX() << " and in Y is : " << build->GetNRoomsY() << endl;

	Simulator::Stop (Seconds (2.00));

	Simulator::Run ();

	Simulator::Destroy ();
	return 0;

}
