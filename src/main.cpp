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
