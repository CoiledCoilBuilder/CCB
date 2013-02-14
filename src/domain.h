/**
 * @file   domain.h
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

#ifndef CCB_DOMAIN_H
#define CCB_DOMAIN_H

#include "pointers.h"

namespace CCB_NS {
class Domain: protected Pointers {

  public:

    // Constructor and Destructor
    Domain(class CCB *); /**< Domain constructor */
    ~Domain(); /**< Domain deconstructor */

    class Site **site; /**< list of sites */
    int nsite; /**< Total number of sites */
    int maxsite; /**< Maximum number of sites based on currently allocated space  */

    //domain management:
    void reset(); /**< clear the domain of all sites */

    // Functions to manage sites
    int add_site(); /**< add an empty site */
    int add_site(unsigned int resid, const char *chain); /**< Add a site to the domain, set resid,chain */
    int add_site(const Site &s); /**< add a site to the domain using the copy constructor */
    int delete_site(int id);
    int find_site(int id);
    int find_site(unsigned int resid, const char *chain);
    void writeDomain();

    double memory_usage(); /**< Calculate domain memory usage */

  private:
    bigint nsite_iter; /** < The unique site id iterator, this should only be increased, bigint because it remembers every atom ever created */
};
}

#endif

