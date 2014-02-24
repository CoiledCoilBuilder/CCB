// -*-c++-*-

// *hd +------------------------------------------------------------------------------------+
// *hd |  This file is part of Coiled-Coil Builder.                                         |
// *hd |                                                                                    |
// *hd |  Coiled-Coil Builder is free software: you can redistribute it and/or modify       |
// *hd |  it under the terms of the GNU General Public License as published by              |
// *hd |  the Free Software Foundation, either version 3 of the License, or                 |
// *hd |  (at your option) any later version.                                               |
// *hd |                                                                                    |
// *hd |  Coiled-Coil Builder is distributed in the hope that it will be useful,            |
// *hd |  but WITHOUT ANY WARRANTY without even the implied warranty of                     |
// *hd |  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                     |
// *hd |  GNU General Public License for more details.                                      |
// *hd |                                                                                    |
// *hd |  You should have received a copy of the GNU General Public License                 |
// *hd |  along with Coiled-Coil Builder.  If not, see <http:www.gnu.org/licenses/>.        |
// *hd +------------------------------------------------------------------------------------+

// *hd | If you intend to use this software for your research, please cite:
// *hd | and inform Chris MacDermaid <chris.macdermaid@gmail.com> of any pending publications.

// *hd | Copyright (c) 2012,2013,2014 by Chris M. MacDermaid <chris.macdermaid@gmail.com>
// *hd | and Jeffery G. Saven <saven@sas.upenn.edu>


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

#ifdef __GNUC__
#  define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#else
#  define UNUSED(x) UNUSED_ ## x
#endif

#ifdef __GNUC__
#  define UNUSED_FUNCTION(x) __attribute__((__unused__)) UNUSED_ ## x
#else
#  define UNUSED_FUNCTION(x) UNUSED_ ## x
#endif

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


