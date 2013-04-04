# -*- tcl -*-

  # +------------------------------------------------------------------------------------+ 
  # | This file is part of Coiled-Coil Builder.                                          | 
  # |                                                                                    | 
  # | Coiled-Coil Builder is free software: you can redistribute it and/or modify        | 
  # | it under the terms of the GNU General Public License as published by               | 
  # | the Free Software Foundation, either version 3 of the License, or                  | 
  # | (at your option) any later version.                                                | 
  # |                                                                                    | 
  # | Coiled-Coil Builder is distributed in the hope that it will be useful,             | 
  # | but WITHOUT ANY WARRANTY without even the implied warranty of                      | 
  # | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                      | 
  # | GNU General Public License for more details.                                       | 
  # |                                                                                    | 
  # | You should have received a copy of the GNU General Public License                  | 
  # | along with Coiled-Coil Builder.  If not, see <http:www.gnu.org/licenses/>.         | 
  # |                                                                                    | 
  # |  *cr                                                                               | 
  # |  *cr            (C) Copyright 1995-2013 The Board of Trustees of the               | 
  # |  *cr                        University of Pennsylvania                             | 
  # |  *cr                         All Rights Reserved                                   | 
  # |  *cr                                                                               | 
  # +------------------------------------------------------------------------------------+ 

# ensemble_example.tcl
# Generate an ensemble of antiparallel, D2 symmetric
# four-helix coiled coils. Each helix will have
# 28 residues.

# Load the coiled-coil builder shared library
load ../src/libccb.so

# Set our pdb output directory and create it
set outdir ensemble
file mkdir $outdir

# Initial pitch and radii (in angstroms)
set p 300;
set r 7.0

# Set our parameter deltas 
set dp -20 
set dr 0.2 

# Set the number of helices and residues
set nres 28
set nhelix 4

# A structure counter
set n 0 

# Generate structures over a range of 
# radii and pitches.
for {set i 0} {$i < 10} {incr i} {
  for {set j 0} {$j < 10} {incr j; incr n} {
   ccb -pdb $outdir/pdb_$n\.pdb\
       -pitch [expr {$p + $i * $dp}]\
       -radius [expr {$r + $j * $dr}]\
       -nhelix $nhelix -nres $nres -antiparallel
  }
}
