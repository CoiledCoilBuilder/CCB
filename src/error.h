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
