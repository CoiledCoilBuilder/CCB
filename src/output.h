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
 * @file   output.h
 * @author CVB <cvb@magrathea.chem.upenn.edu>
 * @date   Thu Jun 23 15:53:06 2011
 * 
 * @brief  Main header for Output class and subclasses
 * 
 * All output styles inherit from this output class
 * note the virtual funtions that are replaced by the
 * inheriting subclass "style". Because of this scheme
 * subclasses can still use the pointer notation error->all!
 */

#ifndef CCB_OUTPUT_H
#define CCB_OUTPUT_H

#include "pointers.h"

namespace CCB_NS {

class Output: protected Pointers {

public:

	char *filename; /**< the output filename */
	char *id; /**< the ID of the syle */
	char *style; /**< the name of the style */

	//Constructor and Destructor
	Output(class CCB *, int, const char **); /**< Output constructor */
	virtual ~Output(); /**< Output destructor, must be virtual! */

	int init();
	int write();

protected:
	int me; /**< Processors info */

	// Child Class Functions
	virtual int init_style() = 0; /**< Initialize the style (declare member variables, etc.. */
	virtual int write_style() = 0; /**< read the file */

	virtual int openfile(); /**< open the file, note this is NOT pure virtual */
	virtual int closefile();

	FILE *fp; /**< file pointer */
private:

};

}

#endif
