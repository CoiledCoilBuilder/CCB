// -*- c++ -*-
// plugin base class implementation

#include <iostream>
#include <cstdlib>

#include "base_class.h"

BaseClass::BaseClass(const char *mytype, const char *myname)
          : type(mytype), name(myname), init_done(0), num_compute(0), num_single(0)
{
     std::cout << "created new instance of class " << type << std::endl;
}

BaseClass::~BaseClass()
{
     std::cout << "deleted instance of class " << type << std::endl;
}

void BaseClass::setup(int reset)
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
