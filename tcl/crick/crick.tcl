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

    ## Single Argument Flags, remove them as they are processed
    set n [llength $args]
    for {set i 0} {$i < $n} {incr i} {
        set arg [lindex $args $i]

        if {$arg == "-asymmetric"} {set params(asymmetric) 1; set args [lreplace $args $i $i]; continue}

        set n [llength $args]
    }

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

    catch {array unset params *}

    set params(nhelix) 2
    set params(nres) 28
    set params(pitch) 179
    set params(radius) 5.5
    set params(rpt) 3.64
    set params(rotation) 0.0
    set params(zoff) 0.0
    set params(z) 0.0
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
    set sys(userparams) {pitch radius rotation zoff z rpt square rpr}; #params to fit
    set sys(tol) 0.0001; # CG Tollerance
    set sys(orderflag) 0;
    set sys(TMPDIR) /tmp

    set sys(directselect) 0; ## Removed from the ccb library
}

## Reset everything to vanilla
proc ::crick::veryclean { args } {
    cleanup
    clearsys
    clearparams
}

## Create a mol and globalized selections
proc ::crick::newmol { args } {

    variable params
    variable sys

    if {$params(pitch) == 0.0} {set params(pitch) 0.000001}

    # Set Options
    set opts [list ccb -newmol\
                  -nhelix $params(nhelix)\
                  -nres $params(nres)\
                  -pitch $params(pitch)\
                  -radius $params(radius)\
                  -rpt $params(rpt)\
                  -rotation $params(rotation)\
                  -zoff $params(zoff)\
                  -Z $params(z)\
                  -square $params(square)\
                  -rpr $params(rpr)]

    if {$params(asymmetric)} {
        lappend opts "-asymmetric"
    }

    if {$params(antiparallel)} {
        lappend opts "-antiparallel"
    }

    ## The command used to generate the coiled-coil
    set sys(opts) [join $opts]

    # Generate structure
    if { [catch {eval $opts} props] } {
        vmdcon -err "ccb: Could not generate structure:\n $props"
        return -1
    }

    set natoms [llength $props]

    ## Create a new empty mol with natoms == length props
    if {[catch {mol new atoms $natoms} sys(ccbid)]} {
        vmdcon -err "ccb: could not create new molecule: $sys(ccbid)"
        return -1
    } else {
        animate dup $sys(ccbid)
    }

    ## Create a globalized selection for the entire coil
    ## delete the old one if it exists
    catch {$sys(sel_ccb_all) delete}

    set sys(sel_ccb_all) [atomselect $sys(ccbid) "all"]
    $sys(sel_ccb_all) global

    ## Set the properties for each atom
    $sys(sel_ccb_all) set {name resid resname chain segname x y z} $props

    ## identify bonds and reanalyze
    mol bondsrecalc $sys(ccbid)
    mol reanalyze $sys(ccbid)

    ## Set a default representation
    adddefaultrep $sys(ccbid)

    ## Create a globalized selection for the atoms to align
    catch {$sys(sel_ccb_align) delete}
    set sys(sel_ccb_align) [atomselect $sys(ccbid) $sys(aligntext)]
    $sys(sel_ccb_align) global
}

proc ::crick::updatemol { args } {

    variable params
    variable sys

    ## Make sure the mol still exists
    if {[lsearch [molinfo list] $sys(ccbid)] < 0} {
        vmdcon -err "ccb: Coiled-coil deleted."
        return -1
    }

    ## Make sure the pitch is never exactly zero
    if {$params(pitch) == 0.0} {set params(pitch) 0.000001}

    # Set Options
    set opts [list ccb\
                  -nhelix $params(nhelix)\
                  -nres $params(nres)\
                  -pitch $params(pitch)\
                  -radius $params(radius)\
                  -rpt $params(rpt)\
                  -rotation $params(rotation)\
                  -zoff $params(zoff)\
                  -Z $params(z)\
                  -square $params(square)\
                  -rpr $params(rpr)]

    if {$params(asymmetric)} {
        lappend opts "-asymmetric"
    }

    if {$params(antiparallel)} {
        lappend opts "-antiparallel"
    }

    ## Check for passing the selection directly or returning through
    ## tcl lists
    if {$sys(directselect)} {
        lappend opts -sel $sys(sel_ccb_all)
    } else {
        lappend opts "-xyz"
    }

    ## The command used to generate the coiled-coil
    set sys(opts) [join $opts]

    # Generate structure
    if { [catch {eval $opts} coords] } {
        vmdcon -err "ccb: Could not generate structure: $coords"
        return -1
    }

    ## Update the coordinates of the global selection if
    ## using tcl lists
    if {!$sys(directselect)} {
        $sys(sel_ccb_all) set {x y z} $coords
    }
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

    foreach {nter cter} $ter_index {
        set sel [atomselect $sys(molid)\
                     "($sys(usertext)) and index >= $nter and index <= $cter and name CA"]

        ## Residues
        set resids [$sel get residue]
        set resids [lsort -unique -integer -increasing $resids]
        lappend sys(resids) $resids

        ## Chains
        set chain [$sel get chain]
        lappend sys(chain) [lsort -unique $chain]

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

        lappend sys(crossangle) [expr {acos($angle)/3.14159*180}]
    }

    ## Calculate how the bundles are arranged
    ## when looking down the z-axis counterclockwise
    ## Calculate the angles between the centers of mass
    ## of the helices, the com of the bundle and com of helix 1
    set com1 [lindex $sys(com) 0]
    set com_avg [vecscale [vecadd {*}$sys(com)] [expr {1.0 / $sys(nhelix)}]]
    set r1 [vecnorm [vecsub $com_avg $com1]]
    set i 0
    foreach com2 $sys(com) chain $sys(chain) {

        set r2 [vecnorm [vecsub $com_avg $com2]]

        # Angle between r1_r2
        set angle [vecdot $r1 $r2]

        ## Watch out for floatng point error
        if {$angle > 1.000} {set angle [expr {$angle - 0.0001}]}
        if {$angle < -1.000} {set angle [expr {$angle + 0.0001}]}

        ## Angle between coms
        set angle [expr {acos($angle)}]

        ## Angle Direction
        set n [veccross $r1 $r2]
        set orient [vecdot $n [list 0 0 1]]

        if {$orient > 0.0} {
            set angle [expr {$angle + 3.1415926}]
        }

        lappend sys(order) [list $i $chain $angle]
        incr i
    }

    # Sort according to increasing angle, gives counterclockwisenn
    # order
    set sys(order) [lsort -increasing -real -index 2 $sys(order)]
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
        set params(order) [lsearch -all -index 2 -subindices -inline $sys(order) *]
    }

    ## Create a new mol consistent with determined topology
    resetmol
}


proc ::crick::run { args } {

    variable sys
    variable params

    # Get the topology of the inputted coiled-coil
    topology

    parray params
    parray sys

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
        set params(z)        [lrepeat $sys(nhelix) 0.0]
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

# emulate the behavior of loading a molecule through
# the regular "mol new" command. the options $selmod
# argument allows to append an additional modified to
# the selection, e.g. 'user > 0.1' for variable number
# particle xyz trajectories. (Thanks Axel!)
proc ::crick::adddefaultrep {mol {selmod none}} {
    mol color [mol default color]
    mol rep [mol default style]
    if {[string equal $selmod none]} {
        mol selection [mol default selection]
    } else {
        mol selection "([mol default selection]) and $selmod"
    }
    mol material [mol default material]
    mol addrep $mol
    display resetview
}
