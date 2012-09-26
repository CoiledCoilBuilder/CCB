/**
 * @file   scadsio.cpp
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Thu Jun 23 17:19:33 2011
 * 
 * @brief  Routines for creating / deleting and tracking input and output styles
 * 
 * 
 */

#include "stdio.h"
#include "string.h"
#include "scadsio.h"
#include "opt.h"
#include "input.h"
#include "style_input.h"
#include "output.h"
#include "style_output.h"
#include "rotamer_library.h"
#include "style_rotamer_library.h"
#include "compute.h"
#include "style_compute.h"
#include "memory.h"
#include "error.h"
#include "universe.h"
#include "domain.h"
#include "atomselecthandler.h"

using namespace SCADS_NS;

/**
 * @def DELTA_INOUT
 * @brief Number of finite inputs and outpts to create 
 * 
 * We need to initially allocate space for these, so we
 * set how many to allocate at once and each time we
 * need to reallocate
 */

#define DELTA_INOUT 4

/**
 * @def DELTA_ROTAMER_LIB
 * @brief Number of finite rotamer libraries to create 
 * 
 * We need to initially allocate space for these, so we
 * set how many to allocate at once and each time we
 * need to reallocate
 */

#define DELTA_ROTAMER_LIB 1

/**
 * @def DELTA_COMPUTE
 *
 * @brief number of computs to create on realloc
 *
 * We need to initially allocate space for these, so we
 * set how many to allocate at once and each time we
 * need to reallocate
 */

#define DELTA_COMPUTE 10

/** 
 * Scadsio Constructor
 *
 * Initializes variables for input/output styles 
 *
 * @param scads The scads pointer for this instance 
 */

Scadsio::Scadsio(SCADS *scads) :
		Pointers(scads) {

	// Initialize some variables
	ninput = maxinput = 0;
	input = NULL;

	noutput = maxoutput = 0;
	output = NULL;

	nrotamer_library = maxrotamer_library = 0;
	rotamer_library = NULL;

	ncompute = maxcompute = 0;
	compute = NULL;

}

/** 
 *  Scadsio Deconstructor
 *
 *  Cleans up and remaining inputs or outputs.
 */

Scadsio::~Scadsio() {

	// Delete Inputs
	while (ninput)
		delete_input(input[0]->id);
	memory->sfree(input);

	// Delete Outputs
	while (noutput)
		delete_output(output[0]->id);
	memory->sfree(output);

	//Delete Rotamer Libraries
	while (nrotamer_library)
		delete_rotamer_library(rotamer_library[0]->id);
	memory->sfree(rotamer_library);

	// Delete Compute
	while (ncompute)
		delete_compute(compute[0]->id);
	memory->sfree(compute);
}


  // +----------------------------------------+ 
  // |  *  INPUT CREATION / DELETION ROUTINES | 
  // +----------------------------------------+ 


/** 
 * Add an input style. 
 *
 * Keep track of this style
 * so that it can remain open or terminated at
 * any time.
 *
 * @param narg number of arguments passed 
 * @param arg the arguments passed
 */

void Scadsio::add_input(int narg, const char **arg) {

	if (narg < 4)
		error->one(FLERR, "Illegal input command");

	// Check to see if the input is already defined.
	int iinput;
	for (iinput = 0; iinput < ninput; iinput++)
		if (strcmp(arg[2], input[iinput]->id) == 0)
			break;

	if (iinput < ninput)
		error->one(FLERR, "The input already exists");

	// realloc more space if we don't have enough to create a new input
	if (ninput == maxinput) {
		maxinput += DELTA_INOUT;
		input = (Input **) memory->srealloc(input, maxinput * sizeof(Input *), "scadsio:input");
	}

	//create the input

	if (0)
		return;

#define INPUT_CLASS
#define InputStyle(key,Class)\
      else if (strcmp(arg[1],#key) == 0) input[ninput] = new Class(scads,narg,arg);
#include "style_input.h"
#undef INPUT_CLASS

	else
		error->one(FLERR,"Invalid input style");

	//Update the total number of active inputs.
	ninput++;
}

void Scadsio::delete_input(const char *id) {

	// Find the input index
	int iinput = find_input(id);

	// Delete the input
	if (iinput < 0)
		error->one(FLERR,"Could not find input type ID to delete");
	delete input[iinput];

	for (int i = iinput + 1; i < ninput; i++)
		input[i - 1] = input[i];

	ninput--;

}

int Scadsio::find_input(const char *id) {
	int iinput;

	for (iinput = 0; iinput < ninput; iinput++)
		if (strcmp(id, input[iinput]->id) == 0)
			break;
	if (iinput == ninput)
		return -1;
	return iinput;
}

void Scadsio::init_input(const char *id) {

	int iinput = find_input(id);
	if (iinput < 0)
		error->one(FLERR,"Could not find input type ID to initialize");

	input[iinput]->init();
}

void Scadsio::read_input(const char *id) {
	int iinput = find_input(id);
	if (iinput < 0)
		error->one(FLERR,"Could not find input type ID to read");

	input[iinput]->read();
}

void Scadsio::active_inputs() {
	for (int i = 0; i < ninput; i++)
		fprintf(screen, "%s\n", input[i]->id);
}


  // +-----------------------------------------+ 
  // |  *  OUTPUT CREATION / DELETION ROUTINES | 
  // +-----------------------------------------+ 


void Scadsio::add_output(int narg, const char **arg) {

	if (narg < 3)
		error->one(FLERR,"Illegal output command");

	// Check to see if the output is already defined.
	int ioutput;
	for (ioutput = 0; ioutput < noutput; ioutput++)
		if (strcmp(arg[2], output[ioutput]->id) == 0)
			break;

	if (ioutput < noutput)
		error->one(FLERR,"The output already exists");

	// realloc more space if we don't have enough to create a new output
	if (noutput == maxoutput) {
		maxoutput += DELTA_INOUT;
		output = (Output **) memory->srealloc(output, maxoutput * sizeof(Output *), "scadsio:output");
	}

	//create the output

	if (0)
		return;

#define OUTPUT_CLASS
#define OutputStyle(key,Class)\
    else if (strcmp(arg[1],#key) == 0) output[noutput] = new Class(scads,narg,arg);
#include "style_output.h"
#undef OUTPUT_CLASS

	else
		error->one(FLERR,"Invalid output style");

	//Update the total number of active outputs.
	noutput++;

}

void Scadsio::delete_output(const char *id) {

	// Find the input index
	int ioutput = find_output(id);

	//fprintf(screen, "output-ID: %d output Name: %s num outputs:%d\n", ioutput,output[ioutput]->id,noutput);

	// Delete the output
	if (ioutput < 0)
		error->one(FLERR,"Could not find output type ID to delete");
	delete output[ioutput];

	for (int i = ioutput + 1; i < noutput; i++)
		output[i - 1] = output[i];

	noutput--;

}

int Scadsio::find_output(const char *id) {
	int ioutput;
	for (ioutput = 0; ioutput < noutput; ioutput++)
		if (strcmp(id, output[ioutput]->id) == 0)
			break;
	if (ioutput == noutput)
		return -1;
	return ioutput;
}

void Scadsio::init_output(const char *id) {
	int ioutput = find_output(id);
	if (ioutput < 0)
		error->one(FLERR,"Could not find output type ID to initialize");

	output[ioutput]->init();
}

void Scadsio::write_output(const char *id) {
	int ioutput = find_output(id);
	if (ioutput < 0)
		error->one(FLERR,"Could not find output type ID to write");

	output[ioutput]->write();
}

void Scadsio::active_outputs() {
	for (int i = 0; i < noutput; i++)
		fprintf(screen, "%s\n", output[i]->id);
}


  // +----------------------------------------------+ 
  // | ROTAMER LIBRARY CREATION / DELETION ROUTINES | 
  // +----------------------------------------------+ 

void Scadsio::add_rotamer_library(int narg, const char **arg) {

	if (narg < 3)
		error->one(FLERR,"Illegal rotamer library command");

	// Check to see if the rotamer library is already defined.
	int irotamer_library;
	for (irotamer_library = 0; irotamer_library < nrotamer_library; irotamer_library++)
		if (strcmp(arg[3], rotamer_library[irotamer_library]->id) == 0)
			break;

	if (irotamer_library < nrotamer_library)
		error->one(FLERR,"The rotamer library already exists");

	// realloc more space if we don't have enough to create a new rotamer_library
	if (nrotamer_library == maxrotamer_library) {
		maxrotamer_library += DELTA_ROTAMER_LIB;
		rotamer_library = (Rotamer_library **) memory->srealloc(rotamer_library,
               maxrotamer_library * sizeof(Rotamer_library *), "scadsio:rotamer_library");
	}

	//create the rotamer library instance

	if (0)
		return;

#define ROTAMER_LIBRARY_CLASS
#define RotamerlibraryStyle(key,Class)\
     else if (strcmp(arg[2],#key) == 0) rotamer_library[nrotamer_library] = new Class(scads,narg,arg);
#include "style_rotamer_library.h"
#undef ROTAMER_LIBRARY_CLASS

	else
		error->one(FLERR,"Invalid rotamer library style");

	//Update the total number of active rotamer libraries.
	nrotamer_library++;
}

void Scadsio::delete_rotamer_library(const char *id) {

	// Find the input index
	int irotamer_library = find_rotamer_library(id);

	//fprintf(screen, "input-ID: %d input Name: %s num inputs:%d\n", iinput,input[iinput]->id,ninput);

	// Delete the rotamer library
	if (irotamer_library < 0)
		error->one(FLERR,"Could not find the rotamer library id to delete");
	delete rotamer_library[irotamer_library];

	for (int i = irotamer_library + 1; i < nrotamer_library; i++)
		rotamer_library[i - 1] = rotamer_library[i];

	nrotamer_library--;

}

int Scadsio::find_rotamer_library(const char *id) {
	int irotamer_library;
	for (irotamer_library = 0; irotamer_library < nrotamer_library; irotamer_library++)
		if (strcmp(id, rotamer_library[irotamer_library]->id) == 0)
			break;
	if (irotamer_library == nrotamer_library)
		return -1;
	return irotamer_library;
}

int Scadsio::find_rotamer_bin(const char *id) {

	int irotamer_library, ibin;

	for (irotamer_library = 0; irotamer_library < nrotamer_library; irotamer_library++) {
		ibin = rotamer_library[irotamer_library]->find_bin(id);

		if (ibin >= 0)
			break;
	}

	if (irotamer_library == nrotamer_library)
		return -1;

	return irotamer_library;
}

void Scadsio::init_rotamer_library(const char *id) {
	int irotamer_library = find_rotamer_library(id);
	if (irotamer_library < 0) {
          char str[128];
          sprintf(str,"Could not find rotamer library id %s to initialize", id);
		error->one(FLERR,str);
     }

	rotamer_library[irotamer_library]->init();
}

void Scadsio::read_rotamer_library(const char *id) {
	int irotamer_library = find_rotamer_library(id);
	if (irotamer_library < 0)
		error->one(FLERR,"Could not find rotamer library id to read");

	rotamer_library[irotamer_library]->read();
}

void Scadsio::update_rotamer_library(int argc, const char **argv, int n) {

     int irotamer_library = find_rotamer_library(argv[2]);

     if (irotamer_library < 0)
          error->one(FLERR, "Couldn't find rotamer library ID to update");

     rotamer_library[irotamer_library]->update(argc, argv, n);
}


void Scadsio::active_rotamer_libraries() {
	for (int i = 0; i < nrotamer_library; i++)
		fprintf(screen, "%s\n", rotamer_library[i]->id);
}

  // +---------------------------------------+ 
  // |  COMPUTE CREATION / DELETION ROUTINES | 
  // +---------------------------------------+ 


/**
 * Add an compute style.
 *
 * Keep track of this style
 * so that it can remain open or terminated at
 * any time.
 *
 * @param narg number of arguments passed
 * @param arg the arguments passed
 */

void Scadsio::add_compute(int narg, const char **arg) {

	if (narg < 3)
		error->one(FLERR,"Illegal compute command");

	// Check to see if the compute is already defined.
	int icompute;
	for (icompute = 0; icompute < ncompute; icompute++)
		if (strcmp(arg[2], compute[icompute]->id) == 0)
			break;

	if (icompute < ncompute)
		error->one(FLERR,"The compute already exists");

	// realloc more space if we don't have enough to create a new compute
	if (ncompute == maxcompute) {
		maxcompute += DELTA_COMPUTE;
		compute = (Compute **) memory->srealloc(compute, maxcompute * sizeof(Compute *), "scadsio:compute");
	}

	//create the compute

	if (0)
		return;

#define COMPUTE_CLASS
#define ComputeStyle(key,Class)\
      else if (strcmp(arg[1],#key) == 0) compute[ncompute] = new Class(scads,narg,arg);
#include "style_compute.h"
#undef COMPUTE_CLASS

	else
		error->one(FLERR,"Invalid compute style");

	//Update the total number of active compute.
	ncompute++;
}

void Scadsio::delete_compute(const char *id) {

	// Find the compute index
	int icompute = find_compute(id);

	// Delete the compute
	if (icompute < 0)
		error->one(FLERR,"Could not find compute type ID to delete");
	delete compute[icompute];

	for (int i = icompute + 1; i < ncompute; i++)
		compute[i - 1] = compute[i];

	ncompute--;

}

int Scadsio::find_compute(const char *id) {
	int icompute;

	for (icompute = 0; icompute < ncompute; icompute++)
		if (strcmp(id, compute[icompute]->id) == 0)
			break;
	if (icompute == ncompute)
		return -1;
	return icompute;
}

void Scadsio::init_compute(const char *id) {

	int icompute = find_compute(id);
	if (icompute < 0)
		error->one(FLERR,"Could not find compute type ID to initialize");

	compute[icompute]->init();
}

void Scadsio::active_computes() {
	for (int i = 0; i < ncompute; i++)
		fprintf(screen, "%s\n", compute[i]->id);
}

void Scadsio::memory_usage() {

	bigint bytes = 0;

	bytes += domain->memory_usage();
	bytes += atomselect->memory_usage();

	double mbytes = bytes / 1024.0 / 1024.0;

	if (universe->me == 0)
		fprintf(screen, "Domain/Atomselect memory usage per processor = %g Mbytes\n", mbytes);
}
