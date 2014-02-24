

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

