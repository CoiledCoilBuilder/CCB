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
