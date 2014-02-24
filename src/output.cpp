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

int Output::init() {
	return init_style();
}

int Output::write() {
	return write_style();
}

int Output::openfile() {
	fp = fopen(filename, "w");
	if (fp == NULL && universe->me == 0) 
          return error->one(FLERR, "Can't open output file");

     return CCB_OK;
}

int Output::closefile() {
	if (universe->me == 0 && fp != NULL) fclose(fp);
     return CCB_OK;
}
