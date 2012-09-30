package provide crick 1.0

package require ccb 1.0; # For coiled-coil generation
package require topotools 1.2; # Determining chain topologies

namespace eval ::crick:: {
    namespace export crick

    catch {array unset sys}
    catch {array unset params}

    variable sys
    set sys(aligntext) "name CA"; # The atoms to align for RMSD
    set sys(usertext) "all"; #optional user text

    if {[info exists env(TMPDIR)]} {
        set sys(TMPDIR) $env(TMPDIR)
    } else {
        set sys(TMPDIR) /tmp
    }

    # Define initial params
    variable params;  # CCB parameters
    set params(nhelix) 2
    set params(nres) 28
    set params(pitch) 180
    set params(radius) 4.5
    set params(rpt) 3.64
    set params(rotation) 0.0
    set params(zoff) 0.0
    set params(square) 0.0
    set params(rpr) 1.5
    set params(antiparallel) 0
    set params(asymmetric) 0
    set params(order) {0 1}
}

proc crick { args } {
    eval ::crick::crick $args
}

proc ::crick::crick { args } {

    variable sys;     # Procedure parameters

    # Default mol is the top one
    set sys(molid) [molinfo top]

    # Parse the passed arguments
    parse $args

    # Run the fit
    run
}

proc ::crick::parse { args } {

    foreach {i j} $args {
        if {$i == "-sel"} {
            set sys(usersel) $j
            set sys(molid) [$j molid]
            set sys(usertext) [$j get text]
            continue
        }
        if {$i == "-molid"} { set sys(molid) $j; continue}
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
                  -square $params(square)\
                  -rpr $params(rpr)\
                  -order $params(order)]

    if {$params(asymmetric)} {
        lappend opts "-asymmetric"
    }

    if {$params(antiparallel)} {
        lappend opts "-antiparallel"
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
    set sys(sel_ccb_all) [atomselect $sys(ccbid) "all"]
    $sys(sel_ccb_all) global

    ## Create a globalized selection for just the CA atoms

}

proc ::crick::updatemol { args } {

    variable params
    variable sys

    # Set Options
    # The number of residues or helices can't be changed
    # as this changes the number of atoms
    set opts [list ccb -vmd\
                  -nhelix $params(nhelix)\
                  -nres $params(nres)\
                  -pitch $params(pitch)\
                  -radius $params(radius)\
                  -rpt $params(rpt)\
                  -rotation $params(rotation)\
                  -zoff $params(zoff)\
                  -square $params(square)\
                  -rpr $params(rpr)\
                  -order $params(order)]

    if {$params(asymmetric)} {
        lappend opts "-asymmetric"
    }

    if {$params(antiparallel)} {
        lappend opts "-antiparallel"
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
    mol delete

}

proc ::crick::reset { args } {

    variable params
    variable sys

    ## Delete the selection
    catch {$sys(sel_ccb_all) delete}

    ## Delete Molecule
    if {[catch {mol delete $sys(ccbid)} err]} {
        vmdcon -error "ccb: could not delete molecule: $err"
        return -1
    }

    ## Make a new coiled-coil
    newmol
}

# Determine the inputted coiled-coil topology
# including the orientation, order, helices
# and the number of residues per helix
proc ::crick::topology { args } {

    variable sys

    ## Make selection for main chain connectivity
    set sel [atomselect $sys(molid) "name N C CA"]

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
                     "$sys(usertext) and index >= $nter and index <= $cter and name CA"]

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
        lappend sys(crossangle) [expr {acos([vecdot [vecnorm $r1] [vecnorm $r2]])/3.14159*180}]
    }

    ## Calculate how the bundles are arranged
    ## when looking down the z-axis counterclockwise
    ## Calculate the angles between the centers of mass
    ## of the helices, the com of the bundle and com of helix 1
    set com1 [lindex $sys(com) 0]
    set com_avg [vecscale [vecadd {*}$sys(com)] [expr {1.0 / $sys(nhelix)}]]
    set r1 [vecnorm [vecsub $com_avg $com1]]
    foreach com2 $sys(com) chain $sys(chain) {

            set r2 [vecnorm [vecsub $com_avg $com2]]
            set angle [expr {acos([vecdot $r1 $r2])}]
            set n [veccross $r1 $r2]
            set orient [vecdot $n [list 0 0 1]]

            if {$orient < 0.0} {
                set angle [expr {$angle + 3.14159}]
            }

            lappend sys(order) [list $chain $angle]
        }
    
    ## Sort according to increasing angle
    set sys(order) [lsort -increasing -real -index 1 $sys(order)]

}

proc ::crick::run { args } {

    # Get the topology of the coiled-coil
    topology

    #

}

proc ::crick::test { args } {

    variable params
    variable sys

    set params(nhelix) 4
    set params(radius) 7.5
    set params(antiparallel) 0

    newmol
    crick
}



