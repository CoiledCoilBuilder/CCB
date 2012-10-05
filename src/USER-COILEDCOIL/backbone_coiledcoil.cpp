/**
 * @file   backbone_coiledcoil.cpp
 * @author Chris MacDermaid <chris@cabal1.nfs>
 * @date   Sun Jan 22 21:56:16 2012
 *
 * @brief  coiled-coil backbone generation routines
 *
 * The basis for the methodology here can be found in:
 * Quine,J.R. Helix parameters and protein structure using quaternions.
 * Journal of Molecular Structure: THEOCHEM. 460(1-3):53 - 66.
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "error.h"
#include "universe.h"
#include "memory.h"
#include "atom.h"
#include "group.h"
#include "site.h"
#include "domain.h"
#include "backbone_coiledcoil.h"
#include "bitmask.h"
#include "math_extra.h"
#include "constants.h"

/**
 * @def SITE_DELTA
 *
 * @brief increase site array by delta each realloc
 */

#define SITE_DELTA 64 

using namespace SCADS_NS;
using namespace MathExtra;

BackboneCoiledCoil::BackboneCoiledCoil(SCADS *scads, int narg, const char **arg) :
    Backbone(scads, narg, arg) {

    // Check to see that we have a legit style and the format is correct.
    if (strcmp(style, "coiledcoil") != 0 || narg < 3)
        error->all(FLERR, "Illegal backbone coiled-coil command");

    // Set some default values for the coiled coil geometries

    nres = 35;
    nhelix = 2;
    pitch = 120.0;
    radius = 4.65;
    square = 0.0;
    phi = -65.0;
    psi = -40.0;
    rpr = 1.495;

    // Asymmetric parameters, assume parallel, 3.64 rpt
    for (int i = 0; i < MAX_HELIX; i++) {
        rotation[i] = 0.0;
        zoff[i] = 0.0;
        rpt[i] = 3.64;
    }

    // Initial Output Order, maximum 64 chains
    for (int i = 0; i < 64; i++) {
        order[i] = i;
    }

    anti_flag = false;
    asymmetric_flag = false;
    rebuild_domain = true;

    x = NULL;
    pp_x = NULL;
    axis_x = NULL;

    // hardcoded bond lengths/angles
    // taken from CHARMM27 RESI GLY

    // bond lengths
    ca_c = 1.5166;
    c_o = 1.2273;
    c_n = 1.3475;
    n_ca = 1.4521;

    // bond angles
    ca_c_o = 120.2400;
    ca_c_n = 117.5700;
    c_n_ca = 123.2700;
    n_ca_c = 108.9400;

    // convert square, rotation to RAD
    square *= DEG2RAD;
    rotation[0] *= DEG2RAD;

    site = NULL;
    nsite = maxsite = 0;

    //Parse the provided parameters, if any
    if (narg > 3)
        set_params(narg, arg, 4);

    // Set parameter dependent values
    maxatom = 0;
    natom = nres * nhelix * 4;
    omega = -2 * PI * rpr / pitch;
    omega_alpha = 2 * PI / rpt[0];

    // Set default mask
    mask = 0;
}

/**
 * The style destructor
 *
 */
BackboneCoiledCoil::~BackboneCoiledCoil() {

    // destroy pp_x
    memory->destroy(pp_x);

    // Memory for axis
    memory->destroy(axis_x);

    // Memory for coil coordinates
    memory->destroy(x);

    // Memory for associated sites
    memory->sfree(site);
}

/**
 * Sets the coiled-coil parameters
 *
 * @param argv array of arguments
 * @param argc number of argumets
 * @param n where in the array of arguments to start parsing
 *
 */
void BackboneCoiledCoil::set_params(int argc, const char **argv, int n) {

    if (error->verbosity_level == 10)
        for (int i = 0; i < argc; i++)
            fprintf(screen, "%s\n", argv[i]);


    while (n < argc) {

        if (strcmp(argv[n], "-pitch") == 0) {
            n++;
            if (n == argc) error->one(FLERR, "Missing argument to -pitch");
            isfloat(argv[n]);
            pitch = atof(argv[n]);

        } else if (strcmp(argv[n], "-radius") == 0) {
            n++;
            if (n == argc) error->one(FLERR, "Missing argument to -radius");
            isfloat(argv[n]);
            radius = atof(argv[n]);

        } else if (strcmp(argv[n], "-rpr") == 0) {
            n++;
            if (n == argc) error->one(FLERR, "Missing argument to -rpr");
            isfloat(argv[n]);
            rpr = atof(argv[n]);

        } else if (strcmp(argv[n], "-square") == 0) {
            n++;
            if (n == argc) error->one(FLERR, "Missing argument to -square");
            isfloat(argv[n]);
            square = atof(argv[n]) * DEG2RAD;

        } else if (strcmp(argv[n], "-nres") == 0) {
            n++;
            if (n == argc) error->one(FLERR, "Missing argument to -nres");
            isfloat(argv[n]);
            nres = atoi(argv[n]);
            rebuild_domain = true;

            if (nres < 1) error->one(FLERR, "nres must be greater than 0");

        } else if (strcmp(argv[n], "-nhelix") == 0) {
            n++;
            if (n == argc) error->one(FLERR, "Missing argument to -nhelix");
            isfloat(argv[n]);
            nhelix = atoi(argv[n]);
            rebuild_domain = true;

            if (nhelix < 1) error->one(FLERR, "nhelix must be greater than 0");

        } else if (strcmp(argv[n], "-antiparallel") == 0) {
            anti_flag = true;
            n++;
            continue;

        } else if (strcmp(argv[n], "-asymmetric") == 0) {
            asymmetric_flag = true;
            n++;
            continue;

            /** These commands can have multiple arguments
             * due to their asymmetry. Arguments look like
             * -rotation 0 90 170
             */

        } else if (strcmp(argv[n], "-rotation") == 0) {
            n++;
            if (n == argc) error->one(FLERR, "Missing argument to -rotation");
            int i = 0;
            while (n < argc && isfloat(argv[n])) {
                
                double rot = atof(argv[n]);

                // Make sure rotation is between [-180, 180]
                while (rot > 180) rot -= 360;
                while (rot < -180) rot += 360;

                rotation[i++] = rot * DEG2RAD;
                n++;
            }
            continue;

        } else if (strcmp(argv[n], "-rpt") == 0) {
            n++;
            if (n == argc) error->one(FLERR, "Missing argument to -rpt");
            int i = 0;
            while (n < argc && isfloat(argv[n])) {
                rpt[i++] = atof(argv[n]);
                n++;
            }
            continue;

        } else if (strcmp(argv[n], "-zoff") == 0) {
            n++;
            if (n == argc) error->one(FLERR, "Missing argument to -zoff");
            int i = 0;
            while (n < argc && isfloat(argv[n])) {
                zoff[i++] = atof(argv[n]);
                n++;
            }
            continue;

        } else if (strcmp(argv[n], "-order") == 0) {
            n++;
            if (n == argc) error->one(FLERR, "Missing argument to -order");
            int i = 0;
            while (n < argc && isfloat(argv[n])) {
                order[i++] = atoi(argv[n]);
                n++;
 
            }
           continue;

        } else {
            char str[128];
            sprintf(str, "CCB: unknown option: %s", argv[n]);
            error->one(FLERR, str);
        }

        // Next Argument
        n++;
    }
}

void BackboneCoiledCoil::update_style(int argc, const char **argv, int n) {

    // set the params
    set_params(argc, argv, n);

    // Update parameter dependent values
    natom = nres * nhelix * 4;
    omega = -2 * PI * rpr / pitch;
    omega_alpha = 2 * PI / rpt[0];
}

/**
 * @brief Generate Coordinates
 *
 */
void BackboneCoiledCoil::generate_style() {

    // generate the coiled-coil
    if (asymmetric_flag) generate_asymmetric();
    else generate();

    // update the domain
    update_domain();
}

/**
 * Initalize the coiled coil backbone style
 *
 * Allocate memory for the peptide plane
 */

void BackboneCoiledCoil::init_style() {

    // create/set the bitmask for the backbone atoms in this style
    // we set it to the user-provided style id
    bitmask->add_bitmask(id);
    mask = bitmask->find_mask(id);

    // allocate memory for initial peptide plane
    // and construct it
    memory->create(pp_x, 5, 4, "backbonecoiledcoil:ppx");
}

/**
 * Allocates memory to store coordinates
 * for the entire helical system.
 */
void BackboneCoiledCoil::allocate() {

    //reallocate coordinate array if natom >= maxatom

    if (natom >= maxatom) {
        maxatom = natom;

        // reallocate the array to store coordinates of the atoms
        x = memory->grow(x, natom, 4, "backbonecoiledcoil:x");

        // reallocate the array to store the coodinates of the helical axis
        axis_x = memory->grow(axis_x, nres + 2, 4, "backbonecoiledcoild:axis_x");
    }

}

/**
 * Identifies a site that belongs to this style so
 * that it can be kept track of, updated, deleted later
 *
 * @param s
 */
void BackboneCoiledCoil::add_site(Site *s) {

    if (nsite >= maxsite) {
        maxsite += SITE_DELTA;
        site = (Site **) memory->srealloc(site, maxsite * sizeof(Site *), "backbonecoiledcoil:site");
    }

    site[nsite++] = s;
}

/**
 * zeros out the coordinate array, x
 */

void BackboneCoiledCoil::azzero() {
    for (unsigned int i = 0; i < natom; i++) {
        x[i][0] = 0.0;
        x[i][1] = 0.0;
        x[i][2] = 0.0;
        x[i][3] = 1.0;
    }

    for (unsigned int i = 0; i < nres + 2; i++) {
        axis_x[i][0] = 0.0;
        axis_x[i][1] = 0.0;
        axis_x[i][2] = 0.0;
        axis_x[i][3] = 1.0;
    }
}

/**
 * The main routine for generating
 * the helical system
 */

void BackboneCoiledCoil::generate() {

    // Print Header with info
    if (error->verbosity_level >= 10)
        print_header();

    // Set initial peptide-plane coordiantes
    build_plane();

    //see if we need to reallocate the coordinate array
    allocate();

    // zero out the coordinate and axis arrays
    azzero();

    // Generate the minior-helical axis
    helix_axis();

    // bring the plane to the first
    // helix axis point
    double m[4][4];
    double temp[4] = { 0.0 };

    moveby(axis_x[0], m);

    for (int i = 0; i < 5; i++) {
        matvec4(m, pp_x[i], temp);
        pp_x[i][0] = temp[0];
        pp_x[i][1] = temp[1];
        pp_x[i][2] = temp[2];
        pp_x[i][3] = temp[3];
    }

    double v[3] = { 0.0 }, r[3] = { 0.0 }, u[3] = { 0.0 };
    double theta = 0.0;

    // Align the plane rotation vector with
    // the helical axis
    align_plane(axis_x[1]);

    // get u, r, v for the peptide pane
    // axis vector becomes normalized u at axis0
    get_pp_params(axis_x[0], axis_x[1], u, v, r, theta);

    //rotate the plane to set the correct crick angle
    crick(u, rotation[0], r, axis_x[0]);

    ////offset the plane by calculated r
    get_pp_params(axis_x[0], axis_x[1], u, v, r, theta);
    moveto(r, m);

    for (int i = 0; i < 5; i++) {
        matvec4(m, pp_x[i], temp);
        pp_x[i][0] = temp[0];
        pp_x[i][1] = temp[1];
        pp_x[i][2] = temp[2];
        pp_x[i][3] = temp[3];
    }

    get_pp_params(axis_x[0], axis_x[1], u, v, r, theta);

    for (unsigned int i = 1, n = 0; i <= nres; i++) {

        // copy coordinates from peptide plane to x
        for (int j = 0; j < 4; j++, n++) {
            x[n][0] = pp_x[j][0];
            x[n][1] = pp_x[j][1];
            x[n][2] = pp_x[j][2];
            x[n][3] = pp_x[j][3];
        }

        /**
         * Get the next plane, use omega_alpha
         * so the user can manipulate the residues
         * per turn of the helix directly at risk
         * of losing the rpt-pitch relaonship...
         */
        next_plane(u, v, omega_alpha);

        // determine the next set of parameters
        // to produce the next plane
        get_pp_params(axis_x[i], axis_x[i + 1], u, v, r, theta);

    }

    // Terminate the helix
    terminate();

    // Generate helical symmetry mates
    symmetry();
}

/**
 * The main routine for generating
 * the helical system
 *
 * Adapted to remove the symmetry assumption
 * between helices in the coiled-coil, leading to
 * modulations in helical rotation and axial offset.
 *
 * This is particularally important for antiparallel
 * structures.
 */


void BackboneCoiledCoil::generate_asymmetric() {

    // Print Header with info
    if (error->verbosity_level >= 10)
        print_header();

    // Set initial peptide-plane coordiantes
    build_plane();

    //see if we need to reallocate the coordinate array
    allocate();

    // zero out the coordinate and axis arrays
    azzero();

    // Generate the minior-helical axis
    helix_axis();

    // Allocate some memory for copies of the helical axes and
    // generate the symmetric counterparts
    double **axis = NULL;
    axis = memory->grow(axis, (nres + 2) * nhelix, 4, "backbonecoiledcoild:axis");
    symmetry_axis(axis);

    /* Now, do the generation procedure for each axis, the procedure is
     * essentially the same, except we store the coordinates as an offset
     * after we terminate them in the final coordinate matrix and then return it
     */

    for (int i = nhelix - 1; i >= 0; i--) {

        // Set initial peptide-plane coordiantes (do this each time)
        build_plane();

        // calculate omega_alpha for the asymmetric rpt
        omega_alpha = 2 * PI / rpt[i];

        // Copy coordinates into the axis_x array
        int n = nres + 2;
        for (int j = 0; j < n; j++) {
            int offset = i * n + j;
            axis_x[j][0] = axis[offset][0];
            axis_x[j][1] = axis[offset][1];
            axis_x[j][2] = axis[offset][2];
            axis_x[j][3] = axis[offset][3];
        }

        // bring the plane to the first
        // helix axis point
        double m[4][4];
        double temp[4] = { 0.0 };

        moveby(axis_x[0], m);

        for (int j = 0; j < 5; j++) {
            matvec4(m, pp_x[j], temp);
            pp_x[j][0] = temp[0];
            pp_x[j][1] = temp[1];
            pp_x[j][2] = temp[2];
            pp_x[j][3] = temp[3];
        }

        double v[3] = { 0.0 }, r[3] = { 0.0 }, u[3] = { 0.0 };
        double theta = 0.0;

        // Align the plane rotation vector with
        // the helical axis
        align_plane(axis_x[1]);

        // get u, r, v for the peptide pane
        // axis vector becomes normalized u at axis0
        get_pp_params(axis_x[0], axis_x[1], u, v, r, theta);

        //rotate the plane to set the correct crick angle
        crick(u, rotation[i], r, axis_x[0]);

        ////offset the plane by calculated r
        get_pp_params(axis_x[0], axis_x[1], u, v, r, theta);
        moveto(r, m);

        for (int j = 0; j < 5; j++) {
            matvec4(m, pp_x[j], temp);
            pp_x[j][0] = temp[0];
            pp_x[j][1] = temp[1];
            pp_x[j][2] = temp[2];
            pp_x[j][3] = temp[3];
        }

        get_pp_params(axis_x[0], axis_x[1], u, v, r, theta);

        for (unsigned int j = 1, n = 0; j <= nres; j++) {

            // copy coordinates from peptide plane to x
            for (int k = 0; k < 4; k++, n++) {
                x[n][0] = pp_x[k][0];
                x[n][1] = pp_x[k][1];
                x[n][2] = pp_x[k][2];
                x[n][3] = pp_x[k][3];
            }

            /**
             * Get the next plane, use omega_alpha
             * so the user can manipulate the residues
             * per turn of the helix directly at risk
             * of losing the rpt-pitch relaonship...
             */
            next_plane(u, v, omega_alpha);

            // determine the next set of parameters
            // to produce the next plane
            get_pp_params(axis_x[j], axis_x[j + 1], u, v, r, theta);
        }

        // Terminate the helix
        terminate();

        // copy coordinates
        unsigned int natomper = natom / nhelix;
        if (i > 0)
            for (unsigned int j = 0; j < natomper; j++) {
                int offset = i * natomper + j;
                x[offset][0] = x[j][0];
                x[offset][1] = x[j][1];
                x[offset][2] = x[j][2];
                x[offset][3] = x[j][3];
            }
    }

    // Free up memory used by local copy of axis
    memory->destroy(axis);
}

/**
 *  Generates coordinates of the helix
 *  axis from the superhelical parameters
 *  provided from defaults or the user
 */

void BackboneCoiledCoil::helix_axis() {

    // center the helix axis in the x,y plane
    double z0 = -((nres - 1) * rpr / 2);

    // generate the coordinates
    for (unsigned int i = 0; i <= nres + 1; i++) {
        axis_x[i][0] = radius * cos(i * omega);
        axis_x[i][1] = radius * sin(i * omega);
        axis_x[i][2] = i * rpr + z0;
        axis_x[i][3] = 1.0;
    }
}

/**
 * Builds an idealized peptide plane
 * using CHARMM27 geometries from GLY
 */

void BackboneCoiledCoil::build_plane() {

    // build the peptide plane

    double m[4][4];
    double z[3] = { 0.0 };
    double zero[3] = { 0.0 };
    double temp[4] = { 0.0 };

    // unit normal along z;
    z[2] = 1;

    zero4(m);

    // first alpha-carbon
    pp_x[0][0] = -ca_c;
    pp_x[0][1] = 0;
    pp_x[0][2] = 0;
    pp_x[0][3] = 1;

    // carbonyl carbon
    pp_x[1][0] = 0;
    pp_x[1][1] = 0;
    pp_x[1][2] = 0;
    pp_x[1][3] = 1;

    // carbonyl oxygen;
    axis_angle_to_mat_trans4(ca_c_o * DEG2RAD, z, zero, m);
    matvec4(m, pp_x[0], pp_x[2]);
    double scale = c_o / ca_c;
    pp_x[2][0] *= scale;
    pp_x[2][1] *= scale;
    pp_x[2][2] *= scale;
    pp_x[2][3] = 1;

    // amide nitrogen
    axis_angle_to_mat_trans4(-ca_c_n * DEG2RAD, z, zero, m);
    matvec4(m, pp_x[0], pp_x[3]);
    scale = c_n / ca_c;
    pp_x[3][0] *= scale;
    pp_x[3][1] *= scale;
    pp_x[3][2] *= scale;
    pp_x[3][3] = 1;

    // second alpha carbon
    scale = n_ca / c_n;
    pp_x[4][0] = -pp_x[3][0] * scale;
    pp_x[4][1] = -pp_x[3][1] * scale;
    pp_x[4][2] = -pp_x[3][2] * scale;
    pp_x[4][3] = 1;

    axis_angle_to_mat_trans4(c_n_ca * DEG2RAD, z, pp_x[3], m);
    matvec4(m, pp_x[4], temp);

    pp_x[4][0] = temp[0];
    pp_x[4][1] = temp[1];
    pp_x[4][2] = temp[2];
    pp_x[4][3] = temp[3];

    //move all the coordinates to the ca reference position at the origin
    moveto(pp_x[0], m);
    for (int i = 0; i < 5; i++) {
        matvec4(m, pp_x[i], temp);
        pp_x[i][0] = temp[0];
        pp_x[i][1] = temp[1];
        pp_x[i][2] = temp[2];
        pp_x[i][3] = temp[3];
    }

    /*
    // Coordinates from old program for comparison
    pp_x[0][0] = 0.0;    pp_x[0][1] = 0.0;     pp_x[0][2] = 0.0; pp_x[0][3] = 1.0;
    pp_x[1][0] = 1.5100; pp_x[1][1] = 0.0;     pp_x[1][2] = 0.0; pp_x[1][3] = 1.0;
    pp_x[2][0] = 2.1393; pp_x[2][1] =-1.0684;  pp_x[2][2] = 0.0; pp_x[2][3] = 1.0;
    pp_x[3][0] = 2.0930; pp_x[3][1] = 1.1954;  pp_x[3][2] = 0.0; pp_x[3][3] = 1.0;
    pp_x[4][0] = 3.5450; pp_x[4][1] = 1.3480;  pp_x[4][2] = 0.0; pp_x[4][3] = 1.0;
    */
}

/**
 *  Aligns the peptide plane's rotation vector
 *  u along the axis generated by the parametric
 *  curve using the superhelical parameters
 *
 * @param w axis vector to align to
 */

void BackboneCoiledCoil::align_plane(double *w) {

    /**
     * pp_x[0] = CA
     * pp_x[1] = C
     * pp_x[2] = O
     * pp_x[3] = N
     * pp_x[4] = CA2
     */

    double ca[3] = { 0.0 }, a[3] = { 0.0 }, c[3] = { 0.0 };
    double n[3] = { 0.0 }, w_temp[4] = { 0.0 };
    double zero[3] = { 0.0 };

    double pp_temp[5][4];

    // get the coordinates of the ca atom
    ca[0] = -pp_x[0][0];
    ca[1] = -pp_x[0][1];
    ca[2] = -pp_x[0][2];

    // Center the peptide plane coordinates at the Ca atom
    double m1[4][4];
    zero4(m1);
    moveby(ca, m1);
    for (int i = 0; i < 5; i++)
        matvec4(m1, pp_x[i], pp_temp[i]);

    // Center the helix axis at ca..
    matvec4(m1, w, w_temp);
    norm3(w_temp);

    // Calculate peptide-plane vectors (c,a)
    // and peptide plane normal (n) centered
    // at ca1
    sub3(pp_temp[1], pp_temp[0], c);
    sub3(pp_temp[4], pp_temp[3], a);
    norm3(c);
    norm3(a);
    cross3(c, a, n);
    norm3(n);

    // convert to 4x4 quaternion representation
    // to perform sequentical rotation

    double m2[4][4];
    double m3[4][4];

    zero4(m2);
    zero4(m3);

    axis_angle_to_mat_quat4((phi + 180) * DEG2RAD,a,m1);
    axis_angle_to_mat_quat4(76 * DEG2RAD,n,m2);
    times4(m1, m2, m3);

    axis_angle_to_mat_quat4(psi * DEG2RAD,c,m1);
    times4(m3, m1, m2);

    // Get the quaternion coefficents and calculate
    // the vector of rotation (u) and the magnitude
    // of rotation, theta given by the 4x4 matrix

    double u[3] = { 0.0 };
    u[0] = m2[0][1];
    u[1] = m2[0][2];
    u[2] = m2[0][3];
    norm3(u);

    //double theta = 2*acos(m2[0][0]);

    /* calculate the vector normal to
     * u and w_temp, rotate about this vector
     * offset = acos(dot(u,w_temp)) to
     * determine the necessary
     * rotation to apply to the peptide plane to
     * align the rotation vector with provided axis, w
     */

    cross3(u, w_temp, n);
    double offset = acos(dot3(u, w_temp));

    // return to the global frame
    moveto(ca, m2);

    norm3(n);

    //rotate the peptide plane coordinates
    axis_angle_to_mat_trans4(offset, n, zero, m1);

    times4(m2, m1, m3);

    for (int i = 0; i < 5; i++)
        matvec4(m3, pp_temp[i], pp_x[i]);
}

/**
 *  Rotates the plane about the peptide
 *  plane rotation vector u so that the
 *  axis - CA vector points at the superhelical
 *  axis, setting the crick angle.
 *
 * @param u peptide plane rotation vector
 * @param r1 the current planes radius vector
 * @param r2 the superhelical radius vector
 */
void BackboneCoiledCoil::crick(double *u, double rho, double *r1, double *r2) {

    // rotates the peptide plane
    // about it's rotation axis so that
    // the planes radius vector points
    // at the coiled-coil axis.
    // e.g set the crick angle

    double b1[3] = { 0.0 };
    double b2[3] = { 0.0 };

    b1[0] = -r1[0];
    b1[1] = -r1[1];
    b1[2] = 0;

    b2[0] = -r2[0];
    b2[1] = -r2[1];
    b2[2] = 0;

    norm3(b1);
    norm3(b2);

    double crick = acos(dot3(b1, b2));
    double offset = rho - crick;

    double zero[3] = { 0.0 };

    // rotate the peptide plane about u
    // a magnitude offset...
    next_plane(u, zero, offset);
}

/**
 * Determines the helical parameters of the current
 * peptide plane in pp_x.
 *
 * @param axis0 the first axis point, the plane is placed here
 * @param axis1 the second axis point, u points at this point.
 * @param u peptdie plane rotation vector (calculated)
 * @param v virtual bond vector, CA2-CA1 (calculated)
 * @param r radius vector
 * @param theta rotation about u to produce next plane
 */

void BackboneCoiledCoil::get_pp_params(
    double *axis0, double *axis1, 
    double *u, double *v, double *r, 
    double &theta) 
{

    /**
     * calculates the helical parameters of the
     * current peptide plane from the
     * screw axis parameters:
     *
     * u = normalized rotation axis
     * v = translation vector (calculated)
     * theta = magnitude of rotation (calculated)
     * r = radius of helix (calculated)
     */

    double pp_temp[5][4], m[4][4];
    zero4(m);

    // Center the peptide plane coordinates at the axis0 position
    moveto(axis0, m);
    for (int i = 0; i < 5; i++)
        matvec4(m, pp_x[i], pp_temp[i]);

    // Center the axis vector axis1, at axis0 and
    // normalize it
    matvec4(m, axis1, u);
    norm3(u);

    double a[3] = { 0.0 }, c[3] = { 0.0 }, n[3] = { 0.0 };

    // Calculate peptide-plane vectors (c,a)
    // and peptide plane normal (n) centered
    // at ca1
    sub3(pp_temp[1], pp_temp[0], c);
    sub3(pp_temp[4], pp_temp[3], a);
    norm3(c);
    norm3(a);
    cross3(c, a, n);
    norm3(n);

    // Calculate theta
    double A = 0.994521895 - (dot3(u, c) * dot3(u, a));
    double B = 0.104528463 * dot3(u, n);
    double C = 0.342020143 - (dot3(u, c) * dot3(u, a));

    double A0 = A / sqrt((A * A) + (B * B));
    //double B0 = B/sqrt((A*A)+(B*B));
    double C0 = C / sqrt((A * A) + (B * B));

    double delta1 = acos(A0);
    //double delta2 = asin(B0);

    theta = acos(C0) + delta1;

    // calculate the translation vector
    // of the peptide plane (ca2 - ca1)
    sub3(pp_temp[4], pp_temp[0], v);

    // calculate the radius of the helix
    cross3(u, v, r);
    scale3((1.0 / tan(theta / 2.0)), r);
    add3(r, v, r);

    double r1[3] = { 0.0 };
    r1[0] = u[0];
    r1[1] = u[1];
    r1[2] = u[2];

    double dot = dot3(v, u);
    scale3(dot, r1);
    sub3(r, r1, r);
    scale3(0.5, r);

    if (error->verbosity_level == 10) {
        fprintf(screen, "theta:\t%10.4f\n", theta);
        fprintf(screen, "len u:\t%10.4f\n", len3(u));
        fprintf(screen, "len v:\t%10.4f\n", len3(v));
        fprintf(screen, "len r:\t%10.4f\n\n", len3(r));

        fprintf(screen, "u:\t%10.4f %10.4f %10.4f\n", u[0], u[1], u[2]);
        fprintf(screen, "v:\t%10.4f %10.4f %10.4f\n", v[0], v[1], v[2]);
        fprintf(screen, "r:\t%10.4f %10.4f %10.4f\n\n", r[0], r[1], r[2]);
    }

}

/**
 * Generates the next successive peptide plane
 * given u, v and theta
 *
 * @param u peptide plane rotation vector
 * @param v virtual bond vector (CA2 - CA1)
 * @param theta rotation magnitude about theta
 */

void BackboneCoiledCoil::next_plane(double *u, double *v, double theta) {

    double m1[4][4], m2[4][4], m3[4][4], m4[4][4];
    double ca[3] = { 0.0 };

    // Zero the matrices
    zero4(m1);
    zero4(m2);
    zero4(m3);
    zero4(m4);

    // get the coordinates of the ca atom
    ca[0] = -pp_x[0][0];
    ca[1] = -pp_x[0][1];
    ca[2] = -pp_x[0][2];

    // Center the peptide plane coordinates at the Ca atom
    moveby(ca, m1);

    // perform a rotation about u a magnitude theta and translate
    // by v on the peptide plane
    axis_angle_to_mat_trans4(theta, u, v, m2);

    // return to global-frame positions
    moveto(ca, m3);

    // multiply out vectors to get one
    times4(m3, m2, m4);
    times4(m4, m1, m3);

    double temp[4] = { 0.0 };
    for (int i = 0; i < 5; i++) {
        matvec4(m3, pp_x[i], temp);
        pp_x[i][0] = temp[0];
        pp_x[i][1] = temp[1];
        pp_x[i][2] = temp[2];
        pp_x[i][3] = temp[3];
    }
}

/**
 * Generates symmetrically related helices depending
 * on the options specified
 *
 * This is a pretty hacky way of doing the symmetry.
 */

void BackboneCoiledCoil::symmetry() {

    // Vector for offsetting helices
    double v[3] = { 0.0 };

    // Rotation about z-axis
    double z[3] = { 0.0 };
    z[2] = 1.0;

    double m1[4][4];
    double m2[4][4];
    double m3[4][4];
    double m4[4][4];
    double ident[4][4];
    double xtemp[natom][4];

    // get a 4x4 identity matrix, clear others
    identity4(m1);
    identity4(m2);
    identity4(m3);
    identity4(m4);
    identity4(ident);

    if (anti_flag) {

        // antiparallel
        // Calculate the superhelical radius
        // vector in the xy plane, should probably
        // interpolate this when nres/2 is odd.

        double r2d[3] = { 0.0 };
        r2d[0] = axis_x[nres / 2][0];
        r2d[1] = axis_x[nres / 2][1];
        r2d[2] = 0.0;
        norm3(r2d);

        axis_angle_to_mat_trans4(PI, r2d, v, m1);
    }

    /**
     * rotation about r2d a magnitude pi and about
     * z a magnitude 2*pi*i/nhelix. Odd helices
     * are offset along z if specified, even
     * helices are offset in x/y depending on
     * squareness.
     */

    for (unsigned int i = 0; i < nhelix; i++) {

        copy4(m4, ident);
        double theta = (2 * PI * i / nhelix);

        if ((i % 2) == 0) {
            v[2] = 0.0;
            theta += square;
        } else {
            v[2] = zoff[0];

            if (anti_flag)
                copy4(m4, m1);
        }

        // Rotation matrix about z
        axis_angle_to_mat_trans4(theta, z, v, m2);

        //multiply m2 on m4 return in m3
        times4(m2, m4, m3);

        // Perform rotation.
        int natomper = natom / nhelix;
        int jstart = i * natomper;
        int jend = jstart + natomper;

        for (int j = jstart, k = 0; j < jend; j++, k++)
            matvec4(m3, x[k], xtemp[j]);

    }

    /// Copy coordinates to x
    for (unsigned int i = 0; i < natom; i++) {
        x[i][0] = xtemp[i][0];
        x[i][1] = xtemp[i][1];
        x[i][2] = xtemp[i][2];
        x[i][3] = xtemp[i][3];
    }
}

/**
 * Applies the symmetry to just the helical axis, allows
 * for asymmetry in the individual helicles while retaining
 * the radius, pitch, residues per turn etc...
 *
 * This is a pretty hacky way of doing the symmetry.
 */

void BackboneCoiledCoil::symmetry_axis(double **axis) {

    // Vector for offsetting helices
    double v[3] = { 0.0 };

    // Rotation about z-axis
    double z[3] = { 0.0 };
    z[2] = 1.0;

    double m1[4][4];
    double m2[4][4];
    double m3[4][4];
    double m4[4][4];
    double ident[4][4];
    // double xtemp[(nres + 2) * nhelix][4];

    // get a 4x4 identity matrix, clear others
    identity4(m1);
    identity4(m2);
    identity4(m3);
    identity4(m4);
    identity4(ident);

    if (anti_flag) {

        // antiparallel
        // Calculate the superhelical radius
        // vector in the xy plane, should probably
        // interpolate this when nres/2 is odd.

        double r2d[3] = { 0.0 };
        r2d[0] = axis_x[nres / 2][0];
        r2d[1] = axis_x[nres / 2][1];
        r2d[2] = 0.0;
        norm3(r2d);

        axis_angle_to_mat_trans4(PI, r2d, v, m1);
    }

    /**
     * rotation about r2d a magnitude pi and about
     * z a magnitude 2*pi*i/nhelix. Odd helices
     * are offset along z if specified, even
     * helices are offset in x/y depending on
     * squareness.
     */

    for (unsigned int i = 0; i < nhelix; i++) {

        copy4(m4, ident);
        double theta = (2 * PI * i / nhelix);

        if ((i % 2) == 0) {
            v[2] = zoff[i];
            theta += square;
        } else {
            v[2] = zoff[i];

            if (anti_flag)
                copy4(m4, m1);
        }

        // Rotation matrix about z
        axis_angle_to_mat_trans4(theta, z, v, m2);

        //multiply m2 on m4 return in m3
        times4(m2, m4, m3);

        // Perform rotation.
        int natomper = nres + 2;
        int jstart = i * natomper;
        int jend = jstart + natomper;

        for (int j = jstart, k = 0; j < jend; j++, k++) {
            matvec4(m3, axis_x[k], axis[j]);
        }
    }
}

/**
 * Terminates the n-terminus of the
 * helix and shifts coordinate matrix
 * to start at n and end at o instead
 * of starting at ca and ending at n
 */

void BackboneCoiledCoil::terminate() {

    /* move all the coordinates + 1 index */

    unsigned int istart = natom / nhelix - 1;
    unsigned int iend = 0;

    for (unsigned int i = istart; i > iend; i--) {
        x[i][0] = x[i - 1][0];
        x[i][1] = x[i - 1][1];
        x[i][2] = x[i - 1][2];
        x[i][3] = x[i - 1][3];
    }

    // Terminate the end of the helix with "N"
    inner_to_outer(x[4], x[2], x[1], n_ca, n_ca_c, psi, x[0]);
}

/**
 * Passes the coordinates to scads by creating
 * or updating the domain on the fly and adding the atoms.
 */

void BackboneCoiledCoil::update_domain() {

    Site *cursite = NULL;
    Atom *curatom = NULL;
    int serial = 1;

    static const char *chainid[] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X",
                                     "Y", "Z" };

    /**
     * What we do here depends on if the user has requested more helices
     * or residues, e.g. we need to add sites. If no, then we can just update
     * the coordinates and delete the rotamers associated with those sites
     *
     * If we do have to add sites, we delete the sites that belong to this style,
     * using this styles site array, then we add new sites, and set the update flag
     * to false.
     *
     * we need to selectively rebuild the rotamers at these sites, we can make use
     * of the build_site command at each site, and the corresponding bitmask
     * for the backbone atoms
     */

    // Just update the coordinates, delete the rotamers
    if (!rebuild_domain) {
        for (unsigned int i = 0, offset = 0; i < nsite; i++, offset += 4) {

            // Delete all the rotamers at this site
            while (site[i]->nrotamer)
                site[i]->delete_rotamer(site[i]->rotamer[0]->id);

            /**
             * Update the coordinates of the backbone atoms.
             * note, we assume the user hasn't mucked with the
             * order of the backbone atoms, or deleted them.
             * If necessary, we can search for the type, but this
             * creates a lot of unnecessary overhead.
             */

            // N atom
            site[i]->fixed_atoms->atom[0]->x = x[offset][0];
            site[i]->fixed_atoms->atom[0]->y = x[offset][1];
            site[i]->fixed_atoms->atom[0]->z = x[offset][2];

            // CA atom
            site[i]->fixed_atoms->atom[1]->x = x[offset + 1][0];
            site[i]->fixed_atoms->atom[1]->y = x[offset + 1][1];
            site[i]->fixed_atoms->atom[1]->z = x[offset + 1][2];

            // C atom
            site[i]->fixed_atoms->atom[2]->x = x[offset + 2][0];
            site[i]->fixed_atoms->atom[2]->y = x[offset + 2][1];
            site[i]->fixed_atoms->atom[2]->z = x[offset + 2][2];

            // O atom
            site[i]->fixed_atoms->atom[3]->x = x[offset + 3][0];
            site[i]->fixed_atoms->atom[3]->y = x[offset + 3][1];
            site[i]->fixed_atoms->atom[3]->z = x[offset + 3][2];
        }

    } else {

        /**
         * Delete all the sites associated with the old
         * coiled-coil, and create new sites. This is
         * definitely more costly.
         */

        for (unsigned int i = 0; i < nsite; i++) {
            domain->delete_site(site[i]->id);
            site[i] = NULL;
        }

        int nperhelix = natom / nhelix;

        // Recreate the coiled coil contigiously
        nsite = 0;
        for (unsigned int i = 0, offset = 0; i < nhelix; i++) {

            if (order[i] >= nhelix)
              error->one(FLERR, "Orders must be less than the number of helices\n"
                    "e.g. if nhelix = 2, order is either {0 1} or {1 0}");

            // Calculate the offset w.r.t the user specified order
            offset = order[i]*nperhelix;

            for (unsigned int j = 0; j < nres; j++, offset += 4) {

                int isite = domain->add_site();
                cursite = domain->site[isite];

                // Set the site-bitmask for construction and atom inheritance
                cursite->mask |= mask;

                // add a pointer to this site to this style
                add_site(cursite);

                // Set the site resid number, chain
                // and seg id
                cursite->resid = j + 1;
                strcpy(cursite->chain, chainid[i]);
                strcpy(cursite->seg, chainid[i]);

                // set the group properties
                strcpy(cursite->fixed_atoms->type, "GLY");

                // Add N atom to the fixed_atoms group
                int iatom = cursite->fixed_atoms->add_atom();
                curatom = cursite->fixed_atoms->atom[iatom];

                curatom->serial = serial++;
                strcpy(curatom->name, "N");

                curatom->x = x[offset][0];
                curatom->y = x[offset][1];
                curatom->z = x[offset][2];

                //curatom->backbone = true;
                curatom->fixed = true;

                // add reverse references
                curatom->site = cursite;
                curatom->group = cursite->fixed_atoms;

                // Set bitmask
                curatom->mask |= mask;

                // Add CA atom to the fixed_atoms group
                iatom = cursite->fixed_atoms->add_atom();
                curatom = cursite->fixed_atoms->atom[iatom];

                curatom->serial = serial++;
                strcpy(curatom->name, "CA");

                curatom->x = x[offset + 1][0];
                curatom->y = x[offset + 1][1];
                curatom->z = x[offset + 1][2];

                //curatom->backbone = true;
                curatom->fixed = true;

                // add reverse references
                curatom->site = cursite;
                curatom->group = cursite->fixed_atoms;

                // Set bitmask
                curatom->mask |= mask;

                // Add C atom to the fixed_atoms group
                iatom = cursite->fixed_atoms->add_atom();
                curatom = cursite->fixed_atoms->atom[iatom];

                curatom->serial = serial++;
                strcpy(curatom->name, "C");

                curatom->x = x[offset + 2][0];
                curatom->y = x[offset + 2][1];
                curatom->z = x[offset + 2][2];

                //curatom->backbone = true;
                curatom->fixed = true;

                // add reverse references
                curatom->site = cursite;
                curatom->group = cursite->fixed_atoms;

                // Set bitmask
                curatom->mask |= mask;

                // Add O atom to the fixed_atoms group
                iatom = cursite->fixed_atoms->add_atom();
                curatom = cursite->fixed_atoms->atom[iatom];

                curatom->serial = serial++;
                strcpy(curatom->name, "O");

                curatom->x = x[offset + 3][0];
                curatom->y = x[offset + 3][1];
                curatom->z = x[offset + 3][2];

                //curatom->backbone = true;
                curatom->fixed = true;

                // add reverse references
                curatom->site = cursite;
                curatom->group = cursite->fixed_atoms;

                // Set bitmask
                curatom->mask |= mask;

            }

            rebuild_domain = false;
        }
    }
}

void BackboneCoiledCoil::print_header() {

    fprintf(screen, "\nGenerating a coiled-coil with the following parameters:\n"
        "N residues:           %d\n"
        "N helices:            %d\n"
        "Pitch (angstroms):    %.2f\n"
        "Radius (angstroms):   %.2f\n"
        "Rotation (degrees):   %.2f\n"
        "Residues per turn:    %.2f\n"
        "Zoff (angstroms):     %.2f\n"
        "Squareness (degrees): %.2f\n"
        "Rise per residue:     %.2f\n"
        "Antiparallel:         %d\n\n", nres, nhelix, pitch, radius, rotation[0] * RAD2DEG,
        rpt[0], zoff[0], square * RAD2DEG, rpr, anti_flag);

    if (asymmetric_flag)
        fprintf(screen, "Asymmetric Parameters:\n");
    for (unsigned int i = 0; i < nhelix; i++)
        fprintf(screen,
            "Rotation (degrees) %d: %.2f\n"
            "Zoff (angstrom)    %d: %.2f\n"
            "Residues per Turn  %d: %.2f\n",
            i+1, rotation[i] * RAD2DEG, i+1, zoff[i], i+1, rpt[i]);
    fprintf(screen, "\n\n");

}

/* ---------------------------------------------------------
   Helper Functions
   --------------------------------------------------------- */

/**
 * Prints the current peptide plane to screen
 */
void BackboneCoiledCoil::print_plane() {

    fprintf(screen, "CA1: %10.4f\t %10.4f\t %10.4f\t %10.4f\n", pp_x[0][0], pp_x[0][1], pp_x[0][2], pp_x[0][3]);

    fprintf(screen, "C  : %10.4f\t %10.4f\t %10.4f\t %10.4f\n", pp_x[1][0], pp_x[1][1], pp_x[1][2], pp_x[1][3]);

    fprintf(screen, "O  : %10.4f\t %10.4f\t %10.4f\t %10.4f\n", pp_x[2][0], pp_x[2][1], pp_x[2][2], pp_x[2][3]);

    fprintf(screen, "N  : %10.4f\t %10.4f\t %10.4f\t %10.4f\n", pp_x[3][0], pp_x[3][1], pp_x[3][2], pp_x[3][3]);

    fprintf(screen, "CA2: %10.4f\t %10.4f\t %10.4f\t %10.4f\n", pp_x[4][0], pp_x[4][1], pp_x[4][2], pp_x[4][3]);
}

/**
 * Outputs the current peptide plane to an xyz file
 *
 * @param filename the name of the file to output
 */

void BackboneCoiledCoil::ppx_to_xyz(char *filename) {

    // outputs the peptide plane to
    // a file in xyz format

    FILE *fp = NULL; /**< file pointer */

    char str[256];
    strcpy(str, filename);
    strcat(str, ".xyz");

    fp = fopen(str, "w");

    if (fp == NULL && universe->me == 0)
        error->one(FLERR, "Can't open output file");

    fprintf(fp, "%d\n", 5);
    fprintf(fp, "Output from SCADS\n");
    fprintf(fp, "%5s %10.4f %10.4f %10.4f\n", "CA", pp_x[0][0], pp_x[0][1], pp_x[0][2]);
    fprintf(fp, "%5s %10.4f %10.4f %10.4f\n", "C", pp_x[1][0], pp_x[1][1], pp_x[1][2]);
    fprintf(fp, "%5s %10.4f %10.4f %10.4f\n", "O", pp_x[2][0], pp_x[2][1], pp_x[2][2]);
    fprintf(fp, "%5s %10.4f %10.4f %10.4f\n", "N", pp_x[3][0], pp_x[3][1], pp_x[3][2]);
    fprintf(fp, "%5s %10.4f %10.4f %10.4f\n", "CA", pp_x[4][0], pp_x[4][1], pp_x[4][2]);

    fclose(fp);
}

/**
 * Outputs the current helical axis coordinates to an xyz file
 *
 * @param filename the name of the file to output
 */

void BackboneCoiledCoil::axis_to_xyz(char *filename) {

    FILE *fp = NULL; /**< file pointer */

    char str[256];
    strcpy(str, filename);
    strcat(str, ".xyz");

    fp = fopen(str, "w");

    if (fp == NULL && universe->me == 0)
        error->one(FLERR, "Can't open output file");

    fprintf(fp, "%d\n", nres + 1);
    fprintf(fp, "Output from SCADS\n");

    for (unsigned int i = 0; i <= nres; i++)
        fprintf(fp, "%5s %10.4f %10.4f %10.4f\n", "CA", axis_x[i][0], axis_x[i][1], axis_x[i][2]);

    fclose(fp);
}

/**
 * Prints the helix coordinates to screen
 */
void BackboneCoiledCoil::print_coordinates() {

    for (unsigned int i = 0; i < natom; i++) {
        fprintf(screen, "%d\t%10.4f\t%10.4f\t%10.4f\n", i, x[i][0], x[i][1], x[i][2]);
    }
}

/**
 * Given 3 consecutive atoms, a bond length,
 * angle and dihedral, places the next consecutive
 * atom
 *
 * @param a atom a coordinates
 * @param b atom b coordinates
 * @param c atom c coordinages
 * @param bond the bond length between atom c and z
 * @param angle the b-c-z bond angle
 * @param chi the a-b-c-z dihedral
 * @param z atom z's coordinates (calculated, returned)
 */

void BackboneCoiledCoil::inner_to_outer(double a[4], double b[4], double c[4], double bond, double angle, double chi, double z[4]) {

    double x[4] = { 0.0 }, y[4] = { 0.0 }, n[4] = { 0.0 };
    double z1[4] = { 0.0 };
    double v[3] = { 0.0 };

    double m1[4][4];
    zero4(m1);

    x[3] = y[3] = n[3] = z[3] = z1[3] = 1.0;

    // Convert theta & chi to radians
    angle *= DEG2RAD;
    chi *= DEG2RAD;

    // Calcualte vectors in plane
    sub3(a, b, x);
    sub3(c, b, y);
    norm3(x);
    norm3(y);

    // Calculate normal to plane
    cross3(y, x, n);
    norm3(n);

    // Rotate y about normal
    axis_angle_to_mat_trans4(PI - angle, n, v, m1);
    matvec4(m1, y, z1);

    // Rotate z1 about y
    axis_angle_to_mat_trans4(chi, y, v, m1);
    matvec4(m1, z1, z);

    // Scale z to bond length
    snormalize3(bond, z, z);

    // move to atom c
    add3(c, z, z);
}

/**
 * Calculates the memory usage for this style
 * in bytes.
 *
 * @return memory usage for this style in bytes
 */

double BackboneCoiledCoil::memory_usage() {

    // Coordinates of helix
    double bytes = natom * 4 * sizeof(double);

    // Coordinates of helix axis
    bytes += (nres + 2) * 4 * sizeof(double);

    // Coordinates of peptide plane
    bytes += 5 * 4 * sizeof(double);

    return bytes;
}

/**
 * Check to see if string is an integer or floating point
 *
 * @param str the sting to check
 *
 * @return true if the string is an integer or fp, or exp
 */

bool BackboneCoiledCoil::isfloat(const char *str) {

    unsigned int i = 0;
    while (isdigit(str[i]) || str[i] == '.'
         || str[i] == '-' || str[i] == 'e')
        i++;

    if (i == strlen(str))
        return true;
    
    return false;

}
