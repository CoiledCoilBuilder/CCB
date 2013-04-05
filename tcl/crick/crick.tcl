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

package provide crick 1.0

package require ccb 1.0;       # For coiled-coil generation
package require topotools 1.2; # Determining chain topologies
package require minimize 1.0;  # Do CG fit between bundles

namespace eval ::crick:: {
    namespace export crick

    variable sys
    variable params
}

proc crick { args } {
    eval ::crick::crick $args
}

proc ::crick::crick { args } {

    variable sys;     # Procedure parameters
    variable params;  # Crick Parameters

    ## Cleanup past systems
    cleanup

    ## Clear up sys array
    clearsys

    # Default mol is the top one
    set sys(molid) [molinfo top]

    # Parse the passed arguments
    eval parse $args

    # Run the fit
    run

    return 0

}

proc ::crick::parse { args } {

    variable sys
    variable params

    set onearg {}
    ## Two Arguments Flags
    foreach {i j} $args {

        if {$i == "-sel"} {
            set sys(usersel) $j
            set sys(molid) [$j molid]
            set sys(usertext) [$j text]
            continue
        }

        if {$i == "-molid"} { set sys(molid) $j; continue}
        if {$i == "-aligntext"} {set sys(aligntext) $j; continue}
        if {$i == "-text"} {set sys(usertext) $j; continue}
        if {$i == "-params"} {set sys(userparams) [lsort -unique $j]; continue}
        if {$i == "-tol"} {set sys(tol) $j; continue}
        if {$i == "-order"} {set sys(orderflag) 1; set params(order) $j; continue}
    }

    ## Single Argument Flags
    foreach i $args {
        if {$i == "-asymmetric"} {set params(asymmetric) 1; continue}
    }
}

proc ::crick::newmol { args } {

    variable params
    variable sys

    ## Delete existing pdb
    catch {file delete $sys(TMPDIR)/ccb.pdb}

    # Set Options
    set opts [list ccb -pdb $sys(TMPDIR)/ccb.pdb\
                  -nhelix $params(nhelix)\
                  -nres $params(nres)\
                  -pitch $params(pitch)\
                  -radius $params(radius)\
                  -rpt $params(rpt)\
                  -rotation $params(rotation)\
                  -zoff $params(zoff)\
		  -Z $params(Z)\
                  -square $params(square)\
                  -rpr $params(rpr)\
                  -order $params(order)]

    if {$params(antiparallel)} {
        lappend opts "-antiparallel $sys(orient)"
    }

    if {$params(asymmetric)} {
        lappend opts "-asymmetric"
    }

    # Generate structure
    if { [catch {eval $opts} err] } {
        vmdcon -error "ccb: Could not generate structure:\n $err"
        return -1
    }

    ## Load into VMD
    if {[catch {mol new $sys(TMPDIR)/ccb.pdb} sys(ccbid)]} {
        vmdcon -error "ccb: could not create new molecule: $mol"
        return -1
    }

    ## Create a globalized selection for the entire coil
    catch {$sys(sel_ccb_all) delete}
    set sys(sel_ccb_all) [atomselect $sys(ccbid) "all"]
    $sys(sel_ccb_all) global

    ## Create a globalized selection for the atoms to align
    catch {$sys(sel_ccb_align) delete}
    set sys(sel_ccb_align) [atomselect $sys(ccbid) $sys(aligntext)]
    $sys(sel_ccb_align) global
}

proc ::crick::updatemol { args } {

    variable params
    variable sys

    # Set Options
    set opts [list ccb -vmd\
                  -nhelix $params(nhelix)\
                  -nres $params(nres)\
                  -pitch $params(pitch)\
                  -radius $params(radius)\
                  -rpt $params(rpt)\
                  -rotation $params(rotation)\
                  -zoff $params(zoff)\
                  -Z $params(Z)\
                  -square $params(square)\
                  -rpr $params(rpr)\
                  -order $params(order)]

    if {$params(antiparallel)} {
        lappend opts "-antiparallel $sys(orient)"
    }

    if {$params(asymmetric)} {
        lappend opts "-asymmetric"
    }

    # Generate structure
    if { [catch {eval $opts} coords] } {
        vmdcon -error "ccb: Could not generate structure"
        return -1
    }

    $sys(sel_ccb_all) set {x y z} $coords
}

proc ::crick::cleanup { args } {

    variable params
    variable sys

    catch {$sys(sel_ccb_all) delete}
    catch {$sys(sel_ccb_align) delete}
    catch {$sys(sel_user_align) delete}
    catch {mol delete $sys(ccbid)}
}

proc ::crick::resetmol { args } {

    variable params
    variable sys

    ## Delete the selection
    catch {$sys(sel_ccb_all) delete}
    catch {$sys(sel_ccb_align) delete}

    ## Make a new coiled-coil, keeping the old one
    newmol
}

## Clear the params
proc ::crick::clearparams { args } {

    variable params

    catch {array unset params}

    set params(nhelix) 2
    set params(nres) 28
    set params(pitch) 179
    set params(radius) 5.5
    set params(rpt) 3.64
    set params(rotation) 0.0
    set params(zoff) 0.0
    set params(Z) 0.0
    set params(square) 0.0
    set params(rpr) 1.5
    set params(antiparallel) 0
    set params(asymmetric) 0
    set params(order) {0 1}
    set params(orient) {0 0}

}

proc ::crick::clearsys { args } {

    variable sys

    catch {array unset sys}

    set sys(aligntext) "name CA"; # The atoms to align for RMSD
    set sys(usertext) "backbone"; # optional user text
    set sys(userparams) {pitch radius rotation zoff Z rpt square rpr}; #params to fit
    set sys(tol) 0.0001; # CG Tollerance
    set sys(orderflag) 0;
    set sys(TMPDIR) /tmp

}

## Reset everything to vanilla
proc ::crick::veryclean { args } {
    cleanup
    clearsys
    clearparams
}



# Determine the inputted coiled-coil topology
# including the orientation, order, helices
# and the number of residues per helix
proc ::crick::topology { args } {

    variable sys

    ## Make selection for main chain connectivity
    set sel [atomselect $sys(molid) "($sys(usertext)) and name N C CA"]

    ## Get bonded list
    set BL [topo -sel $sel getbondlist]
    $sel delete

    ## Loop over bond list to determine the number
    ## of unique chains. Assume there is at least 1 chain
    ## Looking for a break in C-N Connectivity
    ## This may not be completely robust
    ## Perhaps use VMDs "fragment" option

    ## list is {nter cter nter cter nter cter ...}
    set ter_index [lindex $BL {0 0}]
    foreach a $BL b [lrange $BL 1 end] {
        if {[expr { [lindex $a 1] != [lindex $b 0] }]} {
            lappend ter_index [lindex $a 1] [lindex $b 0]
        }
    }

    ## Trim the last empty id
    set ter_index [lrange $ter_index 0 end-1]
    set sys(terindex) $ter_index

    ## coiled-coil resids/chains/nres
    set sys(resids) {}
    set sys(chain) {}
    set sys(nres) {}
    set sys(direction) {}
    set sys(crossangle) {}
    set sys(order) {}
    set sys(com) {}
    set sys(nhelix) 0
    set sys(orient) 0

    ## Unique segment identifier index
    set segid 0

    ## Move the coiled-coil to the origin, and point the first
    ## helix's axis along +z, this is necessary so we
    ## we have a global frame of reference, e.g. coiled-coil
    ## axis along the Z-ordinate
    set sel [atomselect $sys(molid) "name CA"]
    set all [atomselect $sys(molid) "all"]
    set com [measure center $sel weight mass]
    $all moveby [vecinvert $com]
    $sel delete

    lassign $ter_index nter cter
    set sel [atomselect $sys(molid)\
                 "($sys(usertext)) and index >= $nter and index <= $cter and name CA"]

    set xyz [$sel get {x y z}]
    set nter_xyz [lindex $xyz 0]
    set cter_xyz [lindex $xyz end]
    $sel delete

    ## Move along x-axis
    set R [transvecinv [vecsub $cter_xyz $nter_xyz]]
    $all move $R

    ## Rotate about +y +90 to bring along +z
    set R [transaxis y 90 deg]
    $all move $R

    $all delete

    foreach {nter cter} $ter_index {
        set sel [atomselect $sys(molid)\
                     "($sys(usertext)) and index >= $nter and index <= $cter and name CA"]

        ## Residues
        set resids [$sel get residue]
        set resids [lsort -unique -integer -increasing $resids]
        lappend sys(resids) $resids

        ## Chains
        set chain [$sel get chain]
        set chain [lsort -unique $chain]
        lappend sys(chain) $chain

        ## Residues per
        lappend sys(nres) [llength $resids]

        ## Get the Nter-Cter direction vector
        set xyz [$sel get {x y z}]
        set nter_xyz [lindex $xyz 0]
        set cter_xyz [lindex $xyz end]
        lappend sys(direction) [vecsub $cter_xyz $nter_xyz]

        ## CA-Com of helices
        lappend sys(com) [measure center $sel]

        ## Number of helices
        incr sys(nhelix)

        ## Assign a unique segname for chain identification
        ## always consistent with the program, e.g. not
        ## dependent on how the user lettered the chains
        set segname "$chain$segid"
        $sel set segname $segname
        lappend sys(segname) $segname
        incr segid

        $sel delete
    }

    ## Get the axial orientations of each
    ## helix with respect to
    ## the first helix
    set r1 [lindex $sys(direction) 0]
    foreach r2 [lrange $sys(direction) 1 end] {
        set angle [vecdot [vecnorm $r1] [vecnorm $r2]]

        ## Watch out for floatng point error
        if {$angle > 1.000} {set angle [expr {$angle - 0.0001}]}
        if {$angle < -1.000} {set angle [expr {$angle + 0.0001}]}

        ## Check for parallel vs. antiparallel and specify
        ## if necessary in the orient list
        if {$angle < 0} {
            lappend sys(orient) 1
        } else {
            lappend sys(orient) 0
        }

        lappend sys(crossangle) [expr {acos($angle)/3.14159*180}]
    }

    ## Calculate how the bundles are arranged
    ## when looking down the coild coil axis counterclockwise
    ## Calculate the angles between the centers of mass
    ## of the helices, the com of the bundle and com of helix 1
    set com1 [lindex $sys(com) 0]
    set com_avg [vecscale [vecadd {*}$sys(com)] [expr {1.0 / $sys(nhelix)}]]

    set r1 [vecnorm [vecsub $com1 $com_avg]]
    set i 0
    foreach com2 $sys(com) segname $sys(segname) {

        set r2 [vecnorm [vecsub $com2 $com_avg]]

        set n [veccross $r1 $r2]
        set length [veclength $n]

        ## Get the angle with the correct direction
        set angle [expr {atan2($length, [vecdot $r1 $r2])}]

        # Make sure we've got the correct direction
        if {[lindex $n 2] < 0} {
            set angle [expr {$angle + 3.14159265}]
        }

        ## Make an associated list with index, segid and angle value
        lappend order [list $i $segname $angle]
        incr i
    }

    # Sort according to increasing angle, gives counterclockwise
    # order
    set order [lsort -increasing -real -index 2 $order]

    ## Assign an order index to each
    set i 0
    foreach x $order {
        lappend sys(order) [concat $x $i]
        incr i
    }

    ## Re-sort, so we have the output order counterclockwise
    set sys(order) [lsort -increasing -real -index 0 $sys(order)]

}

proc ::crick::setmol { args } {

    variable sys
    variable params

    set params(nres) $sys(nres)
    set params(nhelix) $sys(nhelix)

    ## Check if parallel/antiparallel
    foreach x $sys(crossangle) {
        if {$x > 90} {
            set params(antiparallel) 1
            break
        }
    }

    ## Set output order as determined from input structure
    ## Cool little hack to get columnwise from a list
    if {!$sys(orderflag)} {
        set params(order) [lsearch -all -index 0 -subindices -inline $sys(order) *]
    }

    ## Create a new mol consistent with determined topology
    resetmol
}


proc ::crick::run { args } {

    variable sys
    variable params

    # Get the topology of the inputted coiled-coil
    topology

    # Create new molecule consistent
    # with inputted coiled-coil
    setmol

    # Create global selection for input mol alignment
    catch {$sys(sel_user_all) delete}
    set sys(sel_user_align) [atomselect $sys(molid)\
                                 "($sys(usertext)) and $sys(aligntext)"]
    $sys(sel_user_align) global

    # Calculate initial RMSD
    rmsd

    ## if asymmetric, assume we want everything to be asymmetric
    if {$params(asymmetric)} {
        set params(rotation) [lrepeat $sys(nhelix) 0.0]
        set params(zoff)     [lrepeat $sys(nhelix) 0.0]
	set params(Z)        [lrepeat $sys(nhelix) 0.0]
        set params(rpt)      [lrepeat $sys(nhelix) 3.64]
	set params(square)   [lrepeat $sys(nhelix) 0.0]
    }

    # user specific minimization parameters
    set p {}
    set sys(params) {}

    foreach x $sys(userparams) {
        set p [concat $p $params($x)]

        ## Need this if we have asymmetric values
        set sys(params) [concat $sys(params)\
                             [lrepeat [llength $params($x)] $x]]
    }

    ## Dimension of minimization
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
    ::minimize::powell p xi $n $sys(tol) fret iter ::crick::compare
}

# Calculate the RMSD between
# the input structure and the
# generated structure

proc ::crick::rmsd { args } {

    variable sys

    # Calculate Alignment
    set R [measure fit $sys(sel_ccb_align) $sys(sel_user_align)]
    $sys(sel_ccb_all) move $R

    set rmsd [measure rmsd $sys(sel_ccb_align) $sys(sel_user_align)]
    set sys(rmsd) $rmsd

    return $rmsd
}

proc ::crick::compare x {

    variable sys
    variable params

    upvar $x crickvals

    ## Clear the parameters being used
    foreach type $sys(userparams) {
        set params($type) {}
    }

    ## Update the parameters in the array
    foreach type $sys(params) y $crickvals {
        lappend params($type) $y
    }

    ## Make sure rotations are [-180 180]
    set newtheta {}
    foreach theta $params(rotation) {
        while {$theta > 180} {set theta [expr {$theta - 360}]}
        while {$theta < -180} {set theta [expr {$theta + 360}]}
        lappend newtheta $theta
    }
    set params(rotation) $newtheta

    ## Update the structure
    updatemol

    ## Calculate the RMSD
    return [rmsd]
}
