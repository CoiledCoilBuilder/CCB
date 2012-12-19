/**
 * @file   ccb.cpp
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Tue Jun 21 12:04:55 2011
 * 
 * @brief  Main routine file, contains the top level CCB Class.
 * 
 * This is where the meat of the program is.
 */

#include "math.h"
#include "string.h"
#include "ccb.h"
#include "stdlib.h"
#include "memory.h"
#include "universe.h"
#include "error.h"
#include "ccbio.h"
#include "output.h"
#include "bitmask.h"
#include "domain.h"
#include "backbonehandler.h"

#define BLEN 200

using namespace CCB_NS;

/** 
 *  Start an instance for CCB and allocate fundamental classes memory, error, ccbio and universe,
 *  
 *
 * @param narg number of arguments passed
 * @param arg argument array
 */

CCB::CCB(int , char **) {

	/// Initialize fundamental classes
	memory = new Memory(this);
	error = new Error(this);
	universe = new Universe(this);
	ccbio = new Ccbio(this);

	/// Set screen to stdout
	screen = stdout;

	/// Create and initialize the top level classes
	create();
	init();

     // Do a test
     //test();

}

/**
 * CCB top-level destructor
 * Kill the top-level classes, and then the fundamental classes
 */

CCB::~CCB() {

	// Kill top level classes
	destroy();

	// Delete fundamental classes
	delete universe;
	delete error;
	delete memory;
	delete ccbio;

     if (error->verbosity_level == 10) {
          ccbio->memory_usage();
          fprintf(screen, "-----Normal Termination of CCB-----\n");
     }
}

/**
 * Create the top level classes
 */

void CCB::create() {

	domain = new Domain(this);
	backbone = new BackboneHandler(this);
	bitmask = new Bitmask(this);
}

/**
 * initialize the top level classes 
 */
void CCB::init() {

}

/**
 * destroy the top level classes
 */
void CCB::destroy() {

	delete bitmask;
	delete backbone;
	delete domain;

}

void CCB::test() {

    const char **newarg = new const char*[5];

    error->verbosity_level=10;

    /// Add the coiled-coil plugin
    newarg[0] = (char *) "backbone";
    newarg[1] = (char *) "add";
    newarg[2] = (char *) "coiledcoil";
    newarg[3] = (char *) "bbcc1";
    newarg[4] = (char *) "-asymmetric";
    backbone->add_backbone(5,newarg);
    backbone->init_backbone(newarg[3]);
    backbone->generate_backbone(newarg[3]);

    /// Write out the coordinates to a pdb file
    newarg[0] = (char *) "output";
    newarg[1] = (char *) "pdb";
    newarg[2] = (char *) "pdb1";
    newarg[3] = (char *) "test.pdb";
    newarg[4] = (char *) "all";
    ccbio->add_output(5, newarg);
    ccbio->init_output(newarg[2]);
    ccbio->write_output(newarg[2]);

    newarg[3] = (char *) "bbcc1";
    backbone->delete_backbone(newarg[3]);
}

