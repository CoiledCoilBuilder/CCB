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
 * @file   main.cpp
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Tue Jun 21 08:53:03 2011
 * 
 * @brief  Initialization of CCB instance: sets up namespace, and cleans up. 
 * 
 * 
 */

#include <stdio.h>
#include "ccb.h"

using namespace CCB_NS;

/**
 * Main Routine creates a new ccb instance.
 * 
 * Instead of putting the main computational routine here, it's been moved to
 * ccb.h/ccb.cpp. This way, the initilization and cleanup is straightforward,
 * e.g. CCB *ccb = new CCB(argc, argv) 
 * e.g. delete ccb
 */

int main(int argc, char **argv) {

	// Create a CCB instance, pass arguments;
	CCB *ccb = new CCB(argc, argv);

	// Delete the CCB instance and clean up
	delete ccb;

	return 0;
}
