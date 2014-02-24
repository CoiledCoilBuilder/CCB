

/**
 * @file   main.cpp
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Tue Jun 21 08:53:03 2011
 * 
 * @brief  Initialization of CCB instance: sets up namespace, and cleans up. 
 * 
 * 
 */

#include <stdio.h>
#include "ccb.h"

using namespace CCB_NS;

/**
 * Main Routine creates a new ccb instance.
 * 
 * Instead of putting the main computational routine here, it's been moved to
 * ccb.h/ccb.cpp. This way, the initilization and cleanup is straightforward,
 * e.g. CCB *ccb = new CCB(argc, argv) 
 * e.g. delete ccb
 */

int main(int argc, char **argv) {

	// Create a CCB instance, pass arguments;
	CCB *ccb = new CCB(argc, argv);

	// Delete the CCB instance and clean up
	delete ccb;

	return 0;
}
