// -*- c++ -*-

#include "ccb_compass.h"
#include "ccb.h"
#include "backbonehandler.h"

using namespace CCB_NS;

int plugin_init(plugin_t *p)
{
     if (p == 0) return PLUGIN_FAIL;

     p->abi     = PLUGIN_ABI;
     p->version = 1;
     p->name    = (const char*) "ccb";
     p->desc    = (const char*) "Coiled-Coil Builder";
     p->author  = (const char*) "Chris MacDermaid";
     p->create_class  = &create_ccb;
     p->destroy_class = &destroy_ccb;

     return PLUGIN_OK;
}

void *create_ccb(const char *name)
{
     CCB_Compass *p;

     p = new CCB_Compass("ccb",name);
     return static_cast<void *>(p);
}

void destroy_ccb(void *p) 
{
     CCB_Compass *d;
     d = static_cast<CCB_Compass *>(p);
     delete d;
}

CCB_Compass::CCB_Compass(const char *mytype, const char *myname)
          : BaseClass(mytype,myname)
{

}

CCB_Compass::~CCB_Compass() {

}


void CCB_Compass::init(int argc, char **argv)
{

     //Fire up a ccb instance
     CCB *ccb = new CCB(0, NULL);

     const char **newarg = new const char*[5];

     /// Add the coiled-coil plugin
     newarg[0] = (char *) "backbone";
     newarg[1] = (char *) "add";
     newarg[2] = (char *) "coiledcoil";
     newarg[3] = (char *) "bbcc1";
     ccb->backbone->add_backbone(4,newarg);
     ccb->backbone->init_backbone(newarg[3]);

     // Generate the coordinates
     ccb->backbone->generate_backbone(newarg[3]);

     //Delete ccb instance
     delete ccb;

     init_done = 1;
     return;
}

void CCB_Compass::compute(int flag)
{
     ++num_compute;
     return;
}

double CCB_Compass::single(int flag1, int flag2)
{
     ++num_single;
     return 1.0;
}
