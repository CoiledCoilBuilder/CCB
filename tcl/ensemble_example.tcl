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
