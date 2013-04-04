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
 * @file   math_extra.cpp
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

#include "stdio.h"
#include "string.h"
#include "math_extra.h"

#define MAXJACOBI 50

namespace MathExtra {

    /* ----------------------------------------------------------------------
       output a matrix
       ------------------------------------------------------------------------- */

    void write3(const double mat[3][3])
    {
        for (unsigned i = 0; i < 3; i++) {
            for (unsigned j = 0; j < 3; j++) printf("%10.4g ",mat[i][j]);
            printf("\n");
        }
    }

    void write3(const double v[3])
    {
        for (unsigned i = 0; i < 3; i++) printf("%10.4g ",v[i]);
        printf("\n");
    }

    void write4(const double mat[4][4])
    {
        for (unsigned i = 0; i < 4; i++) {
            for (unsigned j = 0; j < 4; j++) printf("%10.4g ",mat[i][j]);
            printf("\n");
        }
    }


    /* ----------------------------------------------------------------------
       solve Ax = b or M ans = v
       use gaussian elimination & partial pivoting on matrix
       ------------------------------------------------------------------------- */

    int mldivide3(const double m[3][3], const double *v, double *ans)
    {
        // create augmented matrix for pivoting

        double aug[3][4];
        for (unsigned i = 0; i < 3; i++) {
            aug[i][3] = v[i];
            for (unsigned j = 0; j < 3; j++) aug[i][j] = m[i][j];
        }

        for (unsigned i = 0; i < 2; i++) {
            unsigned p = i;
            for (unsigned j = i+1; j < 3; j++) {
                if (fabs(aug[j][i]) > fabs(aug[i][i])) {
                    double tempv[4];
                    memcpy(tempv,aug[i],4*sizeof(double));
                    memcpy(aug[i],aug[j],4*sizeof(double));
                    memcpy(aug[j],tempv,4*sizeof(double));
                }
            }

            while (aug[p][i] == 0.0 && p < 3) p++;

            if (p == 3) return 1;
            else
                if (p != i) {
                    double tempv[4];
                    memcpy(tempv,aug[i],4*sizeof(double));
                    memcpy(aug[i],aug[p],4*sizeof(double));
                    memcpy(aug[p],tempv,4*sizeof(double));
                }

            for (unsigned j = i+1; j < 3; j++) {
                double m = aug[j][i]/aug[i][i];
                for (unsigned k=i+1; k<4; k++) aug[j][k]-=m*aug[i][k];
            }
        }

        if (aug[2][2] == 0.0) return 1;

        // back substitution

        ans[2] = aug[2][3]/aug[2][2];
        for (int i = 1; i >= 0; i--) {
            double sumax = 0.0;
            for (unsigned j = i+1; j < 3; j++) sumax += aug[i][j]*ans[j];
            ans[i] = (aug[i][3]-sumax) / aug[i][i];
        }

        return 0;
    }

    /* ----------------------------------------------------------------------
       compute evalues and evectors of 3x3 real symmetric matrix
       based on Jacobi rotations
       adapted from Numerical Recipes jacobi() function
       ------------------------------------------------------------------------- */

    int jacobi(double matrix[3][3], double *evalues, double evectors[3][3])
    {
        int i,j,k;
        double tresh,theta,tau,t,sm,s,h,g,c,b[3],z[3];

        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) evectors[i][j] = 0.0;
            evectors[i][i] = 1.0;
        }
        for (i = 0; i < 3; i++) {
            b[i] = evalues[i] = matrix[i][i];
            z[i] = 0.0;
        }

        for (int iter = 1; iter <= MAXJACOBI; iter++) {
            sm = 0.0;
            for (i = 0; i < 2; i++)
                for (j = i+1; j < 3; j++)
                    sm += fabs(matrix[i][j]);
            if (sm == 0.0) return 0;

            if (iter < 4) tresh = 0.2*sm/(3*3);
            else tresh = 0.0;

            for (i = 0; i < 2; i++) {
                for (j = i+1; j < 3; j++) {
                    g = 100.0*fabs(matrix[i][j]);
                    if (iter > 4 && fabs(evalues[i])+g == fabs(evalues[i])
                        && fabs(evalues[j])+g == fabs(evalues[j]))
                        matrix[i][j] = 0.0;
                    else if (fabs(matrix[i][j]) > tresh) {
                        h = evalues[j]-evalues[i];
                        if (fabs(h)+g == fabs(h)) t = (matrix[i][j])/h;
                        else {
                            theta = 0.5*h/(matrix[i][j]);
                            t = 1.0/(fabs(theta)+sqrt(1.0+theta*theta));
                            if (theta < 0.0) t = -t;
                        }
                        c = 1.0/sqrt(1.0+t*t);
                        s = t*c;
                        tau = s/(1.0+c);
                        h = t*matrix[i][j];
                        z[i] -= h;
                        z[j] += h;
                        evalues[i] -= h;
                        evalues[j] += h;
                        matrix[i][j] = 0.0;
                        for (k = 0; k < i; k++) rotate(matrix,k,i,k,j,s,tau);
                        for (k = i+1; k < j; k++) rotate(matrix,i,k,k,j,s,tau);
                        for (k = j+1; k < 3; k++) rotate(matrix,i,k,j,k,s,tau);
                        for (k = 0; k < 3; k++) rotate(evectors,k,i,k,j,s,tau);
                    }
                }
            }

            for (i = 0; i < 3; i++) {
                evalues[i] = b[i] += z[i];
                z[i] = 0.0;
            }
        }
        return 1;
    }

    /* ----------------------------------------------------------------------
       perform a single Jacobi rotation
       ------------------------------------------------------------------------- */

    void rotate(double matrix[3][3], int i, int j, int k, int l,
                double s, double tau)
    {
        double g = matrix[i][j];
        double h = matrix[k][l];
        matrix[i][j] = g-s*(h+g*tau);
        matrix[k][l] = h+s*(g-h*tau);
    }

    /* ----------------------------------------------------------------------
       Richardson iteration to update quaternion from angular momentum
       return new normalized quaternion q
       also returns updated omega at 1/2 step
       ------------------------------------------------------------------------- */

    void richardson(double *q, double *m, double *w, double *moments, double dtq)
    {
        // full update from dq/dt = 1/2 w q

        double wq[4];
        MathExtra::vecquat(w,q,wq);

        double qfull[4];
        qfull[0] = q[0] + dtq * wq[0];
        qfull[1] = q[1] + dtq * wq[1];
        qfull[2] = q[2] + dtq * wq[2];
        qfull[3] = q[3] + dtq * wq[3];
        MathExtra::qnormalize(qfull);

        // 1st half update from dq/dt = 1/2 w q

        double qhalf[4];
        qhalf[0] = q[0] + 0.5*dtq * wq[0];
        qhalf[1] = q[1] + 0.5*dtq * wq[1];
        qhalf[2] = q[2] + 0.5*dtq * wq[2];
        qhalf[3] = q[3] + 0.5*dtq * wq[3];
        MathExtra::qnormalize(qhalf);

        // re-compute omega at 1/2 step from m at 1/2 step and q at 1/2 step
        // recompute wq

        MathExtra::mq_to_omega(m,qhalf,moments,w);
        MathExtra::vecquat(w,qhalf,wq);

        // 2nd half update from dq/dt = 1/2 w q

        qhalf[0] += 0.5*dtq * wq[0];
        qhalf[1] += 0.5*dtq * wq[1];
        qhalf[2] += 0.5*dtq * wq[2];
        qhalf[3] += 0.5*dtq * wq[3];
        MathExtra::qnormalize(qhalf);

        // corrected Richardson update

        q[0] = 2.0*qhalf[0] - qfull[0];
        q[1] = 2.0*qhalf[1] - qfull[1];
        q[2] = 2.0*qhalf[2] - qfull[2];
        q[3] = 2.0*qhalf[3] - qfull[3];
        MathExtra::qnormalize(q);
    }

    /* ----------------------------------------------------------------------
       compute omega from angular momentum, both in space frame
       only know Idiag so need to do M = Iw in body frame
       ex,ey,ez are column vectors of rotation matrix P
       Mbody = P_transpose Mspace
       wbody = Mbody / Idiag
       wspace = P wbody
       set wbody component to 0.0 if inertia component is 0.0
       otherwise body can spin easily around that axis
       ------------------------------------------------------------------------- */

    void angmom_to_omega(double *m, double *ex, double *ey, double *ez,
                         double *idiag, double *w)
    {
        double wbody[3];

        if (idiag[0] == 0.0) wbody[0] = 0.0;
        else wbody[0] = (m[0]*ex[0] + m[1]*ex[1] + m[2]*ex[2]) / idiag[0];
        if (idiag[1] == 0.0) wbody[1] = 0.0;
        else wbody[1] = (m[0]*ey[0] + m[1]*ey[1] + m[2]*ey[2]) / idiag[1];
        if (idiag[2] == 0.0) wbody[2] = 0.0;
        else wbody[2] = (m[0]*ez[0] + m[1]*ez[1] + m[2]*ez[2]) / idiag[2];

        w[0] = wbody[0]*ex[0] + wbody[1]*ey[0] + wbody[2]*ez[0];
        w[1] = wbody[0]*ex[1] + wbody[1]*ey[1] + wbody[2]*ez[1];
        w[2] = wbody[0]*ex[2] + wbody[1]*ey[2] + wbody[2]*ez[2];
    }

    /* ----------------------------------------------------------------------
       compute omega from angular momentum
       w = omega = angular velocity in space frame
       wbody = angular velocity in body frame
       project space-frame angular momentum onto body axes
       and divide by principal moments
       ------------------------------------------------------------------------- */

    void mq_to_omega(double *m, double *q, double *moments, double *w)
    {
        double wbody[3];
        double rot[3][3];

        MathExtra::quat_to_mat(q,rot);
        MathExtra::transpose_matvec(rot,m,wbody);
        if (moments[0] == 0.0) wbody[0] = 0.0;
        else wbody[0] /= moments[0];
        if (moments[1] == 0.0) wbody[1] = 0.0;
        else wbody[1] /= moments[1];
        if (moments[2] == 0.0) wbody[2] = 0.0;
        else wbody[2] /= moments[2];
        MathExtra::matvec(rot,wbody,w);
    }

    /* ----------------------------------------------------------------------
       compute angular momentum from omega, both in space frame
       only know Idiag so need to do M = Iw in body frame
       ex,ey,ez are column vectors of rotation matrix P
       wbody = P_transpose wspace
       Mbody = Idiag wbody
       Mspace = P Mbody
       ------------------------------------------------------------------------- */

    void omega_to_angmom(double *w, double *ex, double *ey, double *ez,
                         double *idiag, double *m)
    {
        double mbody[3];

        mbody[0] = (w[0]*ex[0] + w[1]*ex[1] + w[2]*ex[2]) * idiag[0];
        mbody[1] = (w[0]*ey[0] + w[1]*ey[1] + w[2]*ey[2]) * idiag[1];
        mbody[2] = (w[0]*ez[0] + w[1]*ez[1] + w[2]*ez[2]) * idiag[2];

        m[0] = mbody[0]*ex[0] + mbody[1]*ey[0] + mbody[2]*ez[0];
        m[1] = mbody[0]*ex[1] + mbody[1]*ey[1] + mbody[2]*ez[1];
        m[2] = mbody[0]*ex[2] + mbody[1]*ey[2] + mbody[2]*ez[2];
    }

    /* ----------------------------------------------------------------------
       create unit quaternion from space-frame ex,ey,ez
       ex,ey,ez are columns of a rotation matrix
       ------------------------------------------------------------------------- */

    void exyz_to_q(double *ex, double *ey, double *ez, double *q)
    {
        // squares of quaternion components

        double q0sq = 0.25 * (ex[0] + ey[1] + ez[2] + 1.0);
        double q1sq = q0sq - 0.5 * (ey[1] + ez[2]);
        double q2sq = q0sq - 0.5 * (ex[0] + ez[2]);
        double q3sq = q0sq - 0.5 * (ex[0] + ey[1]);

        // some component must be greater than 1/4 since they sum to 1
        // compute other components from it

        if (q0sq >= 0.25) {
            q[0] = sqrt(q0sq);
            q[1] = (ey[2] - ez[1]) / (4.0*q[0]);
            q[2] = (ez[0] - ex[2]) / (4.0*q[0]);
            q[3] = (ex[1] - ey[0]) / (4.0*q[0]);
        } else if (q1sq >= 0.25) {
            q[1] = sqrt(q1sq);
            q[0] = (ey[2] - ez[1]) / (4.0*q[1]);
            q[2] = (ey[0] + ex[1]) / (4.0*q[1]);
            q[3] = (ex[2] + ez[0]) / (4.0*q[1]);
        } else if (q2sq >= 0.25) {
            q[2] = sqrt(q2sq);
            q[0] = (ez[0] - ex[2]) / (4.0*q[2]);
            q[1] = (ey[0] + ex[1]) / (4.0*q[2]);
            q[3] = (ez[1] + ey[2]) / (4.0*q[2]);
        } else if (q3sq >= 0.25) {
            q[3] = sqrt(q3sq);
            q[0] = (ex[1] - ey[0]) / (4.0*q[3]);
            q[1] = (ez[0] + ex[2]) / (4.0*q[3]);
            q[2] = (ez[1] + ey[2]) / (4.0*q[3]);
        }

        qnormalize(q);
    }

    /* ----------------------------------------------------------------------
       compute space-frame ex,ey,ez from current quaternion q
       ex,ey,ez = space-frame coords of 1st,2nd,3rd principal axis
       operation is ex = q' d q = Q d, where d is (1,0,0) = 1st axis in body frame
       ------------------------------------------------------------------------- */

    void q_to_exyz(double *q, double *ex, double *ey, double *ez)
    {
        ex[0] = q[0]*q[0] + q[1]*q[1] - q[2]*q[2] - q[3]*q[3];
        ex[1] = 2.0 * (q[1]*q[2] + q[0]*q[3]);
        ex[2] = 2.0 * (q[1]*q[3] - q[0]*q[2]);

        ey[0] = 2.0 * (q[1]*q[2] - q[0]*q[3]);
        ey[1] = q[0]*q[0] - q[1]*q[1] + q[2]*q[2] - q[3]*q[3];
        ey[2] = 2.0 * (q[2]*q[3] + q[0]*q[1]);

        ez[0] = 2.0 * (q[1]*q[3] + q[0]*q[2]);
        ez[1] = 2.0 * (q[2]*q[3] - q[0]*q[1]);
        ez[2] = q[0]*q[0] - q[1]*q[1] - q[2]*q[2] + q[3]*q[3];
    }

    /* ----------------------------------------------------------------------
       compute rotation matrix from quaternion
       quat = [w i j k]
       ------------------------------------------------------------------------- */

    void quat_to_mat(const double *quat, double mat[3][3])
    {
        double w2 = quat[0]*quat[0];
        double i2 = quat[1]*quat[1];
        double j2 = quat[2]*quat[2];
        double k2 = quat[3]*quat[3];
        double twoij = 2.0*quat[1]*quat[2];
        double twoik = 2.0*quat[1]*quat[3];
        double twojk = 2.0*quat[2]*quat[3];
        double twoiw = 2.0*quat[1]*quat[0];
        double twojw = 2.0*quat[2]*quat[0];
        double twokw = 2.0*quat[3]*quat[0];

        mat[0][0] = w2+i2-j2-k2;
        mat[0][1] = twoij-twokw;
        mat[0][2] = twojw+twoik;

        mat[1][0] = twoij+twokw;
        mat[1][1] = w2-i2+j2-k2;
        mat[1][2] = twojk-twoiw;

        mat[2][0] = twoik-twojw;
        mat[2][1] = twojk+twoiw;
        mat[2][2] = w2-i2-j2+k2;
    }

    /* ----------------------------------------------------------------------
       compute rotation matrix from quaternion conjugate
       quat = [w i j k]
       ------------------------------------------------------------------------- */

    void quat_to_mat_trans(const double *quat, double mat[3][3])
    {
        double w2 = quat[0]*quat[0];
        double i2 = quat[1]*quat[1];
        double j2 = quat[2]*quat[2];
        double k2 = quat[3]*quat[3];
        double twoij = 2.0*quat[1]*quat[2];
        double twoik = 2.0*quat[1]*quat[3];
        double twojk = 2.0*quat[2]*quat[3];
        double twoiw = 2.0*quat[1]*quat[0];
        double twojw = 2.0*quat[2]*quat[0];
        double twokw = 2.0*quat[3]*quat[0];

        mat[0][0] = w2+i2-j2-k2;
        mat[1][0] = twoij-twokw;
        mat[2][0] = twojw+twoik;

        mat[0][1] = twoij+twokw;
        mat[1][1] = w2-i2+j2-k2;
        mat[2][1] = twojk-twoiw;

        mat[0][2] = twoik-twojw;
        mat[1][2] = twojk+twoiw;
        mat[2][2] = w2-i2-j2+k2;
    }

    /**
     * @brief calculate 4x4 trans matrix that brings the given axis along v
     *
     * @param axis particular axis to bring along given vector v, 'x', 'y', 'z'
     * @param v bring specified axis along this vector
     */

    void axistovec(const char *axis, const double v[3], double m[4][4]) {

        double m1[4][4];
        double m2[4][4];
        double zero[3] = { 0.0 };

        // Set m to identity
        identity4(m);

        // Bring x along vector
        if (*axis == 'x' || *axis == 'X') {

            double y[3] = {0.0, 1.0, 0.0};
            double z[3] = {0.0, 0.0, 1.0};

            double theta = atan2(v[1], v[0]);
            double length = sqrt(v[1] * v[1] + v[0] * v[0]);
            double phi = atan2(v[2], length);

            // Generate transformation matrices
            axis_angle_to_mat_trans4(theta,z,zero,m1);
            axis_angle_to_mat_trans4(-phi,y,zero,m2);

            // Multiply out
            times4(m2,m1,m);

            // bring y along vector
        } else if (*axis == 'y' || *axis == 'Y') {

            double z[3] = {0.0, 0.0, 1.0};
            double x[3] = {1.0, 0.0, 0.0};

            double theta = atan2(v[2],v[1]);
            double length = sqrt(v[2] * v[2] + v[1] * v[1]);
            double phi = atan2(v[0], length);

            // Generate transformation matricies
            axis_angle_to_mat_trans4(theta,x,zero,m1);
            axis_angle_to_mat_trans4(-phi,z,zero,m2);

            // Multiply out
            times4(m2,m1,m);

            //bring z along vector
        } else if (*axis == 'z' || *axis == 'Z') {

            double x[3] = {1.0, 0.0, 0.0};
            double y[3] = {0.0, 1.0, 0.0};

            double theta = atan2(v[0],v[2]);
            double length = sqrt(v[0] * v[0] + v[2] * v[2]);
            double phi = atan2(v[1], length);

            // Generate transformation matricies
            axis_angle_to_mat_trans4(theta,y,zero,m1);
            axis_angle_to_mat_trans4(-phi,x,zero,m2);

            // Multiply out
            times4(m2,m1,m);

        } else {
            // Return m as identity
        }

    }

    /**
     * @brief calculate 4x4 trans matrix that being the vector along the given axis
     *
     * @param axis particular axis to bring along vector v, 'x', 'y', 'z'
     * @param v bring vector along particular axis v
     */

    void vectoaxis(const char *axis, const double v[3], double m[4][4]) {

        double m1[4][4];
        double m2[4][4];
        double zero[3] = { 0.0 };

        // Set m to identity
        identity4(m);

        // Bring vector along x
        if (*axis == 'x' || *axis == 'X') {

            double y[3] = {0.0, 1.0, 0.0};
            double z[3] = {0.0, 0.0, 1.0};

            double theta = atan2(v[1], v[0]);
            double length = sqrt(v[1] * v[1] + v[0] * v[0]);
            double phi = atan2(v[2], length);

            // Generate transformation matrices
            axis_angle_to_mat_trans4(phi,y,zero,m1);
            axis_angle_to_mat_trans4(-theta,z,zero,m2);

            // Multiply out
            times4(m2,m1,m);

        } else if (*axis == 'y' || *axis == 'Y') {

            double z[3] = {0.0, 0.0, 1.0};
            double x[3] = {1.0, 0.0, 0.0};

            double theta = atan2(v[2],v[1]);
            double length = sqrt(v[2] * v[2] + v[1] * v[1]);
            double phi = atan2(v[0], length);

            // Generate transformation matricies
            axis_angle_to_mat_trans4(phi,z,zero,m1);
            axis_angle_to_mat_trans4(-theta,x,zero,m2);

            // Multiply out
            times4(m2,m1,m);

        } else if (*axis == 'z' || *axis == 'Z') {

            double x[3] = {1.0, 0.0, 0.0};
            double y[3] = {0.0, 1.0, 0.0};

            double theta = atan2(v[0],v[2]);
            double length = sqrt(v[0] * v[0] + v[2] * v[2]);
            double phi = atan2(v[1], length);

            // Generate transformation matricies
            axis_angle_to_mat_trans4(phi,x,zero,m1);
            axis_angle_to_mat_trans4(-theta,y,zero,m2);

            // Multiply out
            times4(m2,m1,m);

        } else {
            // Return m as identity
        }

    }
}
