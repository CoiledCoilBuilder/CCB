


/**
 * @file   bitmask.cpp
 * @author Chris MacDermaid <chris@cabal.nfs>
 * @date   Sun Dec 11 19:05:07 2011
 *
 * @brief  keeps track of groups of atom bitmasking
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "error.h"
#include "universe.h"
#include "memory.h"
#include "atom.h"
#include "group.h"
#include "site.h"
#include "domain.h"
#include "bitmask.h"

/**
 * @def MAX_BITMASK
 *
 * Maximum number of Bitmasking groups, this number
 * is machine dependent and can not be increased
 * on 32 bit machines or 64 bit with 32bit int
 */

#define MAX_BITMASK 32

using namespace CCB_NS;

Bitmask::Bitmask(CCB *ccb) : Pointers(ccb) {

    names = new char*[MAX_BITMASK];
    mask = new int[MAX_BITMASK];
    inversemask = new int[MAX_BITMASK];

    // Set name=null, and bitmask, invbitmask to necessary values
    for (int i = 0; i < MAX_BITMASK; i++) names[i] = NULL;
    for (int i = 0; i < MAX_BITMASK; i++) mask[i] = 1 << i;
    for (int i = 0; i < MAX_BITMASK; i++) inversemask[i] = mask[i] ^ ~0;

    // create "all" bitmask containing all atoms (mask == 1)

    char *str = (char *) "all";
    int n = strlen(str) + 1;
    names[0] = new char[n];
    strcpy(names[0],str);
    nmask = 1;

}

Bitmask::~Bitmask() {

    for (int i = 0; i < MAX_BITMASK; i++) delete [] names[i];

    delete [] names;
    delete [] mask;
    delete [] inversemask;
}

int Bitmask::find(const char *name)
{
    for (int imask = 0; imask < MAX_BITMASK; imask++)
        if (names[imask] && strcmp(name,names[imask]) == 0) return imask;

    return -1;
}

int Bitmask::find_mask(const char *name) {

    int imask = find(name);

    if (imask < 0)
        return -1;

    return mask[imask];

}

int Bitmask::find_unused()
{
    for (int imask = 0; imask < MAX_BITMASK; imask++)
        if (names[imask] == NULL) return imask;
    return -1;
}

int Bitmask::add_bitmask(const char *name) {

    // Check to see that the bitmask name is legit
    int n = strlen(name);
    for (int i = 0; i < n; i++)
        if (!isalnum(name[i]) && name[i] != '_')
            return error->one(FLERR, "bitmask: names must be alphanumeric or underscore characters");

    int imask = find(name);

    if (imask < 0) {
        if (nmask == MAX_BITMASK) return error->all(FLERR,"bitmask: Too many bitmasks");
        imask = find_unused();
        int n = strlen(name) + 1;
        names[imask] = new char[n];
        strcpy(names[imask],name);
        nmask++;
    }

    return CCB_OK;
}

int Bitmask::delete_bitmask(const char *name) {

    if (strcmp(name,"all") == 0)
        return error->one(FLERR, "bitmask: can't delete \"all\" bitmask");

    int imask = find(name);

    if (imask < 0)
        return CCB_OK;

    int bmask = mask[imask];

    // unset bit on all atoms in domain
    for (int i = 0; i < domain->nsite; i++)
        for (int j = 0; j < domain->site[i]->nrotamer; j++)
            for (int k = 0; k < domain->site[i]->rotamer[j]->natom; k++)
                domain->site[i]->rotamer[j]->atom[k]->mask &= ~bmask;

    delete [] names[imask];

    return CCB_OK;
}
