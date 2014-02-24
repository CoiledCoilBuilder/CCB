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
#include "ccbio.h"
#include "domain.h"
#include "site.h"
#include "sort.h"
#include "atom.h"
#include "bitmask.h"

/**
 * @def SITE_DELTA
 *
 * @brief Number of sites to realloc each time we run out of space
 */
#define SITE_DELTA 256

using namespace CCB_NS;

Domain::Domain(CCB *ccb) :
          Pointers(ccb), 
          site(),
          nsite(0), 
          maxsite(0), 
          nsite_iter(0)
{}

/**
 * Domain Deconstructor
 *
 * Delete all sites and residue types
 */
Domain::~Domain() {

    /// Free up Sites
    while (nsite)
        delete_site(site[0]->id);
    memory->sfree(site);
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

    site[nsite] = new Site(ccb, nsite_iter);

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
int Domain::delete_site(int id) {
    /// Find the site with the specified index
    int isite = find_site(id);

    /// Delete the site
    if (isite < 0)
        return error->one(FLERR, "Can't find site to delete");
    delete site[isite];

    /// Update the array of sites to reflect the deletion
    for (int i = isite + 1; i < nsite; i++)
        site[i - 1] = site[i];

    nsite--;

    return CCB_OK;
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

    return bytes;
}
