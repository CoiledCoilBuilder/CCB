// -*-c++-*-

  // +------------------------------------------------------------------------------------+ 
  // |  This file is part of Coiled-Coil Builder.                                         | 
  // |                                                                                    | 
  // |  Coiled-Coil Builder is free software: you can redistribute it and/or modify       | 
  // |  it under the terms of the GNU General Public License as published by              | 
  // |  the Free Software Foundation, either version 3 of the License, or                 | 
  // |  (at your option) any later version.                                               | 
  // |                                                                                    | 
  // |  Coiled-Coil Builder is distributed in the hope that it will be useful,            | 
  // |  but WITHOUT ANY WARRANTY without even the implied warranty of                     | 
  // |  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                     | 
  // |  GNU General Public License for more details.                                      | 
  // |                                                                                    | 
  // |  You should have received a copy of the GNU General Public License                 | 
  // |  along with Coiled-Coil Builder.  If not, see <http:www.gnu.org/licenses/>.        | 
  // |                                                                                    | 
  // |   *cr                                                                              | 
  // |   *cr            (C) Copyright 1995-2013 The Board of Trustees of the              | 
  // |   *cr                        University of Pennsylvania                            | 
  // |   *cr                         All Rights Reserved                                  | 
  // |   *cr                                                                              | 
  // +------------------------------------------------------------------------------------+ 

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

