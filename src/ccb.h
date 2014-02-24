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
