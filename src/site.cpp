/**
 * @file   site.cpp
 * @author Chris Von Bargen <cvb@magrathea.chem.upenn.edu>
 * @date   Nov 14 2011
 *
 * @brief  Site, describing a location in the domain
 *
 * Class to describe a specific site in the domain.
 * Here contains descriptive fields (name, resid, etc.) as well as
 * a static group of fixed common atoms and a collection of corresponding groups (rotamers)
 *
 */

#include "string.h"
#include "memory.h"
#include "error.h"
#include "domain.h"
#include "site.h"
#include "group.h"
#include "type.h"
#include "atom.h"
#include "scadsio.h"
#include "sort.h"
#include "rotamer_library.h"
#include "type.h"
#include "universe.h"
#include "restype.h"
#include "linkage.h"

/**
 * @def ROTAMER_DELTA
 *
 * @brief Number of rotamers to reallocate each time we run out of space
 */
#define ROTAMER_DELTA 50

/**
 * @def TYPES_PER_GROUP
 * @brief Maximum number of types per group
 *
 */
#define TYPES_PER_GROUP 50

using namespace SCADS_NS;

Site::Site(SCADS *scads, int i) :
    Pointers(scads) {

    // Initialize variables
    id = i;
    resid = 0;
    for (int j = 0; j < 3; j++){
       com_cart[j] = 0.0;
       com_frac[j] = 0.0;
    }
    strcpy(chain, "");
    strcpy(seg, "");

    fixed_atoms = new Group(scads, -1);
    fixed_atoms->site = this;
    fixed_atoms->probability = -1.0;
    ngroup_iter = 1;

    nrotamer = maxrotamer = 0;
    rotamer = NULL;
    most_probable = NULL;

    // The site bitmask
    mask = 1;
}

/**
 * Domain Deconstructor
 *
 * Delete fixed_atoms and all rotamers
 */
Site::~Site() {

    /// Free up Backbone atoms
    delete fixed_atoms;

    /// Free up Rotamers
    while (nrotamer)
        delete_rotamer(rotamer[0]->id);
    memory->sfree(rotamer);
}

Site::Site(const Site &s) : Pointers(s) {

    rotamer = NULL;
    most_probable = NULL;

    id = s.id;
    resid = s.resid;
    nrotamer = s.nrotamer;
    maxrotamer = s.maxrotamer;
    strcpy(chain, s.chain);
    strcpy(seg, s.seg);
    mask = s.mask;
    ngroup_iter = s.ngroup_iter;

    // Copy instance of fixed-atoms;
    fixed_atoms = new Group(*(s.fixed_atoms));

    // Allocate memory to store copies of groups
    rotamer = (Group **) memory->srealloc(rotamer, maxrotamer * sizeof(Group *), "site:rotamer");

    // Copy instances, not pointers of groups
    for (int i = 0; i < nrotamer; i++)
        rotamer[i] = new Group(*(s.rotamer[i]));

    // Point to the most probable if assigned
    if (s.most_probable) {
        int irotamer = find_rotamer(s.most_probable->id);
        most_probable = rotamer[irotamer];
    }

    // Set the site pointers for the groups & atoms
    for (int i = 0; i < nrotamer; i++) {
        rotamer[i]->site = this;

        for (int j = 0; j < rotamer[i]->natom; j++)
            rotamer[i]->atom[j]->site = this;
    }

    fixed_atoms->site = this;
    for (int i = 0; i < fixed_atoms->natom; i++)
        fixed_atoms->atom[i]->site = this;


}

Site& Site::operator= (Site const& s) {

    // Don't create if self-assignment
    if (this == &s) return *this;

    // Base operator
    Pointers::operator=(s);

    //Cleanup instance being copied to
    for (int i = 0; i < nrotamer; i++)
        delete rotamer[i];
    memory->sfree(rotamer);
    rotamer = NULL;

    id = s.id;
    resid = s.resid;
    nrotamer = s.nrotamer;
    maxrotamer = s.maxrotamer;
    strcpy(chain, s.chain);
    strcpy(seg, s.seg);
    mask = s.mask;
    ngroup_iter = s.ngroup_iter;

    // Copy instance of fixed_atoms
    //fixed_atoms = new Group(*(s.fixed_atoms));
    *fixed_atoms = *(s.fixed_atoms);

    // Allocate memory to store copies of groups
    rotamer = (Group **) memory->srealloc(rotamer, maxrotamer * sizeof(Group *), "site:rotamer");

    // Copy instances, not pointers of groups
    for (int i = 0; i < nrotamer; i++)
        rotamer[i] = new Group(*(s.rotamer[i]));

    // Point to the most probable if assigned
    if (s.most_probable != NULL) {
        int irotamer = find_rotamer(s.most_probable->id);
        most_probable = rotamer[irotamer];
    }

    // Set the site pointers for the groups & atoms
    for (int i = 0; i < nrotamer; i++) {
        rotamer[i]->site = this;

        for (int j = 0; j < rotamer[i]->natom; j++)
            rotamer[i]->atom[j]->site = this;
    }

    fixed_atoms->site = this;
    for (int i = 0; i < fixed_atoms->natom; i++)
        fixed_atoms->atom[i]->site = this;


    return *this;
}

/**
 * Add a rotamer group to the site
 */
int Site::add_rotamer() {

    /// If we don't have enough space to add rotamer, realloc rotamer to make room.
    if (nrotamer == maxrotamer) {
        maxrotamer += ROTAMER_DELTA;
        rotamer = (Group **) memory->srealloc(rotamer, maxrotamer * sizeof(Group *), "site:rotamer");
    }

    rotamer[nrotamer] = new Group(scads, ngroup_iter);

    nrotamer++;
    ngroup_iter++;

    return (nrotamer - 1);
}

void Site::put_com_cart(double v[3]){
   com_cart[0] = v[0];
   com_cart[1] = v[1];
   com_cart[2] = v[2];
}

void Site::put_com_frac(double v[3]){
   com_frac[0] = v[0];
   com_frac[1] = v[1];
   com_frac[2] = v[2];
}

/**
 * @brief add a rotamer to this site using the copy constructor
 *
 * @param g group instance
 *
 * @return index to group in rotamer array in site
 */
int Site::add_rotamer(Group &g) {

    /// If we don't have enough space to add rotamer, realloc rotamer to make room.
    if (nrotamer == maxrotamer) {
        maxrotamer += ROTAMER_DELTA;
        rotamer = (Group **) memory->srealloc(rotamer, maxrotamer * sizeof(Group *), "site:rotamer");
    }

    rotamer[nrotamer] = new Group(g);
    rotamer[nrotamer]->id = ngroup_iter++;

    nrotamer++;
    return (nrotamer - 1);
}


/**
 * Delete a rotamer from the site
 *
 * @param   id the romater's internal ID.
 */

void Site::delete_rotamer(int id) {

    /// Find the rotamer with the specified index
    int irotamer = find_rotamer(id);

    /// Delete the rotamer
    if (irotamer < 0)
        error->all(FLERR, "Can't find rotamer to delete");
    delete rotamer[irotamer];

    /// Update the array of rotamer to reflect the deletion
    for (int i = irotamer + 1; i < nrotamer; i++)
        rotamer[i - 1] = rotamer[i];

    nrotamer--;
}

void Site::delete_all_rotamers() {

    /// Delete the rotamers
    while (nrotamer > 0)
        delete_rotamer(rotamer[0]->id);
}

/**
 * Find a rotamer by ID
 *
 * @param   id internal ID
 * @return  the identified rotamer's id.
 */

int Site::find_rotamer(int id) {
    int irotamer;

    for (irotamer = 0; irotamer < nrotamer; irotamer++)
        if (id == rotamer[irotamer]->id)
            break;
    if (irotamer == nrotamer)
        return -1;
    return irotamer;
}

/**
 * Add a rotamer to this site, and copy the values of that rotamer into this new rotamer
 */

int Site::add_rotamer(Group *rot) {

    int iatom;

    add_rotamer();
    Group *newRot = rotamer[nrotamer - 1];

    // Set the site that the group belongs to
    newRot->site = this;

    sprintf(newRot->name, "%s_%d", rot->name, newRot->id);
    strcpy(newRot->type, rot->type);
    newRot->totalcharge = rot->totalcharge;

    for (int i = 0; i < rot->natom; i++) {

        //If this is an interresidue atom specifying connectivity, ignore it
        if (rot->atom[i]->interresidue_atom)
            continue;

        if (rot->atom[i]->build_flag != 1) {
            fprintf(screen, "(%s) %s in %s at %d:%s (FLAG %d)\n", rot->type, rot->atom[i]->name, newRot->name, resid, chain, rot->atom[i]->build_flag);
            error->one(FLERR, "This atom does not have the correct coordinates. Cannot add to rotameric state.");
        }

        iatom = newRot->add_atom(rot->atom[i]);

        // Set the group and site that the atom belongs to.
        newRot->atom[iatom]->group = newRot;
        newRot->atom[iatom]->site = this;
    }

    //fprintf(screen, "ADD %s:%d type %s as group %d, name %s\n", chain, resid, newRot->type, newRot->id, newRot->name);

    return (nrotamer - 1);
}

/**
 * @brief identify the most probable rotamer at this site
 *
 * Determines and sets the most probable
 * rotamer at this site. This will be easier
 * than writing this each time in separate routines
 * that need the most probable rotamer and will make
 * selecting the most probable rotamer easier in atomselections
 */

void Site::get_most_probable() {

    // The sorting instance
    Sort<Group> sort(scads);

    // check to see if we have only 0 or 1 rotamers at this site
    if (nrotamer == 1) {
        most_probable = rotamer[0];
        return;
    } else if (nrotamer == 0) {
        most_probable = NULL;
        return;
    }

    Group *g1[nrotamer];
    Group *g2[nrotamer];

    for (int i = 0; i < nrotamer; i++)
        g1[i] = rotamer[i];

    // sort the group array in place by group->type.
    sort.quicksort(g1, 0, nrotamer, sort.compare_group_type);

    // Now go type-by-type and sum the probabilities
    int i = 0, j = 0;
    while (i < nrotamer) {
        g2[j] = new Group(scads, j);
        strcpy(g2[j]->type, g1[i]->type);
        g2[j]->probability = g1[i]->probability;
        i++;

        while (i < nrotamer && strcmp(g2[j]->type, g1[i]->type) == 0) {
            g2[j]->probability += g1[i]->probability;
            i++;
        }

        j++;
    }

    /* Sort g2 (sums), so we now have types sorted from
     * least probable to most probable
     */

    sort.quicksort(g2, 0, j, sort.compare_group_prob);

    /* Resort g1 by probability and
     * Run backward over g1 until we get a match
     * for the most probable type, that's the most
     * probable rotamer for that type
     */

    sort.quicksort(g1, 0, nrotamer, sort.compare_group_prob);

    for (i = nrotamer - 1; i >= 0; i--)
        if (strcmp(g1[i]->type, g2[j - 1]->type) == 0)
            break;

    // Set most probable
    most_probable = g1[i];

    for (int i = 0; i < j; i++)
        delete g2[i];
}

double Site::memory_usage() {

    double bytes = (nrotamer + 1) * sizeof(Group);
    bytes += (maxrotamer + 1) * sizeof(Group *);

    return bytes;
}

/**
 * Builds all allowed rotamers at this site given
 * that the topologies have been loaded and
 * there are rotamers or internal coordinates
 * provided for the monomers.
 *
 * @param mask the bitmask of the backbone atoms corresponding to the sites to build
 *
 * @return the number of rotamers constructed at this site
 */

int Site::build_site() {

    int irot;
    int rot_count = 0;
    int rot_total = 0;
    TypeResid* tres = NULL;
    TypeCap* tcap = NULL;
    Bin *rotBin = NULL;
    Restype *curRestype = NULL;

    // Builds all rotamers for all monomer types at this site.

    domain->set_type(fixed_atoms);
    domain->set_params(fixed_atoms);

    // If we have existing rotamers, set their parameters
    for (int i = 0; i < nrotamer; i++) {
        domain->set_type(rotamer[i]);
        domain->set_params(rotamer[i]);
    }

    // Obtain all allowed types for this site
    domain->type->get_allowed(resid, chain, tres);
    domain->type->get_chain_cap(chain, tcap);

    if (!tres)
        return 0; //There are no allowed types

    // For each allowed residue type at this site
    for (int i = 0; i < tres->ntype; i++) {

        rot_count = 0;

        // Find the restype index (either RESI or RESICAP)
        // If the site not capped, just places tres->type[i] in type_name
        Restype* r;
        domain->get_site_restype(chain, resid, tres->type[i], r);

        curRestype = new Restype(scads, 0);
        domain->clone_restype(r->type, curRestype);

        // Check all the rotamer libraries for this type
        for (int j = 0; j < scadsio->nrotamer_library; j++) {
            scadsio->rotamer_library[j]->get_bin(tres->type[i], this, rotBin);

            //Did we find some rotamers to add? Build monomers for each one...
            if (rotBin) {
                double *angles = new double[rotBin->nchi];

                // Build all the rotamers in this bin
                unsigned int k = 0;
                while (scadsio->rotamer_library[j]->get_rotamer(this, rotBin, k++, angles)) {

                    // Create connectivity entries for each chi angle
                    for (unsigned int l = 0; l < rotBin->nchi; l++)
                        curRestype->set_dihedral_build_value(rotBin->chi[l]->connect[0], rotBin->chi[l]->connect[1], rotBin->chi[l]->connect[2],
                                                             rotBin->chi[l]->connect[3], angles[l]);

                    // construct the monomer and add this rotamer build to the site
                    curRestype->build_coords(fixed_atoms);
                    irot = add_rotamer(curRestype);
                    rot_count++;
                }

                delete[] angles;
            }
        }

        // If we don't have any rotamers, add one using
        // the provided internal coordiantes, print a warning
        if (rot_count == 0) {
            curRestype->build_coords(fixed_atoms);
            add_rotamer(curRestype);
            rot_count++;

            if (universe->me == 0 && error->verbosity_level > 5) {
                // Print a warning
                char str[128];
                sprintf(str, "No rotamers found for type %s at chain %s resid %d, using internal coordinates", curRestype->type, chain, resid);
                error->warning(FLERR, str);
            }
        }

        rot_total += rot_count;
        rot_count = 0;

        delete curRestype;
    }

    //Check the parameters of each rotamer
    for (int i = 0; i < nrotamer; i++)
        rotamer[i]->check_parameters();

    /**
     * Set the initial probabilities, all rotamers
     * are assumed to be equally likely
     */
    double prob = 1.0 / nrotamer;
    for (int i = 0; i < nrotamer; i++)
        rotamer[i]->probability = prob;

    return rot_total;
}

/**
 * @brief globalize all atoms in the site, make them available
 * @brief for atomselection
 *
 * Adds atoms to the global lookup "GID" table. Also assigns the atoms
 * a Global IDentifier.
 */
void Site::make_global() {

    // Make fixed-atoms global
    fixed_atoms->make_global();

    // Make all rotamers global
    for (int i = 0; i < nrotamer; i++)
        rotamer[i]->make_global();

}
