/**
 * @file   ccbio.cpp
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Thu Jun 23 17:19:33 2011
 * 
 * @brief  Routines for creating / deleting and tracking input and output styles
 * 
 * 
 */

#include "stdio.h"
#include "string.h"
#include "ccbio.h"
#include "output.h"
#include "style_output.h"
#include "memory.h"
#include "error.h"
#include "universe.h"
#include "domain.h"

using namespace CCB_NS;

/**
 * @def DELTA_INOUT
 * @brief Number of finite inputs and outpts to create 
 * 
 * We need to initially allocate space for these, so we
 * set how many to allocate at once and each time we
 * need to reallocate
 */

#define DELTA_INOUT 4

Ccbio::Ccbio(CCB *ccb) :
		Pointers(ccb) {

	// Initialize some variables
	noutput = maxoutput = 0;
	output = NULL;
}

/** 
 *  Ccbio Deconstructor
 *
 *  Cleans up and remaining inputs or outputs.
 */

Ccbio::~Ccbio() {

	// Delete Outputs
	while (noutput)
		delete_output(output[0]->id);
	memory->sfree(output);
}

  // +-----------------------------------------+ 
  // |  *  OUTPUT CREATION / DELETION ROUTINES | 
  // +-----------------------------------------+ 


int Ccbio::add_output(int narg, const char **arg) {

	if (narg < 3)
		return error->one(FLERR,"Illegal output command");

	// Check to see if the output is already defined.
	int ioutput;
	for (ioutput = 0; ioutput < noutput; ioutput++)
		if (strcmp(arg[2], output[ioutput]->id) == 0)
			break;

	if (ioutput < noutput)
		return error->one(FLERR,"The output already exists");

	// realloc more space if we don't have enough to create a new output
	if (noutput == maxoutput) {
		maxoutput += DELTA_INOUT;
		output = (Output **) memory->srealloc(output, maxoutput * sizeof(Output *), "ccbio:output");
	}

     if (output == NULL)
          return CCB_ERROR;

	//create the output

	if (0)
		return CCB_OK;

#define OUTPUT_CLASS
#define OutputStyle(key,Class)\
    else if (strcmp(arg[1],#key) == 0) output[noutput] = new Class(ccb,narg,arg);
#include "style_output.h"
#undef OUTPUT_CLASS

	else
		return error->one(FLERR,"Invalid output style");

	//Update the total number of active outputs.
	noutput++;

     return CCB_OK;

}

int Ccbio::delete_output(const char *id) {

	// Find the input index
	int ioutput = find_output(id);

	//fprintf(screen, "output-ID: %d output Name: %s num outputs:%d\n", ioutput,output[ioutput]->id,noutput);

	// Delete the output
	if (ioutput < 0)
		return error->one(FLERR,"Could not find output type ID to delete");
	delete output[ioutput];

	for (int i = ioutput + 1; i < noutput; i++)
		output[i - 1] = output[i];

	noutput--;

     return CCB_OK;
}

int Ccbio::find_output(const char *id) {
	int ioutput;
	for (ioutput = 0; ioutput < noutput; ioutput++)
		if (strcmp(id, output[ioutput]->id) == 0)
			break;
	if (ioutput == noutput)
		return -1;
	return ioutput;
}

int Ccbio::init_output(const char *id) {
	int ioutput = find_output(id);
	if (ioutput < 0)
		return error->one(FLERR,"Could not find output type ID to initialize");

	output[ioutput]->init();

     return CCB_OK;

}

int Ccbio::write_output(const char *id) {
	int ioutput = find_output(id);
	if (ioutput < 0)
		return error->one(FLERR,"Could not find output type ID to write");

	output[ioutput]->write();

     return CCB_OK;

}

void Ccbio::active_outputs() {
	for (int i = 0; i < noutput; i++)
		fprintf(screen, "%s\n", output[i]->id);
}


void Ccbio::memory_usage() {

	bigint bytes = 0;

	bytes += domain->memory_usage();

	double mbytes = bytes / 1024.0 / 1024.0;

	if (universe->me == 0)
		fprintf(screen, "Domain memory usage per processor = %g Mbytes\n", mbytes);
}
