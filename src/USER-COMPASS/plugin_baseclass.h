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
     virtual void init(const int, const char **) = 0;
     virtual void compute(int) = 0;
     virtual bool get_atomic_data(std::vector<pluginatomic_t> &) = 0;
     virtual double single(int, int) { ++num_single; return 0.0;};
};

}

#endif
