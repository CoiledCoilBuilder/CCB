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
 * @file   atom.cpp
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Mon Jun 27 16:49:25 2011
 *
 * @brief  atom based quantities routine
 *
 *
 */

#include <stdlib.h>
#include "string.h"
#include "pointers.h"
#include "domain.h"
#include "site.h"
#include "group.h"
#include "atom.h"

using namespace CCB_NS;

Atom::Atom(CCB *ccb, int myid) :
        Pointers(ccb),
        site(),
        group(),
        id(myid),
        serial(0),
        mask(1),
        x(0.0),
        y(0.0),
        z(0.0),
        o(0.0),
        b(0.0),
        fixed(0),
        backbone(0)
{

    /// Initialize variables
    name[0] = '\0';
    type[0] = '\0';
    element[0] = '\0';
}

Atom::~Atom() {
}

Atom::Atom(const Atom &a) : Pointers(a) {

    site = NULL;
    group = NULL;

    id = a.id;
    serial = a.serial;
    mask = a.mask;
    strcpy(name, a.name);
    strcpy(type, a.type);
    strcpy(element, a.element);
    x = a.x;
    y = a.y;
    z = a.z;
    o = a.o;
    b = a.b;
    fixed = a.fixed;
    backbone = a.backbone;
}

Atom& Atom::operator= (Atom const& a) {

    if (this == &a) return *this;

    // base assignment operator
    Pointers::operator=(a);

    // Reset pointers to sites and groups
    site = NULL;
    group = NULL;

    id = a.id;
    serial = a.serial;
    mask = a.mask;
    strcpy(name, a.name);
    strcpy(type, a.type);
    strcpy(element, a.element);
    x = a.x;
    y = a.y;
    z = a.z;
    o = a.o;
    b = a.b;
    fixed = a.fixed;
    backbone = a.backbone;

    return *this;
}
