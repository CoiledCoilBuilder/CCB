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
 * @file   output_pdb.h
 * @author CVB <chris@CirithUngol>
 * @date   Sun Jun 26 14:39:03 2011
 * 
 * @brief  pdb output style header
 * 
 * 
 */


#ifdef OUTPUT_CLASS

OutputStyle(pdb,OutputPDB)

#else

#ifndef CCB_OUTPUT_PDB_H
#define CCB_OUTPUT_PDB_H

#include "output.h"

namespace CCB_NS {
     
     class OutputPDB : public Output {
     
     public:
          OutputPDB(class CCB *, int, const char **);
          ~OutputPDB() {}
     
     private:
          int mask;                   /**< bitmask for atoms to output */
          int init_style();          /**< Initialize the style */
          int write_style();         /**< read the file based on style */
          void chomp(char *s, int n); /**< Shorten string to n characters */
     };
}

#endif
#endif
