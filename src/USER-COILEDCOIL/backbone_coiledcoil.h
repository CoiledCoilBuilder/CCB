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

#ifndef SCADS_BACKBONE_COILEDCOIL_H
#define SCADS_BACKBONE_COILEDCOIL_H

#include "backbone.h"
#include "site.h"

/**
 * @def MAX_HELIX
 *
 * @brief maximum number of asymmetric helices
 */

#define MAX_HELIX 20

namespace SCADS_NS {
     
     class BackboneCoiledCoil : public Backbone {
     
     public:
          BackboneCoiledCoil(class SCADS *, int, const char **);
          ~BackboneCoiledCoil();

          // Member Functions
          void generate();                                              /**< build coiled-coil, update coordinates in x */ 
          void update_domain();                                         /**< set the domain coordinates to the backbone coordianates*/     
          void set_params(int argc, const char **argv, int n);          /**< set the backbone parameters before generation*/ 

     protected:
          virtual void init_style();                                     /**< Initialize the style (declare member variables, etc.. */ 
          virtual void update_style(int argc, const char **argv, int n); /**< Update the parameters and re-generate the structure */
          virtual void generate_style();                                 /**< Generate coordinates */

     private: 

          // Variables that describe the backbone topology
          unsigned int natom;           /**< number of atoms = numres*numhelix*4 */  
          unsigned int maxatom;         /**< maximum number of atoms allowed before realloc of atom vector */ 
          unsigned int nres;            /**< number of residues per helix */ 
          unsigned int nhelix;          /**< number of helices per coiled-coil */

          // Symmetric Parameters
          double pitch;                 /**< pitch of the coiled-coil */
          double radius;                /**< radius of the coiled-coil */ 
          double square;                /**< squareness of the coiled-coil */
          double phi;                   /**< initial phi angle used to build the peptide plane */ 
          double psi;                   /**< initial psi angle used to build the peptide plane */
          double rpr;                   /**< rise per residue, major helix frame, d */
          double omega;                 /**< major helix angular yield per residue; -2*PI*rpr/pitch */
          double omega_alpha;           /**< minor helix angular yield per residue; 2*pi/rpt */

          // Asymmetric Parameters
          double rotation[MAX_HELIX];   /**< rotation of the helices, for asymmetric, a list of rotations for each helix */ 
          double rpt[MAX_HELIX];        /**< residues per turn of the helices */   
          double zoff[MAX_HELIX];       /**< z-axis displacement of helices, for asymmetric, a list of offsets for each helix */ 

          bool anti_flag;               /**< antiparallel flag, specifies construction of antiparallel coiled-coil */
          bool asymmetric_flag;         /**< Are the helices in the coiled-coil symmetric? **/
          bool rebuild_domain;          /**< if true, we erase the existing coiled coil when we update */
                                        
          double **axis_x;              /**< coordinates of the minor-helical axis */ 
          double **pp_x;                /**< 2D-array of initial peptide-plane coordinates */ 
          double **x;                   /**< 2D-array of coordinates for coiled-coil */  
                                        
          Site **site;                  /**< Array of sites that this style created and can update or delete */
          unsigned int nsite;           /**< current number of sites that belong to this style */
          unsigned int maxsite;         /**< maximum number of sites before realloc is necessary */
          void add_site(Site *s);       /**< adds a site for this style to keep track of*/
                                        
          int mask;                     /**< sets the backbone atom's bitmask */

          void build_plane();           /**< build the first peptide plane */ 
          void align_plane(double *w);  /**< align the peptide-plane rotation vector with w (axis) */ 
          void helix_axis();            /**< generate the axis of the minor helix */ 
          void symmetry();              /**< apply symmetry operations to helix to generate coil */
          void allocate();              /**< increase size of x, axis if necessary */
          void azzero();                /**< zero out coordinates in x array */  


          void get_pp_params(double *axis0, double *axis1, 
              double *u, double *v, double *r, double &theta);             /**< Determines the screw-rotation parameters from the peptide plane */
          void crick(double *u, double rho, double *r1, double *r2);       /**< Sets the rotation angle to correspond to the crick angle */
          void next_plane(double *u, double *v, double theta);             /**< Generates the next plane given u, v, theat */
          void terminate();                                                /**< Adds the n-terminal nitrogen, rearranges coordinates */
          double memory_usage();                                           /**< Calculates the memory usage of this style */
          void print_header();                                             /**< Prints the header information each time a coil is generated*/

          // Stuff for asymmetry
          void symmetry_axis(double **axis);
          void generate_asymmetric();

          /**
           * Helper Functions
           * 
           */

          void print_plane();
          void print_coordinates();
          void ppx_to_xyz(char *filename);
          void axis_to_xyz(char *filename);
          void inner_to_outer(double a[4], double b[4], double c[4],
               double bond, double angle, double chi, double z[4]);
          bool isfloat(const char *str);

          
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
