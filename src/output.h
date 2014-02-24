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
