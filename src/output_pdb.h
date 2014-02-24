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
