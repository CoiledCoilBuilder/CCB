#!/usr/bin/tclsh
# Get the helical parameters for a set of coordiantes.
# Minimizes the RMSD between the generated helix and the 
# coordinates of interest.

package provide fithelix 1.0

## We require the "minimize" package
## which is just NR in TCL
package require minimize 1.0

namespace eval ::fithelix:: {
  namespace export fithelix

  variable sys
  
  variable deg2rad 0.0174532925     
  variable rad2deg 57.2957795      
  
  catch {array unset sys}
  set sys(r) 2.3;     # Helix radius     
  set sys(omega) 100; # Helix frequency 
  set sys(phi) 0;     # Helix phase 
  set sys(z) 1.5;     # Helix axial rise
  set sys(rmsd) 1000; # RMSD between generated/true points

  #id1 = generated structure, id2 = real structure 
  set sys(id1) "trial"
  set sys(id2) "coords"
  set sys(color_$sys(id1)) 0
  set sys(color_$sys(id2)) 1 

  set sys(tol) 0.0001; # CG Tollerance
}

# x = coordinates in the format:
# {{x1 y1 z1} {x2 y2 z2} {x3 y3 z3}} 
# this is the way they come out of an:
# $sel get {x y z}

## Provides the "fithelix" command 
proc fithelix { args } {
   eval ::fithelix::fithelix $args
}

## The actual fit helix routine in the namespace
proc ::fithelix::fithelix args {

  ## Namespace globals
  variable sys

  ## Assign input variables, assume coordinate array
  lassign $args sys(x_$sys(id2)) 

  ## How many coordinates did we pass
  set sys(N) [llength $sys(x_$sys(id2))]

  ## Generate initial guess of helix coordinates
  helix_gen

  newmol $sys(id1) 
  newmol $sys(id2) 

  ## Calculate RMSD
  rmsd $sys(id1) $sys(id2)

  ## Set initial values to pass to opt 
  set p [list $sys(r) $sys(omega) $sys(phi) $sys(z)]

  ## Dimension of opt
  set n [llength $p]

  #Generate initial direction matrix
  ## using the identity matrix (n orthogonal vectors)
  #  e.g. {1 0 0}
  #       {0 1 0}
  #       {0 0 1}

  set xi [lrepeat $n [lrepeat $n 0]]

  for {set i 0} {$i < $n} {incr i} {
      lset xi [list $i $i] 1
  }

  set iter {}
  set fret {}

  ## Run Optimization
  ::minimize::powell p xi $n $sys(tol) fret iter ::fithelix::compare

  puts [format "RMSD:%.6f r:%.4f omega:%.4f phi:%.4f z:%.4f"\
        $fret {*}$p]
}

## Generate/update trial helical coordinates
proc ::fithelix::helix_gen {} {

  variable sys
  variable deg2rad

  set sys(x_$sys(id1)) {}  

  for {set i 0} {$i < $sys(N)} {incr i} {
        lappend sys(x_$sys(id1)) [list [expr {$sys(r) * cos(($sys(omega) * $deg2rad * $i) + $sys(phi))}]\
        [expr {$sys(r) * sin(($sys(omega) * $deg2rad * $i) + $sys(phi))}]\
        [expr {$sys(z) * $i}]
        ] 
 }
}

## Make a new molecule and assign coordinates
## to empty atoms, need to do this for both the
## trial coordinates and coordinates being fit
proc ::fithelix::newmol {id} {
        
  variable sys
 
  ## Make molcule with N atoms
  set sys(molid_$id) [mol new atoms $sys(N)]
  animate dup $sys(molid_$id) 
 
  ## Create an atomselection for N atoms
  catch {$sys(sel_$id) delete}
  set sys(sel_$id) [atomselect $sys(molid_$id) "all"]
  $sys(sel_$id) global

  ## Assign coordiantes
  $sys(sel_$id) set {x y z} $sys(x_$id)

  ## Set types and radii
  $sys(sel_$id) set name CA 
  $sys(sel_$id) set type CA 
  $sys(sel_$id) set chain A
  $sys(sel_$id) set radius 1.0 

  ## Add a representation for visualization
  addrep $id 
}

## Update the coordinates of the helices 
proc ::fithelix::updatemol {id} {

  variable sys
 
  ## Re-assign coordintes in array to atoms
  $sys(sel_$id) set {x y z} $sys(x_$id)
}

## calculate the RMSD between coordinates
proc ::fithelix::rmsd {id1 id2} {

  variable sys

  set R [measure fit $sys(sel_$id1) $sys(sel_$id2)]
  $sys(sel_$id1) move $R      

  set sys(rmsd) [measure rmsd $sys(sel_$id1) $sys(sel_$id2)]
  
  return $sys(rmsd)
}

## Compare the structures
proc ::fithelix::compare x {

  variable sys

  upvar $x vals
  lassign $vals sys(r) sys(omega) sys(phi) sys(z)

  helix_gen
  updatemol $sys(id1)  

  display update

  return [rmsd $sys(id1) $sys(id2)]
}

## Test the fitter with a randomly generated helix
proc ::fithelix::test {} {

  variable sys

  ## Set some random values for a test helix 
  #Int((MaxValue - MinValue + 1) * Rnd) + MinValue
  set sys(r) [expr {10 * rand() + 2}] 
  set sys(omega) [expr {((125 - 90 + 1) * rand()) + 90}] 
  set sys(z) [expr {3 * rand() + 1}] 
  set sys(phi) 0

  #set sys(r) 2 
  #set sys(omega) 130 
  #set sys(z) 8.0
  set sys(N) 30 

  puts "r= $sys(r)"
  puts "omega = $sys(omega)"
  puts "phi = $sys(phi)"
  puts "z = $sys(z)"

  ## Generate coordinates
  helix_gen

  ## set default helix values
  set sys(r) 2.3;     # Helix radius     
  set sys(omega) 100; # Helix frequency 
  set sys(phi) 0;     # Helix phase 
  set sys(z) 1.5;     # Helix axial rise

  ## Do the fit
  fithelix $sys(x_$sys(id1))      
}

proc ::fithelix::test_nievergelt {} { 

  set coords {{12 102 198} {48 138 180} {65 163 169}
              {77 187 157} {85 209 149} {94 266 128} 
              {93 288 120} {89 316 112} {82 347 107}
              {62 397 103}}

  ## set default helix values
  set sys(r) 2.3;     # Helix radius     
  set sys(omega) 100; # Helix frequency 
  set sys(phi) 0;     # Helix phase 
  set sys(z) 1.5;     # Helix axial rise

  ## Do the fit
  fithelix $coords
}

proc ::fithelix::reset {} {

  variable sys

  ## Delete global selections
  catch {$sys(sel_$sys(id1) delete}
  catch {$sys(sel_$sys(id2) delete}

  ## Delete mols
  catch {mol delete $sys(molid_$id1)}
  catch {mol delete $sys(molid_$id2)}

  ## Kill the array
  catch {array unset sys}

  set sys(r) 2.3;     # Helix radius     
  set sys(omega) 100; # Helix frequency 
  set sys(phi) 0;     # Helix phase 
  set sys(z) 1.5;     # Helix axial rise
  set sys(rmsd) 1000; # RMSD between generated/true points

  set sys(id1) "trial"
  set sys(id2) "coords"

  set sys(tol) 0.0001; # CG Tollerance
}

proc ::fithelix::addrep {id} {
  variable sys

  ## Add representations
  mol representation VDW 1.00000 8.000000
  mol color ColorID $sys(color_$id)
  mol material Opaque 
  mol selection "add"
  mol addrep $sys(molid_$id) 
}
