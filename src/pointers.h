// -*-c++-*-

  // +------------------------------------------------------------------------------------+ 
  // |  This file is part of Coiled-Coil Builder.                                         | 
  // |                                                                                    | 
  // |  Coiled-Coil Builder is free software: you can redistribute it and/or modify       | 
  // |  it under the terms of the GNU General Public License as published by              | 
  // |  the Free Software Foundation, either version 3 of the License, or                 | 
  // |  (at your option) any later version.                                               | 
  // |                                                                                    | 
  // |  Coiled-Coil Builder is distributed in the hope that it will be useful,            | 
  // |  but WITHOUT ANY WARRANTY without even the implied warranty of                     | 
  // |  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                     | 
  // |  GNU General Public License for more details.                                      | 
  // |                                                                                    | 
  // |  You should have received a copy of the GNU General Public License                 | 
  // |  along with Coiled-Coil Builder.  If not, see <http:www.gnu.org/licenses/>.        | 
  // |                                                                                    | 
  // |   *cr                                                                              | 
  // |   *cr            (C) Copyright 1995-2013 The Board of Trustees of the              | 
  // |   *cr                        University of Pennsylvania                            | 
  // |   *cr                         All Rights Reserved                                  | 
  // |   *cr                                                                              | 
  // +------------------------------------------------------------------------------------+ 

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


