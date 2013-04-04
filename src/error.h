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
 * @file   error.h
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Tue Jun 21 15:37:10 2011
 *
 * @brief  Error Routines header.
 *
 * Contains all of the necessary error routines
 */

#ifndef CCB_ERROR_H
#define CCB_ERROR_H

#include "pointers.h"

#define FLERR __FILE__,__LINE__
#define CCB_OK 0
#define CCB_ERROR 1

namespace CCB_NS {

/**
 * Error Class
 * these can be called like: error->all("Hey, this is broken")
 */

class Error: protected Pointers {
  public:
    Error(class CCB *);

    int verbosity_level; /**< Verbosity Level, higher = more verbose */

    int  all(const char *file, int line, const char *str);
    int  one(const char *file, int line, const char *str);
    void warning(const char *file, int line, const char *str);
    void message(const char *file, int line, const char *str);
    void message_verb(char *str, int verbosity);
};

}

#endif
