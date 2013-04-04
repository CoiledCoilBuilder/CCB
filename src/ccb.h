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
 * @file   ccb.h
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Tue Jun 21 09:29:05 2011
 * 
 * @brief  Main CCB header, define namespace, fundamental and top level classes.
 */

#ifndef CCB_CCB_H
#define CCB_CCB_H

#include "stdio.h"

/**
 * @namespace CCB_NS
 *
 * The ccb namespace. This is the top-level namespace 
 * all classes should belong to this namespace if they're
 * inherited from the ccb super-class.
 */


namespace CCB_NS {

	/**
	 * CCB main class
	 *
	 * All fundamental and top level classes initialize under the CCB class.
	 *
	 * This class is initialized in the main.cpp file. All fundamental and
	 * top-level classes should be public and listed below. Note that the top level
	 * and fundamental class names have a capital beginning letter, their pointers do not.
	 */

	class CCB {

     public:

          // Pointers to fundamental CCB classes

          class Memory *memory; /**< Memory Allocation Routines: memory.cpp/memory.h, taken from LAMMPS */
          class Error *error; /**< Error handling: error.cpp/error.h,	taken from LAMMPS */
          class Universe *universe; /**< Universe of Processors: universe.cpp/universe.h, taken from LAMMPS */
          class Ccbio *ccbio; /**< Input and output styles for reading and outputting files */

          // Pointers to top-level CCB classes

          class Domain *domain; /**< All atomistic/coarse-grain propertities and quantities related to the system: domain.cpp/domain.h */
          class Bitmask *bitmask; /**< Dynamic collections of atoms... */
          class BackboneHandler *backbone; /** <manages backbone modeler styles */

          // Output and Communication
          FILE *screen; /**< Output to Screen, "what am I doing at this very moment?" */

          // Constructor and Destructors
          CCB(int, char**);
          ~CCB();

          // Functions
          void create(); /**< Creates instances of the top level classes */
          void init(); /**< Initializes & sets up the top level classes, Input->init() */
          void destroy(); /**< Destroys the top level classes */
          void test();   /**< testing function for debugging */

     private:

	};

}

#endif
