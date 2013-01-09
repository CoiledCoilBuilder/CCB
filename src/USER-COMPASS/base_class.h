// -*- c++ -*-
// base class for plugin classes
#ifndef BASE_CLASS_H
#define BASE_CLASS_H

class BaseClass 
{

public:
     const char *name;          /**< User provided handle to the class instance */
     const char *type;           /**< The type of the class, backbone, compute, etc.. */

protected:

     int init_done;
     int num_compute;
     int num_single;

public:
     BaseClass(const char *, const char *);
     virtual ~BaseClass();

     // disable default constructor
protected:
     BaseClass() {};

     // generic class method
public:
     void setup(int);
    
     // virtual methods to be overridded in derived classes
public:
     virtual void init(int, char **) = 0;
     virtual void compute(int) = 0;
     virtual double single(int, int) { ++num_single; return 0.0;};
};

#endif
