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
