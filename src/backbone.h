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
 * @file   backbone.h
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Fri Jan 20 16:44:06 2012
 * 
 * @brief  Main header for backbone modeler base class 
 * 
 */

#ifndef CCB_BACKBONE_H
#define CCB_BACKBONE_H

#include "pointers.h"

namespace CCB_NS {
  class Backbone: protected Pointers {

    public:

            char *id; /**< the ID of the syle */
            char *style; /**< the name of the style */

            //Constructor and Destructor
            Backbone(class CCB *, int, const char **); /**< Backbone constructor */
            virtual ~Backbone(); /**< Backbone destructor, must be virtual! */

            int init();
            int update(int argc, const char **argv, int n); /**< Update parameters */
            int generate();    /**< Generate coordiantes */

    protected:

            // Child Class Functions
            virtual int init_style() = 0; /**< Initialize the style (declare member variables, etc.. */
            virtual int update_style(int argc, const char **argv, int n) = 0; /**< update coordinates based on passed params*/
            virtual int generate_style() = 0; /**< Generate Coordiantes for the particular style */

    private:

  };
}

#endif
