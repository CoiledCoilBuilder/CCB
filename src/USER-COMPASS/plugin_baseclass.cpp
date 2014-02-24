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
