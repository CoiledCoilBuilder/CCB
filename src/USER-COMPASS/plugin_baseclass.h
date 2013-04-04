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

// -*- c++ -*-
// base class for plugin classes
#ifndef SCADS_PLUGIN_BASE_CLASS_H
#define SCADS_PLUGIN_BASE_CLASS_H

#include <vector>
#include "plugin.h"

namespace SCADS_NS {

class Plugin_BaseClass 
{

public:
     char *type;           /**< The type of the class, backbone, compute, etc.. */
     char *name;          /**< User provided handle to the class instance */

protected:

     int init_done;
     int num_compute;
     int num_single;

public:
     Plugin_BaseClass(const char *, const char *);
     virtual ~Plugin_BaseClass();

     // disable default constructor
protected:
     Plugin_BaseClass() {};

     // generic class method
public:
     void setup(int);
    
     // virtual methods to be overridded in derived classes
public:
     virtual int init(const int, const char **) = 0;
     virtual int compute(int) = 0;
     virtual bool get_atomic_data(std::vector<pluginatomic_t> &) = 0;
     virtual double single(int, int) { ++num_single; return 0.0;};
};

}

#endif
