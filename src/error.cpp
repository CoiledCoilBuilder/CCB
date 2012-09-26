/**
 * @file   error.cpp
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Tue Jun 21 15:18:56 2011
 * 
 * @brief  Error Routines.
 *
 * char str[BLEN];
 * sprintf(str, "Error message:%s", variable);
 * error->one(FLERR, str);
 * 
 */

/* ----------------------------------------------------------------------
 LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
 http://lammps.sandia.gov, Sandia National Laboratories
 Steve Plimpton, sjplimp@sandia.gov

 Copyright (2003) Sandia Corporation.  Under the terms of Contract
 DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
 certain rights in this software.  This software is distributed under
 the GNU General Public License.

 See the README file in the top-level LAMMPS directory.
 ------------------------------------------------------------------------- */

#include "mpi.h"
#include "stdlib.h"
#include "error.h"
#include "universe.h"
#include "memory.h"

using namespace SCADS_NS;

Error::Error(SCADS *scads) :
		Pointers(scads) {

	// Set the default verbosity level
	verbosity_level = 1;
}

/** 
 * error->all called by all procs in universe
 *
 * close all output, screen, and log files in world and universe
 *
 * @param str Error Message i
 */

void Error::all(const char *str) {
	MPI_Barrier(universe->world);

	if (universe->me == 0) {
		if (screen) {
			fprintf(screen, "ERROR: %s\n", str);
			fflush(screen);
		}
	}

	//if (output) delete output;

	MPI_Finalize();
	exit(1);
}

// New error function, overloaded  
void Error::all(const char *file, int line, const char *str) {
	MPI_Barrier(universe->world);

	if (universe->me == 0) {
		if (screen) {
			fprintf(screen, "ERROR: %s (%s:%d)\n", str, file, line);
			fflush(screen);
		}
	}

	//if (output) delete output;

	MPI_Finalize();
	exit(1);
}

/** 
 * error->one: called by one proc in universe
 *
 * @param str error message 
 */

void Error::one(const char *str) {
	if (screen) {
		fprintf(screen, "ERROR on proc %d: %s\n", universe->me, str);
		fflush(screen);
	}

	MPI_Abort(universe->world, 1);
}

void Error::one(const char *file, int line, const char *str) {
	if (screen) {
		fprintf(screen, "ERROR on proc %d: %s (%s:%d)\n", universe->me, str, file, line);
		fflush(screen);
	}

	MPI_Abort(universe->world, 1);
}

/** 
 *  error->warning: called by one proc in universe
 *
 *  only write to screen if non-NULL on this proc since could be file 
 *
 * @param str Error Message 
 * @param logflag 0=no log
 */

void Error::warning(const char *str, int) {
	if (screen)
		fprintf(screen, "WARNING at proc %d: %s\n", universe->me, str);
}

void Error::warning(const char *file, int line, const char *str) {
	if (screen)
		fprintf(screen, "WARNING: %s (%s:%d)\n", str, file, line);
}

/** 
 * error->message called by one proc in universe, typically proc 0
 *
 * write message to screen and logfile (if logflag is set)
 *
 * @param str 
 * @param logflag 
 */

void Error::message(char *str) {
	if (screen)
		fprintf(screen, "%s\n", str);
}

void Error::message(const char *file, int line, const char *str) {
	if (screen)
		fprintf(screen, "%s (%s:%d)\n", str, file, line);
}

/** 
 * error->message(char *str, int verbosity) 
 * called by one proc in universe, typically proc 0
 *
 * write message to screen if verbosity level is 
 * greater than the specified verbosity.
 *
 * @param str 
 * @param logflag 
 */

void Error::message(char *str, int verbosity) {

	if (screen && verbosity_level >= verbosity)
		fprintf(screen, "%s\n", str);
}
