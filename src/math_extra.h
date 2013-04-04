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
 * @file   math_extra.h
 * @author Chris Von Bargen <cvb@magrathea.chem.upenn.edu>
 * @date   Nov 29 2011
 *
 * Vector, matrix, etc. routines
 *
 * Taken directly from LAMMPS
 */

/* ----------------------------------------------------------------------
   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   http://lammps.sandia.gov, Sandia National Laboratories
   Steve Plimpton, sjplimp@sandia.gov

   Copyright (2003) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under
   the GNU General Public License.

   See the README file in the top-level LAMMPS directory.
   ------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------
   Contributing author: Mike Brown (SNL)
   ------------------------------------------------------------------------- */

#ifndef CCB_MATH_EXTRA_H
#define CCB_MATH_EXTRA_H

#include "math.h"
#include "stdio.h"
#include "string.h"
#include "error.h"
#include "constants.h"

namespace MathExtra {

    // 3 vector operations
    void norm3(double *v);
    void normalize3(const double *v, double *ans);
    void snormalize3(const double, const double *v, double *ans);
    void negate3(double *v);
    void scale3(double s, double *v);
    void add3(const double *v1, const double *v2, double *ans);
    void sub3(const double *v1, const double *v2, double *ans);
    double len3(const double *v);
    double lensq3(const double *v);
    double dot3(const double *v1, const double *v2);
    void cross3(const double *v1, const double *v2, double *ans);
    void zero3(double v[3]);
    void identity3(double ans[3][3]);
    void rotate(double *p, double *center, double *vec, double theta); /**< Rotate point p about vector going through center by theta */

    // 3x3 matrix operations
    double det3(const double mat[3][3]);
    void diag_times3(const double *diagonal, const double mat[3][3], double ans[3][3]);
    void plus3(const double m[3][3], const double m2[3][3], double ans[3][3]);
    void times3(const double m[3][3], const double m2[3][3], double ans[3][3]);
    void transpose3(const double m[3][3], double ans[3][3]);
    void transpose_times3(const double mat1[3][3], const double mat2[3][3], double ans[3][3]);
    void times3_transpose(const double mat1[3][3], const double mat2[3][3], double ans[3][3]);
    void invert3(const double mat[3][3], double ans[3][3]);
    void matvec(const double mat[3][3], const double*vec, double *ans);
    void matvec(const double *ex, const double *ey, const double *ez, const double *vec, double *ans);
    void transpose_matvec(const double mat[3][3], const double*vec, double *ans);
    void transpose_matvec(const double *ex, const double *ey, const double *ez, const double *v, double *ans);
    void transpose_diag3(const double mat[3][3], const double*vec, double ans[3][3]);
    void vecmat(const double *v, const double m[3][3], double *ans);
    void scalar_times3(const double f, double m[3][3]);
    void zero3(double m[3][3]);

    void write3(const double mat[3][3]);
    void write3(const double v[3]);

    int mldivide3(const double mat[3][3], const double *vec, double *ans);
    int jacobi(double matrix[3][3], double *evalues, double evectors[3][3]);
    void rotate(double matrix[3][3], int i, int j, int k, int l, double s, double tau);
    void richardson(double *q, double *m, double *w, double *moments, double dtq);

    // shape matrix operations
    // upper-triangular 3x3 matrix stored in Voigt notation as 6-vector

    void multiply_shape_shape(const double *one, const double *two, double *ans);

    // quaternion operations
    void qnormalize(double *q);
    void qconjugate(double *q, double *qc);
    void vecquat(double *a, double *b, double *c);
    void quatvec(double *a, double *b, double *c);
    void quatquat(double *a, double *b, double *c);
    void invquatvec(double *a, double *b, double *c);
    void axisangle_to_quat(const double *v, const double angle, double *quat);

    void angmom_to_omega(double *m, double *ex, double *ey, double *ez, double *idiag, double *w);
    void omega_to_angmom(double *w, double *ex, double *ey, double *ez, double *idiag, double *m);
    void mq_to_omega(double *m, double *q, double *moments, double *w);
    void exyz_to_q(double *ex, double *ey, double *ez, double *q);
    void q_to_exyz(double *q, double *ex, double *ey, double *ez);
    void quat_to_mat(const double *quat, double mat[3][3]);
    void quat_to_mat_trans(const double *quat, double mat[3][3]);

    // rotation operations
    void rotation_generator_x(const double m[3][3], double ans[3][3]);
    void rotation_generator_y(const double m[3][3], double ans[3][3]);
    void rotation_generator_z(const double m[3][3], double ans[3][3]);

    // 4x4 matrix and quaternion operations
    void axis_angle_to_mat_trans4(const double theta, const double *u, const double *v, double ans[4][4]);
    void axis_angle_to_mat_quat4(const double theta, const double *u, double ans[4][4]);
    void times4(const double m[4][4], const double m2[4][4], double ans[4][4]);
    void transpose4(const double m[4][4], double ans[4][4]);
    void matvec4(const double m[4][4], const double *v, double *ans);
    void moveby(const double *v, double ans[4][4]);
    void moveto(const double *v, double ans[4][4]);
    void identity4(double ans[4][4]);
    void copy4(double ans[4][4], const double m[4][4]);
    void zero4(double v[4]);
    void zero4(double m[4][4]);
    void write4(const double mat[4][4]);

    void axistovec(const char *axis, const double v[3], double m[4][4]);
    void vectoaxis(const char *axis, const double v[3], double m[4][4]);
}

/* ----------------------------------------------------------------------
   normalize a vector in place
   ------------------------------------------------------------------------- */

inline void MathExtra::norm3(double *v) {
    double scale = 1.0 / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    v[0] *= scale;
    v[1] *= scale;
    v[2] *= scale;
}

/* ----------------------------------------------------------------------
   normalize a vector, return in ans
   ------------------------------------------------------------------------- */

inline void MathExtra::normalize3(const double *v, double *ans) {
    double scale = 1.0 / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    ans[0] = v[0] * scale;
    ans[1] = v[1] * scale;
    ans[2] = v[2] * scale;
}

/* ----------------------------------------------------------------------
   scale a vector to length
   ------------------------------------------------------------------------- */

inline void MathExtra::snormalize3(const double length, const double *v, double *ans) {
    double scale = length / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    ans[0] = v[0] * scale;
    ans[1] = v[1] * scale;
    ans[2] = v[2] * scale;
}

/* ----------------------------------------------------------------------
   negate vector v
   ------------------------------------------------------------------------- */

inline void MathExtra::negate3(double *v) {
    v[0] = -v[0];
    v[1] = -v[1];
    v[2] = -v[2];
}

/* ----------------------------------------------------------------------
   scale vector v by s
   ------------------------------------------------------------------------- */

inline void MathExtra::scale3(double s, double *v) {
    v[0] *= s;
    v[1] *= s;
    v[2] *= s;
}

/* ----------------------------------------------------------------------
   ans = v1 + v2
   ------------------------------------------------------------------------- */

inline void MathExtra::add3(const double *v1, const double *v2, double *ans) {
    ans[0] = v1[0] + v2[0];
    ans[1] = v1[1] + v2[1];
    ans[2] = v1[2] + v2[2];
}

/* ----------------------------------------------------------------------
   ans = v1 - v2
   ------------------------------------------------------------------------- */

inline void MathExtra::sub3(const double *v1, const double *v2, double *ans) {
    ans[0] = v1[0] - v2[0];
    ans[1] = v1[1] - v2[1];
    ans[2] = v1[2] - v2[2];
}

/* ----------------------------------------------------------------------
   length of vector v
   ------------------------------------------------------------------------- */

inline double MathExtra::len3(const double *v) {
    return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

/* ----------------------------------------------------------------------
   squared length of vector v, or dot product of v with itself
   ------------------------------------------------------------------------- */

inline double MathExtra::lensq3(const double *v) {
    return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}

/* ----------------------------------------------------------------------
   dot product of 2 vectors
   ------------------------------------------------------------------------- */

inline double MathExtra::dot3(const double *v1, const double *v2) {
    return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

/* ----------------------------------------------------------------------
   cross product of 2 vectors
   ------------------------------------------------------------------------- */

inline void MathExtra::cross3(const double *v1, const double *v2, double *ans) {
    ans[0] = v1[1] * v2[2] - v1[2] * v2[1];
    ans[1] = v1[2] * v2[0] - v1[0] * v2[2];
    ans[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

/* +-----------------+  */
/* | Zero a 3 vector |  */
/* +-----------------+  */

inline void MathExtra::zero3(double v[3]) {
    v[0] = 0.0;
    v[1] = 0.0;
    v[2] = 0.0;
}

/* +------------------+  */
/* | Zero a 3x3 array |  */
/* +------------------+  */

inline void MathExtra::zero3(double m[3][3]) {
    m[0][0] = 0.0; m[0][1] = 0.0; m[0][2] = 0.0;
    m[1][0] = 0.0; m[1][1] = 0.0; m[1][2] = 0.0;
    m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 0.0;
}

/* ----------------------------------------------------------------------
   Return a 3x3 identity matrix
   ------------------------------------------------------------------------- */
inline void MathExtra::identity3(double ans[3][3]) {
    ans[0][0] = 1;    ans[0][1] = 0;    ans[0][2] = 0;
    ans[1][0] = 0;     ans[1][1] = 1;    ans[1][2] = 0;
    ans[2][0] = 0;     ans[2][1] = 0;    ans[2][2] = 1;
}


/**
 * Rotate point p about vector vec (which passes through point center)
 * by theta degrees. New coordinates of p are assigned directly to p.
 *
 * @param   point to rotate
 * @param   point along rotation line
 * @param   vector about which to rotate
 * @param   amount in degrees by which to rotate
 */
inline void MathExtra::rotate(double *p, double *center, double *vector, double theta) {

    double vec[3];
    MathExtra::normalize3(vector, vec);
    double x = p[0];
    double y = p[1];
    double z = p[2];
    double au = center[0] * vec[0];
    double bv = center[1] * vec[1];
    double cw = center[2] * vec[2];
    double uu = vec[0] * vec[0];
    double vv = vec[1] * vec[1];
    double ww = vec[2] * vec[2];
    double uxvywz = vec[0] * x + vec[1] * y + vec[2] * z;
    double sint = sin(theta * DEG2RAD);
    double cost = cos(theta * DEG2RAD);

    p[0] = (center[0] * (vv + ww) - vec[0] * (bv + cw - uxvywz)) * (1 - cost) + x * cost
        + (-(center[2] * vec[1]) + (center[1] * vec[2]) - vec[2] * y + vec[1] * z) * sint;
    p[1] = (center[1] * (uu + ww) - vec[1] * (au + cw - uxvywz)) * (1 - cost) + y * cost
        + ((center[2] * vec[0]) - (center[0] * vec[2]) + vec[2] * x - vec[0] * z) * sint;
    p[2] = (center[2] * (uu + vv) - vec[2] * (au + bv - uxvywz)) * (1 - cost) + z * cost
        + (-(center[1] * vec[0]) + (center[0] * vec[1]) - vec[1] * x + vec[0] * y) * sint;
}

/* ----------------------------------------------------------------------
   determinant of a matrix
   ------------------------------------------------------------------------- */

inline double MathExtra::det3(const double m[3][3]) {
    double ans = m[0][0] * m[1][1] * m[2][2] - m[0][0] * m[1][2] * m[2][1] - m[1][0] * m[0][1] * m[2][2]
        + m[1][0] * m[0][2] * m[2][1] + m[2][0] * m[0][1] * m[1][2] - m[2][0] * m[0][2] * m[1][1];
    return ans;
}

/* ----------------------------------------------------------------------
   diagonal matrix times a full matrix
   ------------------------------------------------------------------------- */

inline void MathExtra::diag_times3(const double *d, const double m[3][3], double ans[3][3]) {
    ans[0][0] = d[0] * m[0][0];
    ans[0][1] = d[0] * m[0][1];
    ans[0][2] = d[0] * m[0][2];
    ans[1][0] = d[1] * m[1][0];
    ans[1][1] = d[1] * m[1][1];
    ans[1][2] = d[1] * m[1][2];
    ans[2][0] = d[2] * m[2][0];
    ans[2][1] = d[2] * m[2][1];
    ans[2][2] = d[2] * m[2][2];
}

/* ----------------------------------------------------------------------
   add two matrices
   ------------------------------------------------------------------------- */

inline void MathExtra::plus3(const double m[3][3], const double m2[3][3], double ans[3][3]) {
    ans[0][0] = m[0][0] + m2[0][0];
    ans[0][1] = m[0][1] + m2[0][1];
    ans[0][2] = m[0][2] + m2[0][2];
    ans[1][0] = m[1][0] + m2[1][0];
    ans[1][1] = m[1][1] + m2[1][1];
    ans[1][2] = m[1][2] + m2[1][2];
    ans[2][0] = m[2][0] + m2[2][0];
    ans[2][1] = m[2][1] + m2[2][1];
    ans[2][2] = m[2][2] + m2[2][2];
}

/* ----------------------------------------------------------------------
   multiply mat1 times mat2
   ------------------------------------------------------------------------- */

inline void MathExtra::times3(const double m[3][3], const double m2[3][3], double ans[3][3]) {
    ans[0][0] = m[0][0] * m2[0][0] + m[0][1] * m2[1][0] + m[0][2] * m2[2][0];
    ans[0][1] = m[0][0] * m2[0][1] + m[0][1] * m2[1][1] + m[0][2] * m2[2][1];
    ans[0][2] = m[0][0] * m2[0][2] + m[0][1] * m2[1][2] + m[0][2] * m2[2][2];
    ans[1][0] = m[1][0] * m2[0][0] + m[1][1] * m2[1][0] + m[1][2] * m2[2][0];
    ans[1][1] = m[1][0] * m2[0][1] + m[1][1] * m2[1][1] + m[1][2] * m2[2][1];
    ans[1][2] = m[1][0] * m2[0][2] + m[1][1] * m2[1][2] + m[1][2] * m2[2][2];
    ans[2][0] = m[2][0] * m2[0][0] + m[2][1] * m2[1][0] + m[2][2] * m2[2][0];
    ans[2][1] = m[2][0] * m2[0][1] + m[2][1] * m2[1][1] + m[2][2] * m2[2][1];
    ans[2][2] = m[2][0] * m2[0][2] + m[2][1] * m2[1][2] + m[2][2] * m2[2][2];
}

/* ----------------------------------------------------------------------
   Take the transpose of the 3x3 matrix
   ------------------------------------------------------------------------- */
inline void MathExtra::transpose3(const double m[3][3], double ans[3][3]) {
    ans[0][0] = m[0][0]; ans[0][1] = m[1][0]; ans[0][2] = m[2][0];
    ans[1][0] = m[0][1]; ans[1][1] = m[1][1]; ans[1][2] = m[2][1];
    ans[2][0] = m[0][2]; ans[2][1] = m[1][2]; ans[2][2] = m[2][2];
}

/* ----------------------------------------------------------------------
   multiply the transpose of mat1 times mat2
   ------------------------------------------------------------------------- */

inline void MathExtra::transpose_times3(const double m[3][3], const double m2[3][3], double ans[3][3]) {
    ans[0][0] = m[0][0] * m2[0][0] + m[1][0] * m2[1][0] + m[2][0] * m2[2][0];
    ans[0][1] = m[0][0] * m2[0][1] + m[1][0] * m2[1][1] + m[2][0] * m2[2][1];
    ans[0][2] = m[0][0] * m2[0][2] + m[1][0] * m2[1][2] + m[2][0] * m2[2][2];
    ans[1][0] = m[0][1] * m2[0][0] + m[1][1] * m2[1][0] + m[2][1] * m2[2][0];
    ans[1][1] = m[0][1] * m2[0][1] + m[1][1] * m2[1][1] + m[2][1] * m2[2][1];
    ans[1][2] = m[0][1] * m2[0][2] + m[1][1] * m2[1][2] + m[2][1] * m2[2][2];
    ans[2][0] = m[0][2] * m2[0][0] + m[1][2] * m2[1][0] + m[2][2] * m2[2][0];
    ans[2][1] = m[0][2] * m2[0][1] + m[1][2] * m2[1][1] + m[2][2] * m2[2][1];
    ans[2][2] = m[0][2] * m2[0][2] + m[1][2] * m2[1][2] + m[2][2] * m2[2][2];
}

/* ----------------------------------------------------------------------
   multiply mat1 times transpose of mat2
   ------------------------------------------------------------------------- */

inline void MathExtra::times3_transpose(const double m[3][3], const double m2[3][3], double ans[3][3]) {
    ans[0][0] = m[0][0] * m2[0][0] + m[0][1] * m2[0][1] + m[0][2] * m2[0][2];
    ans[0][1] = m[0][0] * m2[1][0] + m[0][1] * m2[1][1] + m[0][2] * m2[1][2];
    ans[0][2] = m[0][0] * m2[2][0] + m[0][1] * m2[2][1] + m[0][2] * m2[2][2];
    ans[1][0] = m[1][0] * m2[0][0] + m[1][1] * m2[0][1] + m[1][2] * m2[0][2];
    ans[1][1] = m[1][0] * m2[1][0] + m[1][1] * m2[1][1] + m[1][2] * m2[1][2];
    ans[1][2] = m[1][0] * m2[2][0] + m[1][1] * m2[2][1] + m[1][2] * m2[2][2];
    ans[2][0] = m[2][0] * m2[0][0] + m[2][1] * m2[0][1] + m[2][2] * m2[0][2];
    ans[2][1] = m[2][0] * m2[1][0] + m[2][1] * m2[1][1] + m[2][2] * m2[1][2];
    ans[2][2] = m[2][0] * m2[2][0] + m[2][1] * m2[2][1] + m[2][2] * m2[2][2];
}

/* ----------------------------------------------------------------------
   invert a matrix
   does NOT check for singular or badly scaled matrix
   ------------------------------------------------------------------------- */

inline void MathExtra::invert3(const double m[3][3], double ans[3][3]) {
    double den = m[0][0] * m[1][1] * m[2][2] - m[0][0] * m[1][2] * m[2][1];
    den += -m[1][0] * m[0][1] * m[2][2] + m[1][0] * m[0][2] * m[2][1];
    den += m[2][0] * m[0][1] * m[1][2] - m[2][0] * m[0][2] * m[1][1];

    ans[0][0] = (m[1][1] * m[2][2] - m[1][2] * m[2][1]) / den;
    ans[0][1] = -(m[0][1] * m[2][2] - m[0][2] * m[2][1]) / den;
    ans[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) / den;
    ans[1][0] = -(m[1][0] * m[2][2] - m[1][2] * m[2][0]) / den;
    ans[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) / den;
    ans[1][2] = -(m[0][0] * m[1][2] - m[0][2] * m[1][0]) / den;
    ans[2][0] = (m[1][0] * m[2][1] - m[1][1] * m[2][0]) / den;
    ans[2][1] = -(m[0][0] * m[2][1] - m[0][1] * m[2][0]) / den;
    ans[2][2] = (m[0][0] * m[1][1] - m[0][1] * m[1][0]) / den;
}

/* ----------------------------------------------------------------------
   matrix times vector
   ------------------------------------------------------------------------- */

inline void MathExtra::matvec(const double m[3][3], const double *v, double *ans) {
    ans[0] = m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2];
    ans[1] = m[1][0] * v[0] + m[1][1] * v[1] + m[1][2] * v[2];
    ans[2] = m[2][0] * v[0] + m[2][1] * v[1] + m[2][2] * v[2];
}

/* ----------------------------------------------------------------------
   matrix times vector
   ------------------------------------------------------------------------- */

inline void MathExtra::matvec(const double *ex, const double *ey, const double *ez, const double *v, double *ans) {
    ans[0] = ex[0] * v[0] + ey[0] * v[1] + ez[0] * v[2];
    ans[1] = ex[1] * v[0] + ey[1] * v[1] + ez[1] * v[2];
    ans[2] = ex[2] * v[0] + ey[2] * v[1] + ez[2] * v[2];
}

/* ----------------------------------------------------------------------
   transposed matrix times vector
   ------------------------------------------------------------------------- */

inline void MathExtra::transpose_matvec(const double m[3][3], const double *v, double *ans) {
    ans[0] = m[0][0] * v[0] + m[1][0] * v[1] + m[2][0] * v[2];
    ans[1] = m[0][1] * v[0] + m[1][1] * v[1] + m[2][1] * v[2];
    ans[2] = m[0][2] * v[0] + m[1][2] * v[1] + m[2][2] * v[2];
}

/* ----------------------------------------------------------------------
   transposed matrix times vector
   ------------------------------------------------------------------------- */

inline void MathExtra::transpose_matvec(const double *ex, const double *ey, const double *ez, const double *v, double *ans) {
    ans[0] = ex[0] * v[0] + ex[1] * v[1] + ex[2] * v[2];
    ans[1] = ey[0] * v[0] + ey[1] * v[1] + ey[2] * v[2];
    ans[2] = ez[0] * v[0] + ez[1] * v[1] + ez[2] * v[2];
}

/* ----------------------------------------------------------------------
   transposed matrix times diagonal matrix
   ------------------------------------------------------------------------- */

inline void MathExtra::transpose_diag3(const double m[3][3], const double *d, double ans[3][3]) {
    ans[0][0] = m[0][0] * d[0];
    ans[0][1] = m[1][0] * d[1];
    ans[0][2] = m[2][0] * d[2];
    ans[1][0] = m[0][1] * d[0];
    ans[1][1] = m[1][1] * d[1];
    ans[1][2] = m[2][1] * d[2];
    ans[2][0] = m[0][2] * d[0];
    ans[2][1] = m[1][2] * d[1];
    ans[2][2] = m[2][2] * d[2];
}

/* ----------------------------------------------------------------------
   row vector times matrix
   ------------------------------------------------------------------------- */

inline void MathExtra::vecmat(const double *v, const double m[3][3], double *ans) {
    ans[0] = v[0] * m[0][0] + v[1] * m[1][0] + v[2] * m[2][0];
    ans[1] = v[0] * m[0][1] + v[1] * m[1][1] + v[2] * m[2][1];
    ans[2] = v[0] * m[0][2] + v[1] * m[1][2] + v[2] * m[2][2];
}

/* ----------------------------------------------------------------------
   matrix times scalar, in place
   ------------------------------------------------------------------------- */

inline void MathExtra::scalar_times3(const double f, double m[3][3]) {
    m[0][0] *= f;
    m[0][1] *= f;
    m[0][2] *= f;
    m[1][0] *= f;
    m[1][1] *= f;
    m[1][2] *= f;
    m[2][0] *= f;
    m[2][1] *= f;
    m[2][2] *= f;
}

/* ----------------------------------------------------------------------
   multiply 2 shape matrices
   upper-triangular 3x3, stored as 6-vector in Voigt notation
   ------------------------------------------------------------------------- */

inline void MathExtra::multiply_shape_shape(const double *one, const double *two, double *ans) {
    ans[0] = one[0] * two[0];
    ans[1] = one[1] * two[1];
    ans[2] = one[2] * two[2];
    ans[3] = one[1] * two[3] + one[3] * two[2];
    ans[4] = one[0] * two[4] + one[5] * two[3] + one[4] * two[2];
    ans[5] = one[0] * two[5] + one[5] * two[1];
}

/* ----------------------------------------------------------------------
   normalize a quaternion
   ------------------------------------------------------------------------- */

inline void MathExtra::qnormalize(double *q) {
    double norm = 1.0 / sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
    q[0] *= norm;
    q[1] *= norm;
    q[2] *= norm;
    q[3] *= norm;
}

/* ----------------------------------------------------------------------
   conjugate of a quaternion: qc = conjugate of q
   assume q is of unit length
   ------------------------------------------------------------------------- */

inline void MathExtra::qconjugate(double *q, double *qc) {
    qc[0] = q[0];
    qc[1] = -q[1];
    qc[2] = -q[2];
    qc[3] = -q[3];
}

/* ----------------------------------------------------------------------
   vector-quaternion multiply: c = a*b, where a = (0,a)
   ------------------------------------------------------------------------- */

inline void MathExtra::vecquat(double *a, double *b, double *c) {
    c[0] = -a[0] * b[1] - a[1] * b[2] - a[2] * b[3];
    c[1] = b[0] * a[0] + a[1] * b[3] - a[2] * b[2];
    c[2] = b[0] * a[1] + a[2] * b[1] - a[0] * b[3];
    c[3] = b[0] * a[2] + a[0] * b[2] - a[1] * b[1];
}

/* ----------------------------------------------------------------------
   quaternion-vector multiply: c = a*b, where b = (0,b)
   ------------------------------------------------------------------------- */

inline void MathExtra::quatvec(double *a, double *b, double *c) {
    c[0] = -a[1] * b[0] - a[2] * b[1] - a[3] * b[2];
    c[1] = a[0] * b[0] + a[2] * b[2] - a[3] * b[1];
    c[2] = a[0] * b[1] + a[3] * b[0] - a[1] * b[2];
    c[3] = a[0] * b[2] + a[1] * b[1] - a[2] * b[0];
}

/* ----------------------------------------------------------------------
   quaternion-quaternion multiply: c = a*b
   ------------------------------------------------------------------------- */

inline void MathExtra::quatquat(double *a, double *b, double *c) {
    c[0] = a[0] * b[0] - a[1] * b[1] - a[2] * b[2] - a[3] * b[3];
    c[1] = a[0] * b[1] + b[0] * a[1] + a[2] * b[3] - a[3] * b[2];
    c[2] = a[0] * b[2] + b[0] * a[2] + a[3] * b[1] - a[1] * b[3];
    c[3] = a[0] * b[3] + b[0] * a[3] + a[1] * b[2] - a[2] * b[1];
}

/* ----------------------------------------------------------------------
   quaternion multiply: c = inv(a)*b
   a is a quaternion
   b is a four component vector
   c is a three component vector
   ------------------------------------------------------------------------- */

inline void MathExtra::invquatvec(double *a, double *b, double *c) {
    c[0] = -a[1] * b[0] + a[0] * b[1] + a[3] * b[2] - a[2] * b[3];
    c[1] = -a[2] * b[0] - a[3] * b[1] + a[0] * b[2] + a[1] * b[3];
    c[2] = -a[3] * b[0] + a[2] * b[1] - a[1] * b[2] + a[0] * b[3];
}

/* ----------------------------------------------------------------------
   compute quaternion from axis-angle rotation
   v MUST be a unit vector
   ------------------------------------------------------------------------- */

inline void MathExtra::axisangle_to_quat(const double *v, const double angle, double *quat) {
    double halfa = 0.5 * angle;
    double sina = sin(halfa);
    quat[0] = cos(halfa);
    quat[1] = v[0] * sina;
    quat[2] = v[1] * sina;
    quat[3] = v[2] * sina;
}

/* ----------------------------------------------------------------------
   Apply principal rotation generator about x to rotation matrix m
   ------------------------------------------------------------------------- */

inline void MathExtra::rotation_generator_x(const double m[3][3], double ans[3][3]) {
    ans[0][0] = 0;
    ans[0][1] = -m[0][2];
    ans[0][2] = m[0][1];
    ans[1][0] = 0;
    ans[1][1] = -m[1][2];
    ans[1][2] = m[1][1];
    ans[2][0] = 0;
    ans[2][1] = -m[2][2];
    ans[2][2] = m[2][1];
}

/* ----------------------------------------------------------------------
   Apply principal rotation generator about y to rotation matrix m
   ------------------------------------------------------------------------- */

inline void MathExtra::rotation_generator_y(const double m[3][3], double ans[3][3]) {
    ans[0][0] = m[0][2];
    ans[0][1] = 0;
    ans[0][2] = -m[0][0];
    ans[1][0] = m[1][2];
    ans[1][1] = 0;
    ans[1][2] = -m[1][0];
    ans[2][0] = m[2][2];
    ans[2][1] = 0;
    ans[2][2] = -m[2][0];
}

/* ----------------------------------------------------------------------
   Apply principal rotation generator about z to rotation matrix m
   ------------------------------------------------------------------------- */

inline void MathExtra::rotation_generator_z(const double m[3][3], double ans[3][3]) {
    ans[0][0] = -m[0][1];
    ans[0][1] = m[0][0];
    ans[0][2] = 0;
    ans[1][0] = -m[1][1];
    ans[1][1] = m[1][0];
    ans[1][2] = 0;
    ans[2][0] = -m[2][1];
    ans[2][1] = m[2][0];
    ans[2][2] = 0;
}

/* +--------------------------------+  */
/* | 4x4 matrix and vector routines |  */
/* +--------------------------------+  */

/* ----------------------------------------------------------------------
   Generate a 4x4 rotation matrix corresponding to a rotation about vector
   u a magnitude theta and translated by vector v
   ------------------------------------------------------------------------- */

inline void MathExtra::axis_angle_to_mat_trans4(const double theta, const double *u, const double *v, double ans[4][4]) {


    double thetaover2 = theta / 2.0;
    double sin_theta_2 = sin(thetaover2);

    double quat[4] = { 0.0 };

    quat[0] = cos(thetaover2);
    quat[1] = sin_theta_2 * u[0];
    quat[2] = sin_theta_2 * u[1];
    quat[3] = sin_theta_2 * u[2];

    //double twow2 = 2.0*quat[0]*quat[0];
    double twoi2 = 2.0*quat[1]*quat[1];
    double twoj2 = 2.0*quat[2]*quat[2];
    double twok2 = 2.0*quat[3]*quat[3];
    double twoij = 2.0*quat[1]*quat[2];
    double twoik = 2.0*quat[1]*quat[3];
    double twojk = 2.0*quat[2]*quat[3];
    double twoiw = 2.0*quat[1]*quat[0];
    double twojw = 2.0*quat[2]*quat[0];
    double twokw = 2.0*quat[3]*quat[0];

    ans[0][0] = 1-twoj2-twok2; ans[0][1] = twoij-twokw;   ans[0][2] = twojw+twoik;   ans[0][3] = v[0];
    ans[1][0] = twoij+twokw;   ans[1][1] = 1-twoi2-twok2; ans[1][2] = twojk-twoiw;   ans[1][3] = v[1];
    ans[2][0] = twoik-twojw;   ans[2][1] = twojk+twoiw;   ans[2][2] = 1-twoi2-twoj2; ans[2][3] = v[2];
    ans[3][0] = 0.0;           ans[3][1] = 0.0;           ans[3][2] = 0.0;           ans[3][3] = 1.0;

}

/* ----------------------------------------------------------------------
   Generate a 4x4 quaternion matrix corresponding to a rotation about vector
   u a magnitude theta;
   ------------------------------------------------------------------------- */
inline void MathExtra::axis_angle_to_mat_quat4(const double theta, const double *u, double ans[4][4]) {

    double thetaover2 = theta / 2.0;
    double sin_theta_2 = sin(thetaover2);

    double q0 = cos(thetaover2);
    double q1 = sin_theta_2 * u[0];
    double q2 = sin_theta_2 * u[1];
    double q3 = sin_theta_2 * u[2];

    ans[0][0] = q0;  ans[0][1] = q1;  ans[0][2] = q2;  ans[0][3] = q3;
    ans[1][0] = -q1; ans[1][1] = q0;  ans[1][2] = -q3; ans[1][3] = q2;
    ans[2][0] = -q2; ans[2][1] = q3;  ans[2][2] = q0;  ans[2][3] = -q1;
    ans[3][0] = -q3; ans[3][1] = -q2; ans[3][2] = q1;  ans[3][3] = q0;

}

/* ----------------------------------------------------------------------
   Multiply two 4x4 matrices together, return in ans
   ------------------------------------------------------------------------- */
inline void MathExtra::times4(const double m[4][4], const double m2[4][4], double ans[4][4]) {

    ans[0][0] = m[0][0] * m2[0][0] + m[0][1] * m2[1][0] + m[0][2] * m2[2][0] + m[0][3] * m2[3][0];
    ans[0][1] = m[0][0] * m2[0][1] + m[0][1] * m2[1][1] + m[0][2] * m2[2][1] + m[0][3] * m2[3][1];
    ans[0][2] = m[0][0] * m2[0][2] + m[0][1] * m2[1][2] + m[0][2] * m2[2][2] + m[0][3] * m2[3][2];
    ans[0][3] = m[0][0] * m2[0][3] + m[0][1] * m2[1][3] + m[0][2] * m2[2][3] + m[0][3] * m2[3][3];
    ans[1][0] = m[1][0] * m2[0][0] + m[1][1] * m2[1][0] + m[1][2] * m2[2][0] + m[1][3] * m2[3][0];
    ans[1][1] = m[1][0] * m2[0][1] + m[1][1] * m2[1][1] + m[1][2] * m2[2][1] + m[1][3] * m2[3][1];
    ans[1][2] = m[1][0] * m2[0][2] + m[1][1] * m2[1][2] + m[1][2] * m2[2][2] + m[1][3] * m2[3][2];
    ans[1][3] = m[1][0] * m2[0][3] + m[1][1] * m2[1][3] + m[1][2] * m2[2][3] + m[1][3] * m2[3][3];
    ans[2][0] = m[2][0] * m2[0][0] + m[2][1] * m2[1][0] + m[2][2] * m2[2][0] + m[2][3] * m2[3][0];
    ans[2][1] = m[2][0] * m2[0][1] + m[2][1] * m2[1][1] + m[2][2] * m2[2][1] + m[2][3] * m2[3][1];
    ans[2][2] = m[2][0] * m2[0][2] + m[2][1] * m2[1][2] + m[2][2] * m2[2][2] + m[2][3] * m2[3][2];
    ans[2][3] = m[2][0] * m2[0][3] + m[2][1] * m2[1][3] + m[2][2] * m2[2][3] + m[2][3] * m2[3][3];
    ans[3][0] = m[3][0] * m2[0][0] + m[3][1] * m2[1][0] + m[3][2] * m2[2][0] + m[3][3] * m2[3][0];
    ans[3][1] = m[3][0] * m2[0][1] + m[3][1] * m2[1][1] + m[3][2] * m2[2][1] + m[3][3] * m2[3][1];
    ans[3][2] = m[3][0] * m2[0][2] + m[3][1] * m2[1][2] + m[3][2] * m2[2][2] + m[3][3] * m2[3][2];
    ans[3][3] = m[3][0] * m2[0][3] + m[3][1] * m2[1][3] + m[3][2] * m2[2][3] + m[3][3] * m2[3][3];

}

/* ----------------------------------------------------------------------
   Take the transpose of the 4x4 matrix
   ------------------------------------------------------------------------- */
inline void MathExtra::transpose4(const double m[4][4], double ans[4][4]) {
    ans[0][0] = m[0][0]; ans[0][1] = m[1][0]; ans[0][2] = m[2][0]; ans[0][3] = m[3][0];
    ans[1][0] = m[0][1]; ans[1][1] = m[1][1]; ans[1][2] = m[2][1]; ans[1][3] = m[3][1];
    ans[2][0] = m[0][2]; ans[2][1] = m[1][2]; ans[2][2] = m[2][2]; ans[2][3] = m[3][2];
    ans[3][0] = m[0][3]; ans[3][1] = m[1][3]; ans[3][2] = m[2][3]; ans[3][3] = m[3][3];
}

/* ----------------------------------------------------------------------
   Multiply a 4x4 matrix against a 4x1 vector
   ------------------------------------------------------------------------- */
inline void MathExtra::matvec4(const double m[4][4], const double *v, double *ans) {
    ans[0] = m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2] + m[0][3] * v[3];
    ans[1] = m[1][0] * v[0] + m[1][1] * v[1] + m[1][2] * v[2] + m[1][3] * v[3];
    ans[2] = m[2][0] * v[0] + m[2][1] * v[1] + m[2][2] * v[2] + m[2][3] * v[3];
    ans[3] = m[3][0] * v[0] + m[3][1] * v[1] + m[3][2] * v[2] + m[3][3] * v[3];
}

/* ----------------------------------------------------------------------
   Return a 4x4 matrix necessary to translate the coordinates by a specified
   vector v
   ------------------------------------------------------------------------- */
inline void MathExtra::moveby(const double *v, double ans[4][4]) {

    ans[0][0] = 1;    ans[0][1] = 0;    ans[0][2] = 0;    ans[0][3] = v[0];
    ans[1][0] = 0;    ans[1][1] = 1;    ans[1][2] = 0;    ans[1][3] = v[1];
    ans[2][0] = 0;    ans[2][1] = 0;    ans[2][2] = 1;    ans[2][3] = v[2];
    ans[3][0] = 0;    ans[3][1] = 0;    ans[3][2] = 0;    ans[3][3] = 1;

}

/* ----------------------------------------------------------------------
   Return a 4x4 matrix necessary to translate the coordinates in the
   opposite direction specified by vector v
   ------------------------------------------------------------------------- */
inline void MathExtra::moveto(const double *v, double ans[4][4]) {

    ans[0][0] = 1;    ans[0][1] = 0;    ans[0][2] = 0;    ans[0][3] = -v[0];
    ans[1][0] = 0;    ans[1][1] = 1;    ans[1][2] = 0;    ans[1][3] = -v[1];
    ans[2][0] = 0;    ans[2][1] = 0;    ans[2][2] = 1;    ans[2][3] = -v[2];
    ans[3][0] = 0;    ans[3][1] = 0;    ans[3][2] = 0;    ans[3][3] = 1;

}

/* ----------------------------------------------------------------------
   Return a 4x4 identity matrix
   ------------------------------------------------------------------------- */
inline void MathExtra::identity4(double ans[4][4]) {

    ans[0][0] = 1;    ans[0][1] = 0;    ans[0][2] = 0;    ans[0][3] = 0;
    ans[1][0] = 0;    ans[1][1] = 1;    ans[1][2] = 0;    ans[1][3] = 0;
    ans[2][0] = 0;    ans[2][1] = 0;    ans[2][2] = 1;    ans[2][3] = 0;
    ans[3][0] = 0;    ans[3][1] = 0;    ans[3][2] = 0;    ans[3][3] = 1;
}

/* ----------------------------------------------------------------------
   copy the 4x4 matrix m to ans
   ------------------------------------------------------------------------- */
inline void MathExtra::copy4(double ans[4][4], const double m[4][4]) {

    ans[0][0] = m[0][0];    ans[0][1] = m[0][1];
    ans[1][0] = m[1][0];    ans[1][1] = m[1][1];
    ans[2][0] = m[2][0];    ans[2][1] = m[2][1];
    ans[3][0] = m[3][0];    ans[3][1] = m[3][1];

    ans[0][2] = m[0][2];    ans[0][3] = m[0][3];
    ans[1][2] = m[1][2];    ans[1][3] = m[1][3];
    ans[2][2] = m[2][2];    ans[2][3] = m[2][3];
    ans[3][2] = m[3][2];    ans[3][3] = m[3][3];

}

/* +-----------------+  */
/* | Zero a 4 vector |  */
/* +-----------------+  */

inline void MathExtra::zero4(double v[4]) {
    v[0] = 0.0;
    v[1] = 0.0;
    v[2] = 0.0;
    v[3] = 0.0;
}

/* +------------------+  */
/* | Zero a 4x4 array |  */
/* +------------------+  */

inline void MathExtra::zero4(double m[4][4]) {
    m[0][0] = 0.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = 0.0;
    m[1][0] = 0.0; m[1][1] = 0.0; m[1][2] = 0.0; m[1][3] = 0.0;
    m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 0.0; m[2][3] = 0.0;
    m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 0.0;
}

#endif
