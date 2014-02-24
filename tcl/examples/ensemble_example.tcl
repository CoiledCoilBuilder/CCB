# -*-tcl-*-

# *hd +------------------------------------------------------------------------------------+
# *hd |  This file is part of Coiled-Coil Builder.                                         |
# *hd |                                                                                    |
# *hd |  Coiled-Coil Builder is free software: you can redistribute it and/or modify       |
# *hd |  it under the terms of the GNU General Public License as published by              |
# *hd |  the Free Software Foundation, either version 3 of the License, or                 |
# *hd |  (at your option) any later version.                                               |
# *hd |                                                                                    |
# *hd |  Coiled-Coil Builder is distributed in the hope that it will be useful,            |
# *hd |  but WITHOUT ANY WARRANTY without even the implied warranty of                     |
# *hd |  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                     |
# *hd |  GNU General Public License for more details.                                      |
# *hd |                                                                                    |
# *hd |  You should have received a copy of the GNU General Public License                 |
# *hd |  along with Coiled-Coil Builder.  If not, see <http:www.gnu.org/licenses/>.        |
# *hd +------------------------------------------------------------------------------------+

# *hd | If you intend to use this software for your research, please cite:
# *hd | and inform Chris MacDermaid <chris.macdermaid@gmail.com> of any pending publications.

# *hd | Copyright (c) 2012,2013,2014 by Chris M. MacDermaid <chris.macdermaid@gmail.com>
# *hd | and Jeffery G. Saven <saven@sas.upenn.edu>

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
