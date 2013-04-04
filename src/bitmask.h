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
 * @file   bitmask.h
 * @author Chris MacDermaid <chris@cabal.nfs>
 * @date   Sun Dec 11 19:05:07 2011
 * 
 * @brief  keeps track of groups of atoms via bitmask 
 *
 *
 */

#ifndef CCB_BITMASK_H
#define CCB_BITMASK_H

#include "pointers.h"

namespace CCB_NS {

  class Bitmask: protected Pointers {
      public:
  
          //Constructor and Destructor
          Bitmask(class CCB *);
          ~Bitmask(); 
  
          int nmask;
          char **names;
          int *mask;
          int *inversemask;

          int find(const char *name);
          int find_unused();
          int find_mask(const char *name);

          int add_bitmask(const char *name);
          int delete_bitmask(const char *name);

      private:
  
  };
  
}

#endif
