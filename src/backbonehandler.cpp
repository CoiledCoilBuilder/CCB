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
 * @file   backbonehandler.cpp
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Fri Jan 20 16:18:46 2012
 * 
 * @brief  Routines for creating / deleting and tracking backbone
 * modeling styles.
 *
 * 
 */

#include <stdio.h>
#include <string.h>
#include "error.h"
#include "memory.h"
#include "backbonehandler.h"
#include "style_backbone.h"
#include "backbone.h"

using namespace CCB_NS;

/**
 * @def BACKBONE_DELTA
 * @brief Number of backbone modeling classes to create
 * when we need to reallocate.  This should not be very 
 * large, maybe 1 or 2. 
 *
 * 
 */

#define BACKBONE_DELTA 2

BackboneHandler::BackboneHandler(CCB *ccb) :
		Pointers(ccb),nbackbone(0),maxbackbone(0) {

     backbone = NULL;

}

BackboneHandler::~BackboneHandler() {

     while (nbackbone)
          delete_backbone(backbone[0]->id);

     memory->sfree(backbone);
}

/**
 *  BACKBONE CREATION / DELETION ROUTINES
 * 
 */

/** 
 * @brief Add a backbone style. 
 *
 * Keep track of this style
 * so that it can remain open or terminated at
 * any time.
 *
 * @param narg number of arguments passed 
 * @param arg the arguments passed
 */

int BackboneHandler::add_backbone(int narg, const char **arg) {

	if (narg < 3) 
		return error->one(FLERR, "Illegal backbone command");

	// Check to see if the backbone is already defined.
	int ibackbone = 0;
	for (ibackbone = 0; ibackbone < nbackbone; ibackbone++)
		if (strcmp(arg[2], backbone[ibackbone]->id) == 0)
			break;

	if (ibackbone < nbackbone)
		return error->one(FLERR, "The backbone style already exists");

	// realloc more space if we don't have enough to create a new backbone
     // modeler class
        
	if (nbackbone == maxbackbone) {
		maxbackbone += BACKBONE_DELTA;
		backbone = (Backbone **) memory->srealloc(backbone, maxbackbone * sizeof(Backbone *), "backbonehandler::backbone");
	}

     if (backbone == NULL)
          return CCB_ERROR;

	//create the backbone modeler 

	if (0)
		return CCB_OK;

#define BACKBONE_CLASS
#define BackboneStyle(key,Class)                                        \
     else if (strcmp(arg[2],#key) == 0) backbone[nbackbone] = new Class(ccb,narg,arg);
#include "style_backbone.h"
#undef BACKBONE_CLASS

	else
		return error->one(FLERR, "Invalid backbone style");

	//Update the total number of active backbone modelers.
	nbackbone++;

     return CCB_OK;
}

/** 
 * @brief delete a backbone style
 *
 * Deletes a backbone style with corresponding style id
 *
 * @param id backbone style ID
 */

int BackboneHandler::delete_backbone(const char *id) {

	// Find the backbone index
	int ibackbone = find_backbone(id);

	// Delete the backbone
	if (ibackbone < 0)
		return error->one(FLERR, "Could not find backbone style type ID to delete");
	delete backbone[ibackbone];

	for (int i = ibackbone + 1; i < nbackbone; i++)
		backbone[i - 1] = backbone[i];

	nbackbone--;

     return CCB_OK;
}

/** 
 * @brief find index of backbone style in backbone array
 *
 * @param id 
 */

int BackboneHandler::find_backbone(const char *id) {
	int ibackbone = 0;

	for (ibackbone = 0; ibackbone < nbackbone; ibackbone++)
		if (strcmp(id, backbone[ibackbone]->id) == 0)
			break;
       
        if (ibackbone == nbackbone)
		return -1;

        return ibackbone;  
}

int BackboneHandler::init_backbone(const char *id) {

	int ibackbone = find_backbone(id);
	if (ibackbone < 0)
		return error->one(FLERR, "Could not find backbone type ID to initialize");

	return backbone[ibackbone]->init();
}

int BackboneHandler::update_backbone(const char *id, int argc, const char **argv, int n) {

     int ibackbone = find_backbone(id);
     if (ibackbone < 0) {
          char str[128];
          sprintf(str, "Could not find backbone id %s to update", id);
          return error->one(FLERR, str);
     }

     return backbone[ibackbone]->update(argc, argv, n);
}

int BackboneHandler::generate_backbone(const char *id) {

     int ibackbone = find_backbone(id);
     if (ibackbone < 0) {
          char str[128];
          sprintf(str, "Could not find backbone id %s to generate", id);
          return error->one(FLERR, str);
     }

     return backbone[ibackbone]->generate();
}
