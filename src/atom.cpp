

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
