/**
 * @file   output.cpp
 * @author CVB <cvb@magrathea.chem.upenn.edu>
 * @date   Thu Jun 23 16:24:13 2011
 * 
 * @brief  Routines for the Output Class and Styles
 * 
 * The style implementation follows from that of LAMMPS.
 */

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "ccbio.h"
#include "memory.h"
#include "error.h"
#include "output.h"
#include "universe.h"

using namespace CCB_NS;

/** 
 * The Outtput Constructor, sets up the
 *
 * @param ccb The ccb class pointer
 * @param narg number of arguments passed to constructor
 * @param arg the arguments passed to the constructor
 *
 * @return no return value
 */

Output::Output(CCB *ccb, int /*narg*/, const char **arg) :
		Pointers(ccb) {

	//Set the name of the input style
	int n = strlen(arg[1]) + 1;
	style = new char[n];
	strcpy(style, arg[1]);

	//Set the input ID
	n = strlen(arg[2]) + 1;
	id = new char[n];
	strcpy(id, arg[2]);

	// Check to see that the ID is legit
	for (int i = 0; i < n - 1; i++)
		if (!isalnum(id[i]) && id[i] != '_') error->one(FLERR, 
				"Output style ID must be alphanumeric or underscore characters");

	/// Set the filename
	n = strlen(arg[3]) + 1;
	filename = new char[n];
	strcpy(filename, arg[3]);

	fp = NULL;

}

/** 
 * The Output deconstructor
 *
 * Delete id and style, close the file
 */

Output::~Output() {

	delete[] id;
	delete[] style;
	delete[] filename;

}

void Output::init() {
	init_style();
}

void Output::write() {
	write_style();
}

void Output::openfile() {
	fp = fopen(filename, "w");
	if (fp == NULL && universe->me == 0) 
          error->one(FLERR, "Can't open output file");
}

void Output::closefile() {
	if (universe->me == 0 && fp != NULL) fclose(fp);
}
