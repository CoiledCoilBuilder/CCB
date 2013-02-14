/**
 * @file   pointers.h
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Tue Jun 21 14:06:07 2011
 *
 * @brief  Pointers to top level CCB classes
 *
 * All top level classes inherit from pointers. This allows
 * one to call error->all(), for example, from any class.
 *
 * Pointers class contains ptrs to master copy of
 * fundamental CCB class ptrs stored in ccb.h
 * every CCB class inherits from Pointers to access lammps.h ptrs
 * these variables are auto-initialized by Pointer class constructor
 * *& variables are really pointers to the pointers in ccb.h
 * and enables them to be accessed directly in any class, e.g. atom->x
 *
 * This same approach is taken by LAMMPS.
 *
 * An assignment operator has been added so Atoms, Groups and Sites
 * can be copied.
 *
 */

#ifndef CCB_POINTERS_H
#define CCB_POINTERS_H

#include "ccb.h"
#include "ccbtype.h"

namespace CCB_NS {


    class Pointers {
    public:
    Pointers(CCB *ptr) :
        ccb(ptr),
            memory(ptr->memory),
            error(ptr->error),
            universe(ptr->universe),
            ccbio(ptr->ccbio),
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
        CCB *ccb;
        Memory *&memory;
        Error *&error;
        Universe *&universe;
        Ccbio *&ccbio;

        Domain *&domain;
        Bitmask *&bitmask;
        BackboneHandler *&backbone;

        FILE *&screen;
    };
}

#endif


