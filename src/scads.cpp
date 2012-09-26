/**
 * @file   scads.cpp
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Tue Jun 21 12:04:55 2011
 * 
 * @brief  Main routine file, contains the top level SCADS Class.
 * 
 * This is where the meat of the program is.
 */

#include "math.h"
#include "string.h"
#include "scads.h"
#include "stdlib.h"
#include "memory.h"
#include "universe.h"
#include "error.h"
#include "scadsio.h"
#include "output.h"
#include "bitmask.h"
#include "domain.h"
#include "backbonehandler.h"

#define BLEN 200

using namespace SCADS_NS;

/** 
 *  Start an instance for SCADS and allocate fundamental classes memory, error, scadsio and universe,
 *  
 *
 * @param narg number of arguments passed
 * @param arg argument array
 */

SCADS::SCADS(int narg, char **arg) {

	/// Initialize fundamental classes
	memory = new Memory(this);
	error = new Error(this);
	universe = new Universe(this);
	scadsio = new Scadsio(this);

	/// Set screen to stdout
	screen = stdout;

	/// Create and initialize the top level classes
	create();
	init();

	/// Parse command line
	/// cmdline(narg, arg);

     run();
}

/**
 * SCADS top-level destructor
 * Kill the top-level classes, and then the fundamental classes
 */

SCADS::~SCADS() {

	// Kill top level classes
	destroy();

	// Delete fundamental classes
	delete universe;
	delete error;
	delete memory;
	delete scadsio;

	fprintf(screen, "-----Normal Termination of COMPASS-----\n");
}

/**
 * Create the top level classes
 */

void SCADS::create() {

	domain = new Domain(this);
	backbone = new BackboneHandler(this);
	bitmask = new Bitmask(this);
}

/**
 * initialize the top level classes 
 */
void SCADS::init() {

}

/**
 * destroy the top level classes
 */
void SCADS::destroy() {

	delete bitmask;
	delete backbone;
	delete domain;

}

/**
 * Run the calculation after system setup
 * 
 */
void SCADS::run() {

     const char **newarg = new const char*[5];

     /// Add the coiled-coil plugin 
     newarg[0] = (char *) "backbone";
     newarg[1] = (char *) "add";
     newarg[2] = (char *) "coiledcoil";
     newarg[3] = (char *) "bbcc1";
     backbone->add_backbone(4,newarg);
     backbone->init_backbone(newarg[3]);
     backbone->update_backbone(newarg[3],4,newarg,4);
     backbone->delete_backbone(newarg[3]);

     /// Add an output
     newarg[0] = (char *) "output";
     newarg[1] = (char *) "pdb";
     newarg[2] = (char *) "pdb1";
     newarg[3] = (char *) "test.pdb";
     newarg[4] = (char *) "all";
     scadsio->add_output(5, newarg);
     scadsio->init_output(newarg[2]);
     scadsio->write_output(newarg[2]);
     scadsio->delete_output(newarg[2]);

	// Memory usage pre opt
	scadsio->memory_usage();
}

/**
 * Parse the command line routines
 *
 * @param argc
 * @param argv
 */

void SCADS::cmdline(int argc, char **argv) {
	
        //char *line;
	ctlfile = NULL;

	//line = new char[BLEN];
	int i = strlen(argv[argc - 1]) + 1;
	ctlfile = new char[i];

	if (argc < 2)
		error->all(FLERR, "You didn't specify a control file");

	int iarg = 0;
	while (iarg < argc) {
		/// The last argument issued should be the input file
		/// Do some cmdline parsing if necessary
		iarg++;
	}

	strcpy(ctlfile, argv[argc - 1]);
}
