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
