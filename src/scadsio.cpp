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
#include "output.h"
#include "style_output.h"
#include "memory.h"
#include "error.h"
#include "universe.h"
#include "domain.h"

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

Scadsio::Scadsio(SCADS *scads) :
		Pointers(scads) {

	// Initialize some variables
	noutput = maxoutput = 0;
	output = NULL;
}

/** 
 *  Scadsio Deconstructor
 *
 *  Cleans up and remaining inputs or outputs.
 */

Scadsio::~Scadsio() {

	// Delete Outputs
	while (noutput)
		delete_output(output[0]->id);
	memory->sfree(output);
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


void Scadsio::memory_usage() {

	bigint bytes = 0;

	bytes += domain->memory_usage();

	double mbytes = bytes / 1024.0 / 1024.0;

	if (universe->me == 0)
		fprintf(screen, "Domain memory usage per processor = %g Mbytes\n", mbytes);
}
