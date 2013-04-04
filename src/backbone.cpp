// -*-c++-*-

  // +------------------------------------------------------------------------------------+ 
  // |  This file is part of Coiled-Coil Builder.                                         | 
  // |                                                                                    | 
  // |  Coiled-Coil Builder is free software: you can redistribute it and/or modify       | 
  // |  it under the terms of the GNU General Public License as published by              | 
  // |  the Free Software Foundation, either version 3 of the License, or                 | 
  // |  (at your option) any later version.                                               | 
  // |                                                                                    | 
  // |  Coiled-Coil Builder is distributed in the hope that it will be useful,            | 
  // |  but WITHOUT ANY WARRANTY without even the implied warranty of                     | 
  // |  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                     | 
  // |  GNU General Public License for more details.                                      | 
  // |                                                                                    | 
  // |  You should have received a copy of the GNU General Public License                 | 
  // |  along with Coiled-Coil Builder.  If not, see <http:www.gnu.org/licenses/>.        | 
  // |                                                                                    | 
  // |   *cr                                                                              | 
  // |   *cr            (C) Copyright 1995-2013 The Board of Trustees of the              | 
  // |   *cr                        University of Pennsylvania                            | 
  // |   *cr                         All Rights Reserved                                  | 
  // |   *cr                                                                              | 
  // +------------------------------------------------------------------------------------+ 

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
