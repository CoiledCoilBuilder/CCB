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
 * @file   atom.h
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Mon Jun 27 16:17:52 2011
 *
 * @brief  atom based quantities header
 *
 * Here we treat each atom as an object. While this is good
 * for user manipulation and tracking, for communicating via MPI
 * it's not going to be great. We'll need to think hard about the
 * implementation of the routines for calculating things based on atoms.
 *
 * I've allowed larger res/name/type/chain/seg fields than allowed in
 * the pdb for convenience.
 */

#ifndef CCB_ATOM_H
#define CCB_ATOM_H

#include "pointers.h"

namespace CCB_NS {

    class Atom: protected Pointers {
    public:

        // Constructor and destructor
        Atom(class CCB *, int id);
        ~Atom();

        class Site *site; /**< The site this atom belongs to */
        class Group *group; /**< The group this atom belongs to */

        Atom(const Atom &); /**< Copy Constructor */
        Atom& operator=(Atom const &); /**< assignment operator */

        // Atom quantities
        int id; /**< Local Internal indexing counter */
        int serial; /**< Atom serial number from input */
        int mask; /**< Bitmasking*/

        char name[10]; /**< Atom name from input */
        char type[10]; /**< Atom type from input */
        char element[2]; /**< Atom element type from input */

        double x; /**< Atom x coordinate */
        double y; /**< Atom y coordinate */
        double z; /**< Atom z coordinate */
        double o; /**< User double field1, store occupancy */
        double b; /**< User double field2, store b-factor    */

        bool fixed; /**< Boolean to determine if the coordinates are fixed */
        bool backbone; /**< Boolean to determine if apart of the backbone */

        // Routines for manipulating atomic coordinates
        void get_xyz(double *v); /**< Return the atom coordinates in v*/
        void put_xyz(double *v); /**< Set the atom coordinates to those in v*/
        void move(const double m[4][4]); /**< apply 4x4 matrix to atoms coordinates  */
        void move(const double m[3][3]); /**< apply 3x3 matrix to atoms coordinates  */
        void moveby(const double v[3]); /**< apply translation vector to atoms */
    };

    /* +----------------------------------------------------------+  */
    /* |      COMMON ROUTINES FOR MANIPULATING ATOMIC COORDINATES |  */
    /* +----------------------------------------------------------+  */

    /**
     * Get xyz coordinates from Atom a and assign them to double[3] vector
     *
     * @param  double array
     */

    inline void Atom::get_xyz(double *v) {
        v[0] = x;
        v[1] = y;
        v[2] = z;
    }

    /**
     * Assign double[3] vector to xyz coordinates in Atom a
     *
     * @param  double array
     */

    inline void Atom::put_xyz(double *v) {
        x = v[0];
        y = v[1];
        z = v[2];
    }

    /**
     * @brief Apply 4x4 transformation matrix to atoms coordinates
     *
     * @param m 4x4 transformation matrix
     */

    inline void Atom::move(const double m[4][4]) {

        double ans[4] = { 0.0 };

        ans[0] = m[0][0] * x + m[0][1] * y + m[0][2] * z + m[0][3];
        ans[1] = m[1][0] * x + m[1][1] * y + m[1][2] * z + m[1][3];
        ans[2] = m[2][0] * x + m[2][1] * y + m[2][2] * z + m[2][3];
        ans[3] = m[3][0] * x + m[3][1] * y + m[3][2] * z + m[3][3];

        x = ans[0];
        y = ans[1];
        z = ans[2];
    }

    /**
     * @brief Apply 3x3 transformation (rotation) matrix to atoms coordinates
     *
     * @param m 3x3 rotation matrix
     */

    inline void Atom::move(const double m[3][3]) {

        double ans[3] = { 0.0 };

        ans[0] = m[0][0] * x + m[0][1] * y + m[0][2] * z;
        ans[1] = m[1][0] * x + m[1][1] * y + m[1][2] * z;
        ans[2] = m[2][0] * x + m[2][1] * y + m[2][2] * z;

        x = ans[0];
        y = ans[1];
        z = ans[2];
    }

    /**
     * @brief move the atom by vector v
     *
     * @param v vector length 3
     */

    inline void Atom::moveby(const double v[3]) {
        x += v[0];
        y += v[1];
        z += v[2];
    }
}

#endif
