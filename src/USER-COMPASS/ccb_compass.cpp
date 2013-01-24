// -*- c++ -*-

/**
 * @file   ccb_compass.cpp
 * @author macdercm <macdercm@Mount-Doom>
 * @date   Thu Jan 10 09:09:47 2013
 *
 * @brief  Plugin interface for Compass
 */

#include <vector>
#include <iostream>
#include "ccb_compass.h"
#include "ccb.h"
#include "domain.h"
#include "site.h"
#include "group.h"
#include "atom.h"
#include "backbonehandler.h"
#include "backbone.h"
#include "backbone_coiledcoil.h"
#include "error.h"

using namespace SCADS_NS;

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
        : Plugin_BaseClass(mytype,myname)
{

    ccb = NULL;

}

CCB_Compass::~CCB_Compass() {

    delete ccb;
}


void CCB_Compass::init(const int argc, const char **argv)
{

    // If we've initialized, only pass args 
    if (init_done) {
        ccb->backbone->update_backbone(name,argc,argv,0);
        return;
    }

    //Fire up a ccb instance
    ccb = new CCB_NS::CCB(0, NULL);

    const char **newarg = new const char*[5];

    /// Add the coiled-coil plugin, pass arguments
    newarg[0] = (char *) "backbone";
    newarg[1] = (char *) "add";
    newarg[2] = (char *) "coiledcoil";
    newarg[3] = (char *) name;
    ccb->backbone->add_backbone(4,newarg);
    ccb->backbone->init_backbone(name);

    delete [] newarg;

    init_done = 1;
    return;
}

void CCB_Compass::compute(int flag)
{
  
    if (!init_done) 
     std::cout << "CCB:Compute: You must call 'init' first" << std::endl;

    // Generate the coordinates
    ccb->backbone->generate_backbone(name);

    ++num_compute;
    return;
}

double CCB_Compass::single(int flag1, int flag2)
{
    ++num_single;
    return 1.0;
}

/**
 * @brief Transfer atomic data between the plugin and compass
 *
 * @param data vector of plugindata_t to be passed by reference from scads
 * @return true/false if routine returns someting useful
 */

bool CCB_Compass::get_atomic_data(std::vector<pluginatomic_t> &v) {

    CCB_NS::Atom *a = NULL;
    CCB_NS::Site *s = NULL;

    pluginatomic_t p;
    pluginatomic_t *pd = &p; 


    // Print out the coordinates
    for (int i = 0; i < ccb->domain->nsite; i++) {
        s = ccb->domain->site[i];

        for (int j = 0; j < ccb->domain->site[i]->fixed_atoms->natom; j++) {
            
            a = ccb->domain->site[i]->fixed_atoms->atom[j];

            // atom base quantities
            pd->serial = a->serial;
            pd->name = a->name;
            pd->type = a->type;
            pd->element = a->element;

            pd->x = a->x;
            pd->y = a->y;
            pd->z = a->z;
            pd->o = a->o;
            pd->b = a->b;

            // Group based quantities
            pd->groupname = s->fixed_atoms->name;
            pd->grouptype = s->fixed_atoms->type;

            // site based quantities
            pd->resid = s->resid;
            pd->chain = s->chain;
            pd->seg = s->seg;

            // Add the struct to the vector
            v.push_back(*pd);
        }
    }

    // Make sure "update" knows we've returned atomic data to be 
    // added to the domain
    return true;

}
