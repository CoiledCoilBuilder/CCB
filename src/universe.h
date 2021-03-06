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
 * @file   universe.h
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Wed Jun 22 09:06:38 2011
 * 
 * @brief  Contains information about the state of the processors
 * 
 * 
 */

#ifndef CCB_UNIVERSE_H
#define CCB_UNIVERSE_H

#include "pointers.h"

namespace CCB_NS {

     /**
      * The Universe Class
      */

     class Universe : protected Pointers {
       public:
 
         // Constructors and Destructors
          Universe(class CCB *); /**< The Universe Constructor */
          ~Universe();                       /**< The Universe Deconstructor */
         
          char *version;        /**< CCB version */
          int nthreads;			/**< OMP_NUM_THREADS */

          int me;

     };
}

#endif
