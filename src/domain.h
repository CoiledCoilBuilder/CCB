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

