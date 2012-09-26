/**
 * @file   main.cpp
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Tue Jun 21 08:53:03 2011
 * 
 * @brief  Initialization of SCADS instance: sets up namespace, and cleans up. 
 * 
 * 
 */

#include <stdio.h>
#include "scads.h"

using namespace SCADS_NS;

/**
 * Main Routine creates a new scads instance.
 * 
 * Instead of putting the main computational routine here, it's been moved to
 * scads.h/scads.cpp. This way, the initilization and cleanup is straightforward,
 * e.g. SCADS *scads = new SCADS(argc, argv) 
 * e.g. delete scads
 */

int main(int argc, char **argv) {

	// Create a SCADS instance, pass arguments;
	SCADS *scads = new SCADS(argc, argv);

	// Delete the SCADS instance and clean up
	delete scads;

	return 0;
}
