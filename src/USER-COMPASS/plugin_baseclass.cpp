

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
