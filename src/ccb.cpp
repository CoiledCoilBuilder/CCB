// -*-c++-*-

// *hd +------------------------------------------------------------------------------------+
// *hd |  This file is part of Coiled-Coil Builder.                                         |
// *hd |                                                                                    |
// *hd |  Coiled-Coil Builder is free software: you can redistribute it and/or modify       |
// *hd |  it under the terms of the GNU General Public License as published by              |
// *hd |  the Free Software Foundation, either version 3 of the License, or                 |
// *hd |  (at your option) any later version.                                               |
// *hd |                                                                                    |
// *hd |  Coiled-Coil Builder is distributed in the hope that it will be useful,            |
// *hd |  but WITHOUT ANY WARRANTY without even the implied warranty of                     |
// *hd |  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                     |
// *hd |  GNU General Public License for more details.                                      |
// *hd |                                                                                    |
// *hd |  You should have received a copy of the GNU General Public License                 |
// *hd |  along with Coiled-Coil Builder.  If not, see <http:www.gnu.org/licenses/>.        |
// *hd +------------------------------------------------------------------------------------+

// *hd | If you intend to use this software for your research, please cite:
// *hd | and inform Chris MacDermaid <chris.macdermaid@gmail.com> of any pending publications.

// *hd | Copyright (c) 2012,2013,2014 by Chris M. MacDermaid <chris.macdermaid@gmail.com>
// *hd | and Jeffery G. Saven <saven@sas.upenn.edu>


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

