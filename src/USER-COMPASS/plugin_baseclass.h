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
