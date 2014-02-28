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
 * @file   backbone_coiledcoil.h
 * @author Chris MacDermaid <chris@cabal1.nfs>
 * @date   Jan 22 2112
 *
 * @brief Backbone style to build coiled-coils
 *
 * Routines for creating coiled coils of arbitrary
 * geometries
 *
 */

#ifdef BACKBONE_CLASS

BackboneStyle(coiledcoil,BackboneCoiledCoil)

#else

#ifndef CCB_BACKBONE_COILEDCOIL_H
#define CCB_BACKBONE_COILEDCOIL_H

#include "pointers.h"
#include "backbone.h"
#include "site.h"

/**
 * @def MAX_HELIX
 *
 * @brief maximum number of asymmetric helices
 */

#define MAX_HELIX 20

/**
 * @def MAX_RES
 *
 * @brief maximum number residues per helix
 */

#define MAX_RES 500

namespace CCB_NS {

class BackboneCoiledCoil : public Backbone {

  public:
    BackboneCoiledCoil(class CCB *, int, const char **);
    ~BackboneCoiledCoil();

    // Member Functions
    int generate();                                              /**< build coiled-coil, update coordinates in x */
    int update_domain();                                         /**< set the domain coordinates to the backbone coordianates*/
    int set_params(int argc, const char **argv, int n);          /**< set the backbone parameters before generation*/

  protected:
    virtual int init_style();                                     /**< Initialize the style (declare member variables, etc.. */
    virtual int update_style(int argc, const char **argv, int n); /**< Update the parameters and re-generate the structure */
    virtual int generate_style();                                 /**< Generate coordinates */

  private:

    // Variables that describe the backbone topology
    unsigned int natom;           /**< number of atoms = numres*numhelix*4 */
    unsigned int natomlarge;      /**< Largest number of atoms in a chain */
    unsigned int maxatom;         /**< maximum number of atoms allowed before realloc of atom vector */
    int nhelix;                   /**< number of helices per coiled-coil */
    int nreslarge;                /**< Largest number of residues in a chain */
    int nrestotal;                /**< Total number of residues in the coiled-coil */

    // Symmetric Parameters
    double pitch;                 /**< pitch of the coiled-coil */
    double phi;                   /**< initial phi angle used to build the peptide plane */
    double psi;                   /**< initial psi angle used to build the peptide plane */
    double rpr;                   /**< rise per residue, major helix frame, d */
    double omega;                 /**< major helix angular yield per residue; -2*PI*rpr/pitch */
    double omega_alpha;           /**< minor helix angular yield per residue; 2*pi/rpt */

    // Radius expansion/contraction
    double radius[MAX_RES];       /**<Radius of the coiled-coil*/
    double r0_params[4];          /**<Array of radius parameters for expansion/contraction r0_start, r0_end, res_start, res_end */

    // Asymmetric Parameters
    int nres[MAX_HELIX];          /**< number of residues per helix */
    double rotation[MAX_HELIX];   /**< rotation of the helices, for asymmetric, a list of rotations for each helix */
    double rpt[MAX_HELIX];        /**< residues per turn of the helices */
    double zoff[MAX_HELIX];       /**< z-axis displacement of helices along their axis, for asymmetric, a list of offsets for each helix */
    double z[MAX_HELIX];          /**< z-axis displacement of helices along the coiled-coil axis*/
    double square[MAX_HELIX];     /**< phi_0 offset in addition to the normal 2*pi/i placement about the superhelix */

    bool asymmetric_flag;         /**< Are the helices in the coiled-coil symmetric? **/
    bool rebuild_domain;          /**< if true, we erase the existing coiled coil when we update */

    double **pp_x;                /**< 2D-array of initial peptide-plane coordinates */
    double ***axis_x;             /**< coordinates of the minor-helical axis */
    double ***x;                  /**< 3D-array of coordinates for coiled-coil */

    //Order of output by chain
    int order[MAX_HELIX];         /**< Order of chain output, e.g. {0 3 1 2} switches {A B C D} to {A D B C} */

    // Specify Antiparallel and individual helical orientations
    bool anti_flag;               /**< antiparallel flag, specifies construction of antiparallel coiled-coil */
    int ap_order[MAX_HELIX];      /**< Specifies order of helices with respect to first helix. e.g. an up-down-up is 0 1 0 */

    Site **site;                  /**< Array of sites that this style created and can update or delete */
    unsigned int nsite;           /**< current number of sites that belong to this style */
    unsigned int maxsite;         /**< maximum number of sites before realloc is necessary */
    int add_site(Site *s);       /**< adds a site for this style to keep track of*/

    int mask;                     /**< sets the backbone atom's bitmask */

    void build_plane();             /**< build the first peptide plane */
    void align_plane(double *w);    /**< align the peptide-plane rotation vector with w (axis) */
    void helix_axis(); /**< generate the axis of the minor helix */
    void symmetry();                /**< apply symmetry operations to helix to generate coil */
    void azzero();                  /**< zero out coordinates in x array */
    int allocate();                /**< increase size of x, axis if necessary */


    void get_pp_params(double *axis0, double *axis1,
                       double *u, double *v, double *r, double &theta);             /**< Determines the screw-rotation parameters from the peptide plane */
    void crick(double *u, double rho, double *r1, double *r2);       /**< Sets the rotation angle to correspond to the crick angle */
    void next_plane(double *u, double *v, double theta);             /**< Generates the next plane given u, v, theat */
    void terminate();                                                /**< Adds the n-terminal nitrogen, rearranges coordinates */
    void terminate_asymmetric();                                     /**< Adds the n-terminal nitrogen, rearranges coordinates */
    double memory_usage();                                           /**< Calculates the memory usage of this style */
    void print_header();                                             /**< Prints the header information each time a coil is generated*/

    // Stuff for asymmetry
    void symmetry_axis();
    int generate_asymmetric();

    // Constraint functions
    bool fm_flag;                         /**<apply the fraser-macrae constraint?  */
    double fraser_macrae(int idx);        /**< Constrain the pitch to rpt (Fraser & MacRae) */

    /**
     * Helper Functions
     *
     */

    void print_help();
    void print_plane();
    void print_coordinates();
    void print_axis();      /**< Print the helix axis coodinates to screen */
    int  ppx_to_xyz(char *filename);
    int  axis_to_xyz(char *filename);
    void inner_to_outer(double *&a, double *&b, double *&c,
                        double bond, double angle, double chi, double *&z);
    bool isfloat(const char *str);
    void reverse(double *a, int n);


    // Hardcoded bond lenghts and angles for
    // peptide plane construction

    // bond lengths
    double ca_c;
    double c_o;
    double c_n;
    double n_ca;

    //bond angles
    double ca_c_o;
    double ca_c_n;
    double c_n_ca;
    double n_ca_c;

};
}

#endif
#endif
