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
#include "tagdata.h"
#include "gid.h"

using namespace SCADS_NS;

Atom::Atom(SCADS *scads, int id) :
    Pointers(scads) {

    /// Initialize variables
    this->id = id;
    gid = -1;
    serial = 0;
    x = y = z = 0.0;
    o = b = 0.0;
    mass = charge = rmin = rmin14 = epsilon = epsilon14 = 0.0;
    user3 = user4 = 0.0;
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
    interresidue_atom = 0;

    // build_flag =  1 (built properly)
    // build_flag =  0 (not built)
    // build_flag = -1 (misbuilt)
    build_flag = 0;

}

Atom::~Atom() {

    // Remove from global array
    if (gid >= 0) {
        domain->gid->delete_atom(gid);
    }

}

Atom::Atom(const Atom &a) : Pointers(a) {

    site = NULL;
    group = NULL;

    id = a.id;
    gid = a.gid;
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
    interresidue_atom = a.interresidue_atom;
    build_flag = a.build_flag;
    mass = a.mass;
    charge = a.charge;
    rmin = a.rmin;
    rmin14 = a.rmin14;
    epsilon = a.epsilon;
    epsilon14 = a.epsilon14;
    user3 = a.user3;
    user4 = a.user4;

    // Get unique gid if part of domain
    if (gid >= 0) {
        gid = -1;
        make_global();
    }

}

Atom& Atom::operator= (Atom const& a) {

    if (this == &a) return *this;

    // base assignment operator
    Pointers::operator=(a);

    // Reset pointers to sites and groups
    site = NULL;
    group = NULL;

    id = a.id;
    gid = a.gid;
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
    interresidue_atom = a.interresidue_atom;
    build_flag = a.build_flag;
    mass = a.mass;
    charge = a.charge;
    rmin = a.rmin;
    rmin14 = a.rmin14;
    epsilon = a.epsilon;
    epsilon14 = a.epsilon14;
    user3 = a.user3;
    user4 = a.user4;

    // Get unique gid if part of domain
    if (gid >= 0) {
        gid = -1;
        make_global();
    }

    return *this;
}

/**
 * Put this atom into the global lookup table
 * e.g. it's part of the domain and not a
 * temp atom and is subject to atomselections
 */

void Atom::make_global() {

    if (gid < 0) {

        // Add atom to lookup table, assign GID
        domain->gid->add_atom(this);

        // Inherit the site's bitmask
        if (site)
            mask |= site->mask;
    }
}


/**
 * @brief get a particular atom field for atomselections
 *
 * @param field the field type
 * @param dataobj a struct-union-enum data type
 */
void Atom::get_field(const char *field, TagData *&dataobj) {

    // return the datatype correspoing to field
    // using the tag_data type

    if (strcmp(field, "atomid") == 0) {
        dataobj->type = TagData::INT;
        dataobj->data.i = id;
        return;

    } else if (strcmp(field, "gid") == 0) {
        dataobj->type = TagData::BIGINT;
        dataobj->data.bi = gid;
        return;

    } else if (strcmp(field, "serial") == 0) {
        dataobj->type = TagData::INT;
        dataobj->data.i = serial;
        return;

    } else if (strcmp(field, "mask") == 0) {
        dataobj->type = TagData::INT;
        dataobj->data.i = mask;
        return;

    } else if (strcmp(field, "name") == 0) {
        dataobj->type = TagData::STRING;
        dataobj->data.s = name;
        return;

    } else if (strcmp(field, "type") == 0) {
        dataobj->type = TagData::STRING;
        dataobj->data.s = type;
        return;

    } else if (strcmp(field, "element") == 0) {
        dataobj->type = TagData::STRING;
        dataobj->data.s = element;
        return;

    } else if (strcmp(field, "x") == 0) {
        dataobj->type = TagData::FLOAT;
        dataobj->data.f = x;
        return;

    } else if (strcmp(field, "y") == 0) {
        dataobj->type = TagData::FLOAT;
        dataobj->data.f = y;
        return;

    } else if (strcmp(field, "z") == 0) {
        dataobj->type = TagData::FLOAT;
        dataobj->data.f = z;
        return;

    } else if (strcmp(field, "o") == 0) {
        dataobj->type = TagData::FLOAT;
        dataobj->data.f = o;
        return;

    } else if (strcmp(field, "b") == 0) {
        dataobj->type = TagData::FLOAT;
        dataobj->data.f = b;
        return;

    } else if (strcmp(field, "mass") == 0) {
        dataobj->type = TagData::FLOAT;
        dataobj->data.f = mass;
        return;

    } else if (strcmp(field, "charge") == 0) {
        dataobj->type = TagData::FLOAT;
        dataobj->data.f = charge;
        return;

    } else if (strcmp(field, "rmin") == 0) {
        dataobj->type = TagData::FLOAT;
        dataobj->data.f = rmin;
        return;

    } else if (strcmp(field, "epsilon") == 0) {
        dataobj->type = TagData::FLOAT;
        dataobj->data.f = epsilon;
        return;

    } else if (strcmp(field, "user3") == 0) {
        dataobj->type = TagData::FLOAT;
        dataobj->data.f = user3;
        return;

    } else if (strcmp(field, "user4") == 0) {
        dataobj->type = TagData::FLOAT;
        dataobj->data.f = user4;
        return;

    } else if (strcmp(field, "backbone") == 0) {
        dataobj->type = TagData::INT;
        dataobj->data.i = (int) backbone;

    } else if (strcmp(field, "fixed") == 0) {
        dataobj->type = TagData::INT;
        dataobj->data.i = (int) fixed;

        // site properties
    } else if (strcmp(field, "siteid") == 0) {
        dataobj->type = TagData::INT;
        dataobj->data.i = site->id;
        return;

    } else if (strcmp(field, "resid") == 0) {
        dataobj->type = TagData::INT;
        dataobj->data.i = site->resid;
        return;

    } else if (strcmp(field, "chain") == 0) {
        dataobj->type = TagData::STRING;
        dataobj->data.s = site->chain;
        return;

    } else if (strcmp(field, "seg") == 0) {
        dataobj->type = TagData::STRING;
        dataobj->data.s = site->seg;
        return;

        // group properties
    } else if (strcmp(field, "groupid") == 0) {
        dataobj->type = TagData::INT;
        dataobj->data.i = group->id;
        return;

    } else if (strcmp(field, "groupname") == 0) {
        dataobj->type = TagData::STRING;
        dataobj->data.s = group->name;
        return;

    } else if (strcmp(field, "grouptype") == 0 || strcmp(field, "resname") == 0) {
        dataobj->type = TagData::STRING;
        dataobj->data.s = group->type;
        return;

    } else if (strcmp(field, "totalcharge") == 0) {
        dataobj->type = TagData::FLOAT;
        dataobj->data.f = group->totalcharge;
        return;

    } else if (strcmp(field, "probability") == 0) {
        dataobj->type = TagData::FLOAT;
        dataobj->data.f = group->probability;
        return;

    } else {
        // Bad field
        dataobj->type = TagData::UNDEF;
        return;
    }
}

void Atom::set_field(const char *field, const char *val) {

    // set the field passed from tcl

    /*
     * Note, this has the potential to be abused.
     * The user shouldn't really be able to change
     * the id, gid, groupid, siteid. We allow it though
     * for consistency...
     *
     * currently, no checks for appropirate types
     * are performed, this is mostly for speed but
     * could easily be screwed up by the user
     *
     * Need to write isfloat, isint is char etc..
     *
     */

    if (strcmp(field, "atomid") == 0) {
        id = atoi(val);
        return;

    } else if (strcmp(field, "gid") == 0) {
        gid = atoi(val);
        return;

    } else if (strcmp(field, "serial") == 0) {
        serial = atoi(val);
        return;

    } else if (strcmp(field, "mask") == 0) {
        mask = atoi(val);
        return;

    } else if (strcmp(field, "name") == 0) {
        strcpy(name, val);
        return;

    } else if (strcmp(field, "type") == 0) {
        strcpy(type, val);
        return;

    } else if (strcmp(field, "element") == 0) {
        strcpy(element, val);
        return;

    } else if (strcmp(field, "x") == 0) {
        x = atof(val);
        return;

    } else if (strcmp(field, "y") == 0) {
        y = atof(val);
        return;

    } else if (strcmp(field, "z") == 0) {
        z = atof(val);
        return;

    } else if (strcmp(field, "o") == 0) {
        o = atof(val);
        return;

    } else if (strcmp(field, "b") == 0) {
        b = atof(val);
        return;

    } else if (strcmp(field, "mass") == 0) {
        mass = atof(val);
        return;

    } else if (strcmp(field, "charge") == 0) {
        charge = atof(val);
        return;

    } else if (strcmp(field, "rmin") == 0) {
        rmin = atof(val);
        return;

    } else if (strcmp(field, "epsilon") == 0) {
        epsilon = atof(val);
        return;

    } else if (strcmp(field, "user3") == 0) {
        user3 = atof(val);
        return;

    } else if (strcmp(field, "user4") == 0) {
        user4 = atof(val);
        return;

    } else if (strcmp(field, "backbone") == 0) {
        backbone = atoi(val);
        return;

    } else if (strcmp(field, "fixed") == 0) {
        backbone = atoi(val);
        return;

        // site properties
    } else if (strcmp(field, "siteid") == 0) {
        site->id = atoi(val);
        return;

    } else if (strcmp(field, "resid") == 0) {
        site->resid = atoi(val);
        return;

    } else if (strcmp(field, "chain") == 0) {
        strcpy(site->chain, val);
        return;

    } else if (strcmp(field, "seg") == 0) {
        strcpy(site->seg, val);
        return;

        // group properties
    } else if (strcmp(field, "groupid") == 0) {
        group->id = atoi(val);
        return;

    } else if (strcmp(field, "groupname") == 0) {
        strcpy(group->name, val);
        return;

    } else if (strcmp(field, "grouptype") == 0 || strcmp(field, "resname") == 0) {
        strcpy(group->type, val);
        return;

    } else if (strcmp(field, "totalcharge") == 0) {
        group->totalcharge = atof(val);
        return;

    } else if (strcmp(field, "probability") == 0) {
        group->probability = atof(val);
        return;

    } else {
        // Bad field
        return;
    }
}
