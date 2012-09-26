/**
 * @file   main.cpp
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Tue Jun 21 08:53:03 2011
 * 
 * @brief  Initialization of SCADS instance: sets up namespace, initializes MPI, and cleans up. 
 * 
 * 
 */

#include <stdio.h>
#include "mpi.h"
#include "scads.h"

using namespace SCADS_NS;

/**
 * Main Routine creates a new scads instance.
 * 
 * Instead of putting the main computational routine here, it's been moved to
 * scads.h/scads.cpp. This way, the initilization and cleanup is straightforward,
 * and we can pass everything from MPI_Init to the new scads instance.
 */

int main(int argc, char **argv) {

	// Initialize MPI
	MPI_Init(&argc, &argv);

	// Create a SCADS instance, pass arguments;
	SCADS *scads = new SCADS(argc, argv, MPI_COMM_WORLD);

	// Delete the SCADS instance and clean up
	delete scads;

	// Finalize MPI
	MPI_Finalize();

	return 0;
}
