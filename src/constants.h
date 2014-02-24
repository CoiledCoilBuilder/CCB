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
 * @file   constants.h
 * @author Chris Von Bargen <cvb@magrathea.chem.upenn.edu>
 * @date   Dec 16 2011
 * 
 * @brief  List of fundamental constants
 * http://www.ks.uiuc.edu/Training/Tutorials/science/forcefield-tutorial/forcefield-html/node5.html
 * 
 */

#ifndef CCB_CONSTANTS_H
#define CCB_CONSTANTS_H

/**
 * Pi
 */
#define PI 3.141592653589793

/**
 * Radians to Degrees (180/PI)
 */
#define RAD2DEG 57.295779513082

/**
 * Degrees to Radians (PI/180)
 */
#define DEG2RAD 0.017453292519943

/**
 * R = (Boltzmann constant * Avogadro's number)
 * (3.29762303e-27 kcal/K) * (6.02214129e23 mol^-1) = 0.001985875 kcal/mol.K
 * http://www.wolframalpha.com/input/?i=%28avagadro%27s+number%29*%28boltzmann%27s+constant%29+in+kcal%2FKelvin
 */
#define R 0.001985875

#endif

