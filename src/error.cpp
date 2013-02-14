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

#include "stdlib.h"
#include "error.h"
#include "universe.h"
#include "memory.h"

using namespace CCB_NS;

Error::Error(CCB *ccb) :
        Pointers(ccb) {

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


int Error::all(const char *file, int line, const char *str) {

    if (universe->me == 0) {
        if (screen) {
            fprintf(screen, "ERROR: %s (%s:%d)\n", str, file, line);
            fflush(screen);
        }
    }

    return CCB_ERROR;

}

/**
 * error->one: called by one proc in universe
 *
 * @param str error message
 */

int Error::one(const char *file, int line, const char *str) {

    if (screen) {
        fprintf(screen, "ERROR on proc %d: %s (%s:%d)\n", universe->me, str, file, line);
        fflush(screen);
    }

    return CCB_ERROR;

}

/**
 *  error->warning: called by one proc in universe
 *
 *  only write to screen if non-NULL on this proc since could be file
 *
 * @param str Error Message
 * @param logflag 0=no log
 */

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

void Error::message_verb(char *str, int verbosity) {

    if (screen && verbosity_level >= verbosity)
        fprintf(screen, "%s\n", str);
}
