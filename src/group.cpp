/**
 * @file   group.cpp
 * @author Chris Von Bargen <cvb@magrathea.chem.upenn.edu>
 * @date   Nov 14 2011
 *
 * @brief  Group, describing a collection of atoms
 *
 * Class to describe container for a collection of atoms, will fields
 * filled in as necessary. To be used(extended) as a static fixed_atoms group,
 * or a series of rotameric states.
 *
 */

#include "string.h"
#include "memory.h"
#include "error.h"
#include "atom.h"
#include "group.h"
#include "domain.h"

/**
 * @def ATOM_DELTA
 *
 * @brief Number of atoms to reallocate each time we run out of space
 */
#define ATOM_DELTA 30

using namespace CCB_NS;

/**
 * Group Constructor
 */
Group::Group(CCB *ccb, int myid) :
          Pointers(ccb), 
          id(myid),
          atom(),
          natom(0),
          site(),
          natom_iter(0),
          maxatom(0)
 {
      name[0] = '\0';
      type[0] = '\0';
}

/**
 * Group Deconstructor
 *
 * Delete all atoms
 */
Group::~Group() {

    /// Free up  atoms
    while (natom)
        delete_atom(atom[0]->id);
    memory->sfree(atom);
}

Group::Group(const Group &g) : Pointers(g) {

    atom = NULL;
    site = NULL;

    id = g.id;
    strcpy(name, g.name);
    strcpy(type, g.type);
    natom_iter = g.natom_iter;

    // Deep-copy the atoms;
    natom = g.natom;
    maxatom = g.maxatom;

    // Allocate memory for atoms
    atom = (Atom **) memory->srealloc(atom, maxatom * sizeof(Atom *), "group:atom");

    // Make a copy, don't copy the pointers but the instances instead
    for (int i = 0; i < natom; i++) {
        atom[i] = new Atom(*(g.atom[i]));

        //Set the group pointer
        atom[i]->group = this;
    }

}

Group& Group::operator= (Group const& g) {

    // don't create if self-assignment
    if (this == &g) return *this;

    // Base operator
    Pointers::operator=(g);

    // Cleanup instance being copied to
    for (int i = 0; i < natom; i++)
        delete atom[i];
    memory->sfree(atom);
    atom = NULL;

    // Site this group belongs to
    site = NULL;

    id = g.id;
    strcpy(name, g.name);
    strcpy(type, g.type);
    natom_iter = g.natom_iter;

    // Deep-copy the atoms;
    natom = g.natom;
    maxatom = g.maxatom;

    // Allocate memory for atoms
    atom = (Atom **) memory->srealloc(atom, maxatom * sizeof(Atom *), "group:atom");

    // Make a copy, don't copy the pointers but the instances instead
    for (int i = 0; i < natom; i++) {
        atom[i] = new Atom(*(g.atom[i]));

        //Set the group pointer
        atom[i]->group = this;
    }

    return *this;
}



/**
 * Add an atom to the group
 */
int Group::add_atom() {

    /// If we don't have enough space to add atoms, realloc atom to make room.
    if (natom == maxatom) {
        maxatom += ATOM_DELTA;
        atom = (Atom **) memory->srealloc(atom, maxatom * sizeof(Atom *), "group:atom");
    }

    atom[natom++] = new Atom(ccb, natom_iter++);

    return (natom - 1);
}
/**
 * @brief add an atom using the copy constructor
 *
 * @param a atom instance
 *
 * @return index to atom in rotamer atom array in rotamer
 */
int Group::add_atom(Atom &a) {

    /// If we don't have enough space to add atoms, realloc atom to make room.
    if (natom == maxatom) {
        maxatom += ATOM_DELTA;
        atom = (Atom **) memory->srealloc(atom, maxatom * sizeof(Atom *), "group:atom");
    }

    atom[natom] = new Atom(a);
    atom[natom]->id = natom_iter++;

    natom++;

    return (natom - 1);
}

/**
 * Create a new atom to add to the group, and
 * copy in all fields of atom pointer in argument
 *
 * @param   atom to copy into group
 */
int Group::add_atom(Atom *a) {

    // Check to see if we already have this atom
    int aid = find_atom_name(a->name);
    if (aid >= 0) {
        char str[128];
        sprintf(str, "Atom %s already exists in the group %s(%s)", a->name, name, type);
        error->one(FLERR, str);
    }

    add_atom();
    Atom *curAtom = atom[natom - 1];

    curAtom->serial = a->serial;

    strcpy(curAtom->name, a->name);
    strcpy(curAtom->type, a->type);
    strcpy(curAtom->element, a->element);

    curAtom->x = a->x;
    curAtom->y = a->y;
    curAtom->z = a->z;
    curAtom->o = a->o;
    curAtom->b = a->b;

    curAtom->fixed = a->fixed;
    curAtom->backbone = a->backbone;

    return (natom - 1);
}

/**
 * Delete an atom from the group
 *
 * @param   id the atoms internal ID.
 */
int Group::delete_atom(int id) {

    /// Find the atom with the specified index
    int iatom = find_atom(id);

    /// Delete the atom
    if (iatom < 0)
       return error->all(FLERR, "Can't find atom to delete");

    delete atom[iatom];

    /// Update the array of  atoms to reflect the deletion
    for (int i = iatom + 1; i < natom; i++)
        atom[i - 1] = atom[i];

    natom--;

    return CCB_OK;

}

/**
 * Delete an atom from the group
 *
 * @param   id the atoms internal ID.
 */
int Group::delete_atom_name(const char *name) {

    /// Find the atom with the specified index
    int iatom = find_atom_name(name);

    /// Delete the atom
    if (iatom < 0)
         return error->all(FLERR, "Can't find  atom to delete");

    delete atom[iatom];

    /// Update the array of  atoms to reflect the deletion
    for (int i = iatom + 1; i < natom; i++)
        atom[i - 1] = atom[i];

    natom--;

    return CCB_OK;

}

/**
 * Find an atom by ID
 *
 * @param   id internal ID
 * @return  the identified atom's id.
 */
int Group::find_atom(int id) {
    int iatom;

    for (iatom = 0; iatom < natom; iatom++)
        if (id == atom[iatom]->id)
            break;
    if (iatom == natom)
        return -1;
    return iatom;
}

int Group::find_atom_name(const char *name) {

    int iatom;

    for (iatom = 0; iatom < natom; iatom++)
        if (strcmp(name, atom[iatom]->name) == 0)
            break;
    if (iatom == natom)
        return -1;
    return iatom;
}

double Group::memory_usage() {

    // Tabulate a groups memory usage

    double bytes = natom * sizeof(Atom);
    bytes += maxatom * sizeof(Atom *);

    return bytes;
}
