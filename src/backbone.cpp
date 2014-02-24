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
 * @file   backbone.cpp
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Fri Jan 20 16:52:22 2012
 * 
 * @brief  Routines for the backbone modeling base class 
 * 
 */

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "ccbio.h"
#include "memory.h"
#include "error.h"
#include "universe.h"
#include "backbone.h"

using namespace CCB_NS;

Backbone::Backbone(CCB *ccb, int /*narg*/, const char **arg) :
		Pointers(ccb) {

     /**
      * argv[0] = backbone
      * argv[1] = add/delete/update
      * argv[2] = style
      * argv[3] = id
      */


	//Set the name of the backbone style
	int n = strlen(arg[2]) + 1;
	style = new char[n];
	strcpy(style, arg[2]);

	//Set the backbone ID
	n = strlen(arg[3]) + 1;
	id = new char[n];
	strcpy(id, arg[3]);

	// Check to see that the ID is legit
	for (int i = 0; i < n - 1; i++)
		if (!isalnum(id[i]) && id[i] != '_')
			error->all(FLERR, "backbone style ID must be alphanumeric or underscore characters");
}

Backbone::~Backbone() {
	delete[] id;
	delete[] style;
}

int Backbone::init() {
	return init_style();
}

int Backbone::update(int argc, const char **argv, int n) {
     return update_style(argc, argv, n);
}

int Backbone::generate() {
     return generate_style();
}
