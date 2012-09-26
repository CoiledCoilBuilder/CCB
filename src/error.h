/**
 * @file   error.h
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Tue Jun 21 15:37:10 2011
 * 
 * @brief  Error Routines header.
 * 
 * Contains all of the necessary error routines  
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

#ifndef SCADS_ERROR_H
#define SCADS_ERROR_H

#include "pointers.h"

namespace SCADS_NS {

	/**
	 * Error Class
	 * these can be called like: error->all("Hey, this is broken")
	 */

	class Error: protected Pointers {
		public:
			Error(class SCADS *);

			int verbosity_level; /**< Verbosity Level, higher = more verbose */

			// Normal error throwing routines
			void all(const char *); /**< Error call for all processors */
			void one(const char *); /**< Error call for one processor */

			void warning(const char *, int = 1); /**< Send a warning message */
			void message(char *); /**< Send a message*/
			void message(char *, int); /**< Send a message only if verbosity > */

			// new routines to take advantage of file name and line number
			// currently overloaded to keep backwards compatibility.

			void all(const char *file, int line, const char *str);
			void one(const char *file, int line, const char *str);
			void warning(const char *file, int line, const char *str);
			void message(const char *file, int line, const char *str);
	};

}

#endif
