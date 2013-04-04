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
// plugin base class implementation

#include <iostream>
#include <cstdlib>
#include <string.h>

#include "plugin_baseclass.h"

using namespace SCADS_NS;

Plugin_BaseClass::Plugin_BaseClass(const char *mytype, const char *myname)
          : init_done(0), num_compute(0), num_single(0)
{

     int n = strlen(mytype) + 1;
     type = new char[n];
     strcpy(type, mytype);
        
     n = strlen(myname) + 1;
     name = new char[n];
     strcpy(name, myname);

     std::cout << "created new instance of class " << type << std::endl;
}

Plugin_BaseClass::~Plugin_BaseClass()
{
     std::cout << "deleted instance of class " << type << std::endl;
}

void Plugin_BaseClass::setup(int reset)
{
     if (!init_done) {
          std::cout << "need to call " << type << "::init() before setup"
                    << std::endl;
          exit(1);
     }

     std::cout << "BaseClass setup for " << type << " with reset="
               << reset << std::endl;

     if (reset != 0) {
          num_compute = 0;
          num_single = 0;
     }
}
