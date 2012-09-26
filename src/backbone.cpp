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
#include "scadsio.h"
#include "memory.h"
#include "error.h"
#include "mpi.h"
#include "universe.h"
#include "backbone.h"

using namespace SCADS_NS;

Backbone::Backbone(SCADS *scads, int /*narg*/, const char **arg) :
		Pointers(scads) {

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

void Backbone::init() {
	init_style();
}

void Backbone::update(int argc, const char **argv, int n) {
     update_style(argc, argv, n);
}
