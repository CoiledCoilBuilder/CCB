/**
 * @file   domain.cpp
 * @author Chris Von Bargen <cvb@magrathea.chem.upenn.edu>
 * @date   Nov 17 2011
 *
 * @brief  Domain, storage of the entire molecular complex
 *
 * Class to describe a specific molecular domain.
 * Here comprised of a series of sites. There is also a series of
 * for residue/monomer types, describing connectivity, etc. of
 * allowable residues in the domain (used to then construct the
 * coordinates in the domain).
 *
 */

#include "string.h"
#include "memory.h"
#include "error.h"
#include "universe.h"
#include <omp.h>
#include "timer.h"
#include "scadsio.h"
#include "domain.h"
#include "site.h"
#include "sort.h"
#include "restype.h"
#include "rotamer_library.h"
#include "type.h"
#include "atom.h"
#include "linkage.h"
#include "bitmask.h"
#include "gid.h"

/**
 * @def SITE_DELTA
 *
 * @brief Number of sites to realloc each time we run out of space
 */
#define SITE_DELTA 256

/**
 * @def RESTYPE_DELTA
 *
 * @brief Number of residue/monomer types to realloc each time we run out of space
 */
#define RESTYPE_DELTA 64

/**
 * @def CAPTYPE_DELTA
 *
 * @brief Number of capping types to realloc each time we run out of space
 */
#define CAPTYPE_DELTA 16

using namespace SCADS_NS;

Domain::Domain(SCADS *scads) :
    Pointers(scads) {

    // Initialize variables
    nsite = maxsite = 0;
    nsite_iter = 0;

    site = NULL;
    restype = NULL;
    captype = NULL;
    nrestype = maxrestype = 0;
    ncaptype = maxcaptype = 0;

    autoAngle = 0;
    autoDihedral = 0;
    paratype = new Restype(scads, 0);

    // Instance for managing atom global IDs
    gid = new Gid(scads);

    // Instance for managing allowed monomer types at sites
    type = new Type(scads);

    S = B = U = T = F = 0.0;
}

/**
 * Domain Deconstructor
 *
 * Delete all sites and residue types
 */
Domain::~Domain() {

    /// Free up ResTypes
    while (nrestype)
        delete_restype(restype[0]->id);
    memory->sfree(restype);

    /// Free up CapTypes
    while (ncaptype)
        delete_captype(captype[0]->id);
    memory->sfree(captype);

    delete paratype;

    /// Free up Sites
    while (nsite)
        delete_site(site[0]->id);
    memory->sfree(site);

    /// Delete the GID manager
    delete gid;

    /// Delete Type data
    delete type;
}

/**
 * Remove all sites in the domain
 */

void Domain::reset() {

    // delete all the sites
    while (nsite)
        delete_site(site[0]->id);
}

/**
 * Add empty site to the domain
 */
int Domain::add_site() {

    /// If we don't have enough space to add sites, realloc atom to make room.
    if (nsite == maxsite) {
        maxsite += SITE_DELTA;
        site = (Site **) memory->srealloc(site, maxsite * sizeof(Site *), "domain:site");
    }

    site[nsite] = new Site(scads, nsite_iter);

    nsite++;
    nsite_iter++;

    return (nsite - 1);
}

/**
 * @brief add a site to the domain using the copy constructor
 *
 * @param s site instance
 *
 * @return index of added site in domain's site array
 */
int Domain::add_site(const Site &s) {

    /// If we don't have enough space to add sites, realloc atom to make room.
    if (nsite == maxsite) {
        maxsite += SITE_DELTA;
        site = (Site **) memory->srealloc(site, maxsite * sizeof(Site *), "domain:site");
    }

    site[nsite] = new Site(s);
    site[nsite]->id = nsite_iter++;

    nsite++;
    return (nsite - 1);
}

/**
 * Add an empty site to the domain, set resid, chain
 */
int Domain::add_site(unsigned int resid, const char *chain) {

    //Check to see if this site already exists
    int isite = find_site(resid, chain);

    if (isite > 0)
        error->one(FLERR, "Site already exists, cannot add this site");

    isite = domain->add_site();

    domain->site[isite]->resid = resid;
    strcpy(domain->site[isite]->chain, chain);

    //strcpy(domain->site[isite]->seg, seg);
    domain->site[isite]->fixed_atoms->site = domain->site[isite];
    //strcpy(domain->site[isite]->fixed_atoms->type, res); //Set fixed_atoms type to wildtype
    sprintf(domain->site[isite]->fixed_atoms->name, "%s", "FIX"); //Set fixed_atoms name

    return isite;
}

/**
 * Delete a site from the domain
 *
 * @param   id the sites internal ID.
 */
void Domain::delete_site(int id) {
    /// Find the site with the specified index
    int isite = find_site(id);

    /// Delete the site
    if (isite < 0)
        error->one(FLERR, "Can't find site to delete");
    delete site[isite];

    /// Update the array of sites to reflect the deletion
    for (int i = isite + 1; i < nsite; i++)
        site[i - 1] = site[i];

    nsite--;
}

/**
 * Find a site by ID
 *
 * @param   id internal ID
 * @return  the identified site id.
 */
int Domain::find_site(int id) {
    int isite;

    for (isite = 0; isite < nsite; isite++)
        if (id == site[isite]->id)
            break;
    if (isite == nsite)
        return -1;
    return isite;
}

/**
 * Find a site by resid and chain name
 *
 * @param   id internal ID
 * @return  the identified site id.
 */
int Domain::find_site(unsigned int resid, const char *chain) {
    int isite;

    for (isite = 0; isite < nsite; isite++)
        if (resid == site[isite]->resid && strcmp(site[isite]->chain, chain) == 0)
            break;
    if (isite == nsite)
        return -1;
    return isite;
}

/**
 * Add a residue/monomer type to the domain
 */
int Domain::add_restype() {

    /// If we don't have enough space to add restype, realloc atom to make room.
    if (nrestype == maxrestype) {
        maxrestype += RESTYPE_DELTA;
        restype = (Restype **) memory->srealloc(restype, maxrestype * sizeof(Restype *), "domain:restype");
    }

    restype[nrestype] = new Restype(scads, nrestype);
    nrestype++;

    return (nrestype - 1);
}

/**
 * Delete a residue/monomer type from the domain
 *
 * @param   id the restype internal ID.
 */
void Domain::delete_restype(int id) {
    /// Find the restype with the specified index
    int irestype = find_restype(id);

    /// Delete the restype
    if (irestype < 0)
        error->all("Can't find restype to delete");
    delete restype[irestype];

    /// Update the array of restypes to reflect the deletion
    for (int i = irestype + 1; i < nrestype; i++)
        restype[i - 1] = restype[i];

    nrestype--;
}

/**
 * Find a residue/monomer type by ID
 *
 * @param   id internal ID
 * @return  the identified restype id.
 */
int Domain::find_restype(int id) {
    int irestype;

    for (irestype = 0; irestype < nrestype; irestype++)
        if (id == restype[irestype]->id)
            break;
    if (irestype == nrestype)
        return -1;
    return irestype;
}

/**
 * Find a residue/monomer type by name
 *
 * @param   name
 * @return  the identified restype id.
 */
int Domain::find_restype_name(const char *name) {
    int irestype;

    for (irestype = 0; irestype < nrestype; irestype++)
        if (!strcmp(name, restype[irestype]->name))
            break;

    if (irestype == nrestype)
        return -1;
    return irestype;
}

void Domain::clone_restype(const char *name, Restype *&clone) {

    int ires = find_restype_name(name);
    if (ires < 0) {
        char str[256];
        sprintf(str, "Restype %s not loaded into domain", name);
        error->one(FLERR, str);
    }

    //Copy Group variables
    clone->site = restype[ires]->site;
    strcpy(clone->name, restype[ires]->name);
    strcpy(clone->type, restype[ires]->type);
    clone->totalcharge = restype[ires]->totalcharge;
    clone->probability = restype[ires]->probability;
    for (int i = 0; i < restype[ires]->natom; i++)
        clone->add_atom(restype[ires]->atom[i]);

    //Copy Restype ICs
    for (int i = 0; i < restype[ires]->nbond; i++)
        clone->add_bond(restype[ires]->bond[i]);
    for (int i = 0; i < restype[ires]->nangle; i++)
        clone->add_angle(restype[ires]->angle[i]);
    for (int i = 0; i < restype[ires]->ndihedral; i++)
        clone->add_dihedral(restype[ires]->dihedral[i]);
    for (int i = 0; i < restype[ires]->ncap_delete; i++)
        clone->add_cap_delete(restype[ires]->cap_delete[i]);
    for (int i = 0; i < restype[ires]->nic; i++)
        clone->add_IC(restype[ires]->ic[i]);
    clone->init();
}

/**
 * Add a cap type to the domain
 */
int Domain::add_captype() {

    /// If we don't have enough space to add captype, realloc atom to make room.
    if (ncaptype == maxcaptype) {
        maxcaptype += CAPTYPE_DELTA;
        captype = (Restype **) memory->srealloc(captype, maxcaptype * sizeof(Restype *), "domain:captype");
    }

    captype[ncaptype] = new Restype(scads, ncaptype);
    ncaptype++;

    return (ncaptype - 1);
}

/**
 * Delete a cap type from the domain
 *
 * @param   id the captype internal ID.
 */
void Domain::delete_captype(int id) {
    /// Find the captype with the specified index
    int icaptype = find_captype(id);

    /// Delete the captype
    if (icaptype < 0)
        error->one("Can't find captype to delete");
    delete captype[icaptype];

    /// Update the array of captypes to reflect the deletion
    for (int i = icaptype + 1; i < ncaptype; i++)
        captype[i - 1] = captype[i];

    ncaptype--;
}

/**
 * Find a cap type by ID
 *
 * @param   id internal ID
 * @return  the identified captype id.
 */
int Domain::find_captype(int id) {
    int icaptype;

    for (icaptype = 0; icaptype < ncaptype; icaptype++)
        if (id == captype[icaptype]->id)
            break;
    if (icaptype == ncaptype)
        return -1;
    return icaptype;
}

/**
 * Find a cap type by name
 *
 * @param   name
 * @return  the identified captype id.
 */
int Domain::find_captype_name(const char *name) {
    int icaptype;

    for (icaptype = 0; icaptype < ncaptype; icaptype++)
        if (!strcmp(name, captype[icaptype]->name))
            break;

    if (icaptype == ncaptype)
        return -1;
    return icaptype;
}

/**
 * Create a new residue/monomer type from an existing residue/monomer and
 * a cap type. Creates a specific type with cap in place.
 *
 * @param       residue type to use
 * @param       cap type to use
 */
int Domain::create_capped_restype(const char *resi, const char *cap) {

    int ires, icap, iatom;

    //Find the residue and cap to use
    ires = find_restype_name(resi);
    if (ires < 0) {
        char str[128];
        sprintf(str, "Cannot create RESICAP, this RESI: %s does not exist", resi);
        error->one(FLERR, str);
    }
    icap = find_captype_name(cap);
    if (icap < 0) {
        char str[128];
        sprintf(str, "Cannot create RESICAP, this CAP: %s does not exist", cap);
        error->one(FLERR, str);
    }

    //Create and clone the restype, assigning ires to the new residue
    ires = add_restype();
    clone_restype(resi, restype[ires]);

    //Mergable group variables
    sprintf(restype[ires]->name, "%s%s", resi, cap);
    sprintf(restype[ires]->type, "%s%s", resi, cap);
    restype[ires]->totalcharge = restype[ires]->totalcharge + restype[ires]->totalcharge;

    //Delete atoms/bonds/etc based on each delete_atom in the cap
    for (int idel = 0; idel < captype[icap]->ncap_delete; idel++)
        restype[ires]->delete_atom_name(captype[icap]->cap_delete[idel]);

    //Now add all the captype pieces
    //Add all newly defined atoms
    for (int ia = 0; ia < captype[icap]->natom; ia++) {
        //Only consider atoms in the cap that are NOT temporary
        if (strcmp(captype[icap]->atom[ia]->type, "TEMP")) {
            //if this atom already exists from RESI, then overwrite it, otherwise add it
            iatom = restype[ires]->find_atom_name(captype[icap]->atom[ia]->name);
            if (iatom >= 0)
                restype[ires]->overwrite_atom(captype[icap]->atom[ia]);
            else
                restype[ires]->add_atom(captype[icap]->atom[ia]);
        }
    }

    //Add all resulting bonds
    for (int i = 0; i < captype[icap]->nbond; i++)
        restype[ires]->add_bond(captype[icap]->bond[i]);

    //Add all resulting angles
    for (int i = 0; i < captype[icap]->nangle; i++)
        restype[ires]->add_angle(captype[icap]->angle[i]);

    //Add all resulting dihedrals
    for (int i = 0; i < captype[icap]->ndihedral; i++)
        restype[ires]->add_dihedral(captype[icap]->dihedral[i]);

    //Copy all ICs exist
    for (int i = 0; i < captype[icap]->nic; i++) {
        int iic = restype[ires]->find_IC(captype[icap]->ic[i]->a);
        if (iic >= 0)
            restype[ires]->delete_IC(captype[icap]->ic[i]->a); //If found, delete and use the new def
        restype[ires]->add_IC(captype[icap]->ic[i]);
    }

    //Delete any TEMP atoms that are not interresidue (at some point, figure out how to delete interresidue?)
    for (int ia = 0; ia < restype[ires]->natom; ia++) {
        if (!strcmp(restype[ires]->atom[ia]->type, "TEMP") && !restype[ires]->atom[ia]->interresidue_atom) {
            restype[ires]->delete_atom_name(restype[ires]->atom[ia]->name);
            ia--;
        }
    }

    return ires;
}

/**
 * For a given group of atoms, set each atom's type according to names
 * assigned in the pdb
 *
 * @param       group to set atom types
 */
void Domain::set_type(Group* g) {

    int i, ires, iatom;

    for (i = 0, ires = -1; i < nrestype; i++) {
        if (!strcmp(g->type, restype[i]->type)) {
            ires = i;
            break;
        }
    }
    if (ires < 0) {
        char str[128];
        sprintf(str, "Domain:set_type: Restype %s not found", g->type);
        error->one(FLERR, str);
    }

    for (i = 0; i < g->natom; i++) {
        iatom = restype[ires]->find_atom_name(g->atom[i]->name);
        if (iatom < 0) {
            fprintf(screen, "%s %s\n", restype[ires]->type, g->atom[i]->name);
            error->one(FLERR, "Domain:set_type: atom name not found");
        }

        strcpy(g->atom[i]->type, restype[ires]->atom[iatom]->type);
        g->atom[i]->charge = restype[ires]->atom[iatom]->charge;
    }
}

/**
 * For a given group of atoms, set each atom's parameters according to types found
 * within paratype (i.e. mass, element, sigma, epsilon, etc.)
 *
 * @param       group to set parameters
 */
void Domain::set_params(Group* g) {

    int iatom;

    for (int i = 0; i < g->natom; i++) {

        //If an interresidue atom describing connectivity, ignore setting parameters
        if (g->atom[i]->interresidue_atom)
            continue;

        iatom = paratype->find_atom_name(g->atom[i]->type);
        if (iatom < 0)
            error->one(FLERR, "Domain:set_params: parameter set not found for this atom type");
        g->atom[i]->mass = paratype->atom[iatom]->mass;
        strcpy(g->atom[i]->element, paratype->atom[iatom]->element);
        g->atom[i]->rmin = paratype->atom[iatom]->rmin;
        g->atom[i]->epsilon = paratype->atom[iatom]->epsilon;
        g->atom[i]->rmin14 = paratype->atom[iatom]->rmin14;
        g->atom[i]->epsilon14 = paratype->atom[iatom]->epsilon14;
    }

    //Assure that atoms have a positive mass and radius
    g->check_parameters();
}

int Domain::get_chains(char**& chain_list) {

    //Get all the resids of this chain
    Site **site_list = NULL;
    site_list = (Site **) memory->srealloc(site_list, nsite * sizeof(Site *), "Domain::get_chains::site_list");

    // Get all sites
    for (int i = 0; i < nsite; i++)
        site_list[i] = site[i];

    //Sort them
    Sort<Site> sort(scads);
    int nchain_list;
    sort.unique(site_list, 0, nsite, sort.compare_site_chain, nchain_list);

    //Determine the first and last resid of this chain
    for (int i = 0; i < nchain_list; i++) {
        int n = strlen(site_list[i]->chain) + 1;
        chain_list[i] = new char[n];
        strcpy(chain_list[i], site_list[i]->chain);
    }

    //for (int i = 0; i < nchain_list; i++)
    //  fprintf(screen, "Chain: %s\n", chain_list[i]);

    memory->sfree(site_list);

    return nchain_list;
}

int Domain::get_resids(const char *chain, int*& resid_list) {

    //Get all the resids of this chain
    Site **site_list = NULL;
    site_list = (Site **) memory->srealloc(site_list, nsite * sizeof(Site *), "Domain::get_resids::site_list");

    // Get all sites belonging to the specified chain
    int n = 0;
    for (int i = 0; i < nsite; i++)
        if (!strcmp(site[i]->chain, chain))
            site_list[n++] = site[i];

    //Sort them
    Sort<Site> sort(scads);
    int nresid_list = 0;
    sort.unique(site_list, 0, n, sort.compare_site_resid, nresid_list);

    //Determine the first and last resid of this chain
    for (int i = 0; i < nresid_list; i++)
        resid_list[i] = site_list[i]->resid;

    //    for (int i = 0; i < nresid_list; i++)
    //    fprintf(screen, "Chain: %s Resid: %d\n", chain, resid_list[i]);

    memory->sfree(site_list);

    return nresid_list;
}

void Domain::load_capped_restypes(const char *chain) {

    char rescap_name[15];
    char cap_name[15];
    TypeCap* tcap = NULL;
    TypeResid* tres = NULL;

    int* resid_list = NULL;
    memory->create(resid_list, nsite, "Domain::get_resids");
    int nresid_list = get_resids(chain, resid_list);
    int first_resid = resid_list[0];
    int last_resid = resid_list[nresid_list - 1];
    memory->sfree(resid_list);

    //Check continuity
    //?

    //Get the caps for this chain if they have been explicitly defined
    type->get_chain_cap(chain, tcap);

    //Get the allowed types for the FIRST resids in this chain
    type->get_allowed(first_resid, chain, tres);
    if (tres) {

        //For every type available, create a capped residue of that type
        for (int itype = 0; itype < tres->ntype; itype++) {

            int ires = find_restype_name(tres->type[itype]);
            if (ires < 0)
                error->one(FLERR, "Cannot find this type in the loaded restypes");

            //Determine the cap to use
            if (tcap && strcmp(tcap->first_cap_type, ""))
                strcpy(cap_name, tcap->first_cap_type);
            else
                strcpy(cap_name, restype[ires]->first_cap);

            //Ignore NONE capping
            if (!strcmp(cap_name, "NONE"))
                continue;

            //If RESICAP is not already defined, create it
            sprintf(rescap_name, "%s%s", tres->type[itype], cap_name);
            if (find_restype_name(rescap_name) < 0) {
                int irescap = create_capped_restype(tres->type[itype], cap_name);
                //Delete atoms pointing backwards in this restype
                restype[irescap]->delete_interresidue_atoms(-1);
            }

            //fprintf(screen, "FIRST %d:%s has %s\n", first_resid, chain, rescap_name);

        }
    }

    //Get the allowed types for the LAST resids in this chain
    type->get_allowed(last_resid, chain, tres);
    if (tres) {

        //For every type available, create a capped residue of that type
        for (int itype = 0; itype < tres->ntype; itype++) {

            int ires = find_restype_name(tres->type[itype]);
            if (ires < 0)
                error->one(FLERR, "Cannot find this type in the loaded restypes");

            //Determine the cap to use
            if (tcap && strcmp(tcap->last_cap_type, ""))
                strcpy(cap_name, tcap->last_cap_type);
            else
                strcpy(cap_name, restype[ires]->last_cap);

            //Ignore NONE capping
            if (!strcmp(cap_name, "NONE"))
                continue;

            //If RESICAP is not already defined, create it
            sprintf(rescap_name, "%s%s", tres->type[itype], cap_name);
            if (find_restype_name(rescap_name) < 0) {
                int irescap = create_capped_restype(tres->type[itype], cap_name);
                //Delete atoms pointing forwards in this restype
                restype[irescap]->delete_interresidue_atoms(1);
            }

            //fprintf(screen, "LAST %d:%s has %s\n", last_resid, chain, rescap_name);

        }
    }

}

void Domain::get_site_restype(const char *chain, int resid, const char *this_type, class Restype *&rescap) {

    char rescap_name[15];
    char cap_name[15];
    TypeCap* tcap = NULL;

    int* resid_list = NULL;
    memory->create(resid_list, nsite, "Domain::get_resids");
    int nresid_list = get_resids(chain, resid_list);
    int first_resid = resid_list[0];
    int last_resid = resid_list[nresid_list - 1];
    memory->sfree(resid_list);

    int ires = find_restype_name(this_type);
    if (ires < 0) {
        char str[128];
        sprintf(str, "Unable to find restype %s", this_type);
        error->one(FLERR, str);
    }

    if (resid == first_resid) {
        //Get the caps for this chain if they have been explicitly defined
        type->get_chain_cap(chain, tcap);

        //Determine the cap to use
        if (tcap && strcmp(tcap->first_cap_type, ""))
            strcpy(cap_name, tcap->first_cap_type);
        else
            strcpy(cap_name, restype[ires]->first_cap);

        //Ignore NONE capping
        if (!strcmp(cap_name, "NONE")) {
            rescap = restype[ires];
            return;
        }

        //If RESICAP is not already defined, create it
        sprintf(rescap_name, "%s%s", this_type, cap_name);
        ires = find_restype_name(rescap_name);
        if (ires < 0) {
            char str[128];
            sprintf(str, "Unable to find restype %s", rescap_name);
            error->one(FLERR, str);
        }
        rescap = restype[ires];
        return;

    } else if (resid == last_resid) {
        //Get the caps for this chain if they have been explicitly defined
        type->get_chain_cap(chain, tcap);

        //Determine the cap to use
        if (tcap && strcmp(tcap->last_cap_type, ""))
            strcpy(cap_name, tcap->last_cap_type);
        else
            strcpy(cap_name, restype[ires]->last_cap);

        //Ignore NONE capping
        if (!strcmp(cap_name, "NONE")) {
            rescap = restype[ires];
            return;
        }

        //If RESICAP is not already defined, create it
        sprintf(rescap_name, "%s%s", this_type, cap_name);
        ires = find_restype_name(rescap_name);
        if (ires < 0) {
            char str[128];
            sprintf(str, "Unable to find restype %s", rescap_name);
            error->one(FLERR, str);
        }
        rescap = restype[ires];
        return;

    } else
        rescap = restype[ires];

}

void Domain::load_domain() {

    //Initial structure/topology/parameter/rotamer library checks

    //Load all capped residue types
    char** chain_list = NULL;
    chain_list = (char **) memory->srealloc(chain_list, nsite * sizeof(char *), "Domain::load_domain::chain_list");

    int nchain = get_chains(chain_list);
    for (int i = 0; i < nchain; i++)
        load_capped_restypes(chain_list[i]);

    ///Initialize all restypes
    for (int i = 0; i < nrestype; i++) {
        if (autoAngle)
            restype[i]->generate_angles();
        if (autoDihedral)
            restype[i]->generate_dihedrals();
        restype[i]->init();

        //restype[i]->print_all();
    }

    // Cleanup
    for (int i = 0; i< nchain; i++)
        delete [] chain_list[i];
    memory->sfree(chain_list);



}

/**
 * Build all structures in the domain.
 * Enumerate through all sites, then all allowable residue types,
 * then all allowable rotamers. Build each rotamer and add it to the site.
 *
 * @param mask the bitmask name of the backbone atoms corresponding to the sites to build
 */

void Domain::build_domain(const char *maskname) {

    // Get the bitmask
    int mask = bitmask->find_mask(maskname);

    if (mask < 0) {
        char str[128];
        sprintf(str, "Domain: build_domain: Bitmask name %s not found", maskname);
        error->one(FLERR, str);
    }

    if (universe->me == 0)
        fprintf(screen, "Building domain...\n");

    timer->barrier_start(TIME_BUILD);

    load_domain();

    // Build all the sites in the domain
    // Only build if site bitmask matches provided bitmask
    int rot_total = 0;
    for (int i = 0; i < nsite; i++) {
        if (site[i]->mask & mask)
            rot_total += site[i]->build_site();
    }

    // Make all the atoms in the domain global (required for atomselections)
    make_global(mask);

    timer->barrier_stop(TIME_BUILD);

    if (error->verbosity_level >= 3) {
        fprintf(screen, "Build: Added a total of %d rotamers across %d sites\n", rot_total, nsite);
        fprintf(screen, "Build: %-.3f minutes\n", timer->array[TIME_BUILD] / 60.0);
    }

}

// For all sites in the domain, determines
// the most probable rotamer at that site
void Domain::get_most_probable() {

    for (int i = 0; i < nsite; i++)
        site[i]->get_most_probable();

}

/**
 * @brief Make all the atoms in the domain global
 *
 * Provides the atom with a domain-unique identifier
 *
 * This is called after the domain has been constructed
 * via the build command. This is required for atomselection
 * routines to work properly.
 */

void Domain::make_global(int mask) {

    for (int i = 0; i < nsite; i++) {
        if (site[i]->mask & mask) {
            site[i]->make_global();
        }
    }

}

/**
 * Calculate the memory usage of the domain
 */

double Domain::memory_usage() {

    double bytes = nsite * sizeof(Site);
    bytes += maxsite * sizeof(Site *);

    // The size of the groups and atoms in those
    // groups

    for (int i = 0; i < nsite; i++) {

        // Memory usage for the site
        bytes += site[i]->memory_usage();

        // Memory usage for the fixed_atoms group
        bytes += site[i]->fixed_atoms->memory_usage();

        // Memory usage for all the rotamers
        for (int j = 0; j < site[i]->nrotamer; j++)
            bytes += site[i]->rotamer[j]->memory_usage();

    }

    // Global Hashtable
    bytes += gid->memory_usage();

    // Types
    bytes += type->memory_usage();

    return bytes;
}
