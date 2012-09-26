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

using namespace SCADS_NS;

Atom::Atom(SCADS *scads, int id) :
    Pointers(scads) {

    /// Initialize variables
    this->id = id;
    serial = 0;
    x = y = z = 0.0;
    o = b = 0.0;
    strcpy(name, "");
    strcpy(type, "");
    strcpy(element, "");

    site = NULL;
    group = NULL;

    // Atoms automatically set "all" bitmask;
    mask = 0;
    mask |= 1;

    fixed = 0;
    backbone = 0;
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
