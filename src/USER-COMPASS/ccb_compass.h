// -*- c++ -*-

#ifndef _CCB_COMPASS_H_
#define _CCB_COMPASS_H_

#include <vector>
#include "plugin_baseclass.h"
#include "ccb.h"
#include "plugin.h"

extern "C"
{
    void *create_ccb(const char *);
    void destroy_ccb(void *);
    int plugin_init(plugin_t *);
}

namespace SCADS_NS {

class CCB_Compass : public Plugin_BaseClass
{
  public:
    CCB_Compass(const char *, const char *);
    virtual ~CCB_Compass();

  protected:
    CCB_Compass() {};

  protected:
    class CCB_NS::CCB *ccb;

  public:
    virtual int init(const int, const char **);
    virtual int compute(int);
    virtual double single(int,int);
    virtual bool get_atomic_data(std::vector<pluginatomic_t> &);

   private:
     int init_done;

};

}

#endif /* _CCB_COMPASS_H_ */
