// -*- c++ -*-

#ifndef _CCB_COMPASS_H_
#define _CCB_COMPASS_H_

#include "base_class.h"
#include "plugin.h"

extern "C"
{
    void *create_ccb(const char *);
    void destroy_ccb(void *);
    int plugin_init(plugin_t *);
}

class CCB_Compass : public BaseClass
{
  public:
    CCB_Compass(const char *, const char *);
    virtual ~CCB_Compass();

  protected:
    CCB_Compass() {};

  public:
    virtual void init(int, char **);
    virtual void compute(int);
    virtual double single(int,int);
};


#endif /* _CCB_COMPASS_H_ */
