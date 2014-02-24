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
 * @file   backbonehandler.h
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Fri Jan 20 16:35:19 2012
 * 
 * @brief  Creates and manages backbone modeling styles 
 * 
 */

#ifndef CCB_BACKBONEHANDLER_H
#define CCB_BACKBONEHANDLER_H

#include "pointers.h"

namespace CCB_NS {
	class BackboneHandler: protected Pointers {
     public:

          //Constructor and Destructor
          BackboneHandler(class CCB *); /**< BackboneHandler constructor */
          ~BackboneHandler(); /**< BackboneHandler deconstructor */

          // Memory Usage
          void memory_usage();

          class Backbone **backbone; /**< List of backbone modelers */

          // Backbone Modeler Styles
          int nbackbone; /**< Running count of input styles */
          int maxbackbone; /**< Maximum number of inputs to create */

          // Public Functions for manipulating backbone modelers 

          int init_backbone(const char *);
          int add_backbone(int, const char **);
          int delete_backbone(const char *);
          int update_backbone(const char *id, int argc, const char **argv, int n);
          int generate_backbone(const char *id);
          int find_backbone(const char *);
               
     private:

	};
}

#endif
