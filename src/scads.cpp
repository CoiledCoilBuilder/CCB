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

     if (error->verbosity_level == 10) {
          scadsio->memory_usage();
          fprintf(screen, "-----Normal Termination of CCB-----\n");
     }
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
