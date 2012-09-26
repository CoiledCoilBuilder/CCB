/**
 * @file   pointers.h
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Tue Jun 21 14:06:07 2011
 *
 * @brief  Pointers to top level SCADS classes
 *
 * All top level classes inherit from pointers. This allows
 * one to call error->all(), for example, from any class.
 *
 * Pointers class contains ptrs to master copy of
 * fundamental SCADS class ptrs stored in scads.h
 * every SCADS class inherits from Pointers to access lammps.h ptrs
 * these variables are auto-initialized by Pointer class constructor
 * *& variables are really pointers to the pointers in scads.h
 * and enables them to be accessed directly in any class, e.g. atom->x
 *
 * This same approach is taken by LAMMPS.
 *
 * An assignment operator has been added so Atoms, Groups and Sites
 * can be copied.
 *
 */

#ifndef SCADS_POINTERS_H
#define SCADS_POINTERS_H

#include "scads.h"
#include "scadstype.h"

namespace SCADS_NS {

#define FLERR __FILE__,__LINE__

    class Pointers {
    public:
    Pointers(SCADS *ptr) :
        scads(ptr),
            memory(ptr->memory),
            error(ptr->error),
            universe(ptr->universe),
            scadsio(ptr->scadsio),
            domain(ptr->domain),
            bitmask(ptr->bitmask),
            backbone(ptr->backbone),
            screen(ptr->screen) {}
        virtual ~Pointers() {}

        // We just need pointers to point...
        Pointers& operator=(Pointers const &) {
            return *this;
        }

    protected:
        SCADS *scads;
        Memory *&memory;
        Error *&error;
        Universe *&universe;
        Scadsio *&scadsio;

        Domain *&domain;
        Bitmask *&bitmask;
        BackboneHandler *&backbone;

        FILE *&screen;
    };
}

#endif


