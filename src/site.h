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
 * @file   site.h
 * @author Chris Von Bargen <cvb@magrathea.chem.upenn.edu>
 * @date   Nov 14 2011
 *
 * @brief  Site, describing a location in the domain
 *
 * Class to describe a specific site in the domain.
 * Here contains descriptive fields (name, resid, etc.) as well as
 * a static group fixed common atoms and a collection of corresponding groups (rotamers)
 *
 */

#ifndef CCB_SITE_H
#define CCB_SITE_H

#include "pointers.h"

namespace CCB_NS {
    class Site: protected Pointers {

    public:

        // Constructor and Destructor
        Site(class CCB *, int i); /**< Site constructor */
        ~Site(); /**< Site deconstructor */
        Site(const Site &);   /**< Copy Constructor */
        Site& operator=(Site const &); /**< assignment operator */

        int id; /**< Internal indexing counter */
        unsigned int resid; /**< Site/Resid  number */
        char chain[10]; /**< Chain name */
        char seg[10]; /**< Segment Identifier */

         class Group *fixed_atoms; /**< Static site member of fixed, common atoms */
         class Group **rotamer; /**< list of site members corresponding to fixed atoms (rotamers) */
         class Group *most_probable; /**< The most probable group at this site */


        int nrotamer; /**< Total number of rotamers */

        // Functions to manage rotamers (dynamic groups);
        int add_rotamer();
        int add_rotamer(Group *res);
        int add_rotamer(Group &g); /**< add a group using the copy-constructor */
        int delete_rotamer(int id);
        int find_rotamer(int id);
        void delete_all_rotamers();

        int mask;           /** < The site mask, used for build specifications. All atoms in this site inherit the site mask */

        double memory_usage();

    private:
        bigint ngroup_iter; /** < The unique site id iterator, this should only be increased */
        int maxrotamer; /**< Maximum number of rotamers based on currently allocated space  */


    };
}

#endif

