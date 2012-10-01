package provide crick 1.0

package require ccb 1.0;       # For coiled-coil generation
package require topotools 1.2; # Determining chain topologies
package require minimize 1.0;  # Do CG fit between bundles

namespace eval ::crick:: {
    namespace export crick

    catch {array unset sys}
    catch {array unset params}

    variable sys
    set sys(aligntext) "name CA"; # The atoms to align for RMSD
    set sys(usertext) "all";      # optional user text
    set sys(userparams) {pitch radius rotation zoff rpt square rpr}; #params to fit
    set sys(tol) 0.0001; # CG Tollerance

    if {[info exists env(TMPDIR)]} {
        set sys(TMPDIR) $env(TMPDIR)
    } else {
        set sys(TMPDIR) /tmp
    }

    # Define initial params
    variable params;  # CCB parameters
    set params(nhelix) 2
    set params(nres) 28
    set params(pitch) 179
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
    variable params;  # Crick Parameters

    ## Cleanup past systems
    cleanup

    # Default mol is the top one
    set sys(molid) [molinfo top]

    # Set per-run defaults
    set params(asymmetric) 0
    set sys(tol) 0.0001; # CG Tollerance
    set sys(aligntext) "name CA"; # The atoms to align for RMSD
    set sys(usertext) "all";      # optional user text
 
    # Parse the passed arguments
    eval parse $args

    # Run the fit
    run
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
            set sys(usertext) [$j get text]
            continue
        }

        if {$i == "-molid"} { set sys(molid) $j; continue}
        if {$i == "-aligntext"} {set sys(aligntext) $j; continue}
        if {$i == "-params"} {set sys(userparams) [lsort -unique $j]; continue}
        if {$i == "-tol"} {set sys(tol) $j; continue}
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

# Determine the inputted coiled-coil topology
# including the orientation, order, helices
# and the number of residues per helix
proc ::crick::topology { args } {

    variable sys

    ## Make selection for main chain connectivity
    set sel [atomselect $sys(molid) "$sys(usertext) and name N C CA"]

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

    # Sort according to increasing angle, gives counterclockwise
    # order
    set sys(order) [lsort -increasing -real -index 2 $sys(order)]
    set sys(order) {1 2 3 0}
}

proc ::crick::setmol { args } {

    variable sys
    variable params

    # Set number of residues, make sure each chain is the same length
    set nres [lsort -unique -integer $sys(nres)]
    if {[llength $nres] > 1} {
        vmdcon -error "ccb:number of residues per helix is variable: $sys(nres)"
        return -1
    }

    set params(nres) $nres
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
    set params(order) [lsearch -all -index 0 -subindices -inline $sys(order) *]

    ## Create a new mol consistent with determined params
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
                                 "$sys(usertext) and $sys(aligntext)"]
    $sys(sel_user_align) global

    # Calculate initial RMSD
    rmsd

    ## if asymmetric, assume we want everything to be asymmetric
    if {$params(asymmetric)} {
      set params(rotation) [lrepeat $sys(nhelix) 0.0]
      set params(zoff)     [lrepeat $sys(nhelix) 0.0]
      set params(rpt)      [lrepeat $sys(nhelix) 3.64]
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
