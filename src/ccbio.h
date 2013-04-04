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
 * @file   ccbio.h
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Thu Jun 23 16:54:19 2011
 *
 * @brief  Creates and manages input and output styles
 *
 * Anything that is input or output to/from the program
 * must be setup using these routines.
 *
 * To call this, it should be something like:
 * input in1 control fname
 * output out1 pdb fname
 *
 * input = directive
 * in1 = unique name given to the input
 * control = the input "style"
 * fname = the path to the input file
 */

#ifndef CCB_CCBIO_H
#define CCB_CCBIO_H

#include "pointers.h"

namespace CCB_NS {
class Ccbio: protected Pointers {
  public:

    //Constructor and Destructor
    Ccbio(class CCB *); /**< Ccbio constructor */
    ~Ccbio(); /**< Ccbio deconstructor (deletes all associated styles on termination) */

    // Memory Usage
    void memory_usage();

    class Output **output; /**< List of Outputs, e.g. pointer to pointer to output */

    //Output Styles
    int noutput; /**< Running count of output styles */
    int maxoutput; /**< Maximum number of outputs to create */

    //Outputs
    int init_output(const char *);
    int write_output(const char *);
    int add_output(int, const char **);
    int delete_output(const char *);
    int find_output(const char *);

    void active_outputs();

  private:

};
}

#endif
