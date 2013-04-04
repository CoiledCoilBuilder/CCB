// -*-c++-*-

  // +------------------------------------------------------------------------------------+ 
  // |  This file is part of Coiled-Coil Builder.                                         | 
  // |                                                                                    | 
  // |  Coiled-Coil Builder is free software: you can redistribute it and/or modify       | 
  // |  it under the terms of the GNU General Public License as published by              | 
  // |  the Free Software Foundation, either version 3 of the License, or                 | 
  // |  (at your option) any later version.                                               | 
  // |                                                                                    | 
  // |  Coiled-Coil Builder is distributed in the hope that it will be useful,            | 
  // |  but WITHOUT ANY WARRANTY without even the implied warranty of                     | 
  // |  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                     | 
  // |  GNU General Public License for more details.                                      | 
  // |                                                                                    | 
  // |  You should have received a copy of the GNU General Public License                 | 
  // |  along with Coiled-Coil Builder.  If not, see <http:www.gnu.org/licenses/>.        | 
  // |                                                                                    | 
  // |   *cr                                                                              | 
  // |   *cr            (C) Copyright 1995-2013 The Board of Trustees of the              | 
  // |   *cr                        University of Pennsylvania                            | 
  // |   *cr                         All Rights Reserved                                  | 
  // |   *cr                                                                              | 
  // +------------------------------------------------------------------------------------+ 

/**
 * @file   universe.cpp
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Wed Jun 22 09:15:04 2011
 * 
 * @brief  Contains information about the state of the processors
 *
 * Contains routines pertaining to the number of processors, the processor IDs (ranks), 
 * the partitioning of the processors etc..
 */

#include "universe.h"
#include "version.h"

using namespace CCB_NS;

/** 
 * The universe constructor.
 *
 * @param ccb The ccb class pointer 
 */

Universe::Universe(CCB *ccb) : Pointers(ccb)
{
     version = (char *) CCB_VERSION; 

	//Figure out how many threads running with
 	nthreads = 1;

        // Always rank 0 wrt the universe 
        me = 0;

}

/** 
 * The Universe Deconstructor
 *
 *
 */

Universe::~Universe()
{
}
