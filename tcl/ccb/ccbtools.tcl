## ccbtools Helper Tools and VMD GUI

package provide ccbtools 1.0

## Requires the ccb package
package require ccb

namespace eval ::ccbtools:: {
    namespace export ccbtools

    variable sys
    variable params

    if {[info exists env(TMPDIR)]} {
        set sys(TMPDIR) $env(TMPDIR)
    } else {
        set sys(TMPDIR) /tmp
    }

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

    ## Gui parameters
    set gui(nres_box) {1 200 1}
    set gui(rpt_box) {3.00 5.00 0.01}
    set gui(rotation_box) {-180.00 180.00 1.0}
    set gui(zoff_box) {-10.00 10.00 0.5}

    set gui(nres_scl) {1 200 1}
    set gui(rpt_scl) {3.00 5.00 5}
    set gui(rotation_scl) {-180.00 180.00 5}
    set gui(zoff_scl) {-10.00 10.00 5}
}

proc ccbgui { args } {
    eval ::ccbtools::gui $args
}

proc ::ccbtools::newmol { args } {

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
    if { [catch {eval $opts} err] } {
        vmdcon -error "ccb: Could not generate structure:\n $err"
        return -1
    }

    ## Load into VMD
    if {[catch {mol new $sys(TMPDIR)/ccb.pdb} sys(ccbid)]} {
        vmdcon -error "ccb: could not create new molecule: $sys(ccbid)"
        return -1
    }

    ## Create a globalized selection for the entire coil
    catch {$sys(sel_ccb_all) delete}
    set sys(sel_ccb_all) [atomselect $sys(ccbid) "all"]
    $sys(sel_ccb_all) global
}

proc ::ccbtools::updatemol { args } {

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
                  -square $params(square)\
                  -rpr $params(rpr)]

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

    ## The command used to generate the coiled-coil
    set sys(opts) [join $opts]

    $sys(sel_ccb_all) set {x y z} $coords
}

proc ::ccbtools::cleanup { args } {

    variable sys

    ## Delete the selection
    catch {$sys(sel_ccb_all) delete}

    ## Delete old mol
    catch {mol delete $sys(ccbid)}
}

proc ::ccbtools::resetmol { args } {

    variable sys
    variable params
    variable gui

    ## Cleanup
    cleanup

    ## Make a new coiled-coil, keeping the old one
    newmol
}

proc ::ccbtools::cmdwrap {args} {

    variable params

    lassign $args param val

    lset params($param) 0 $val

    if {$param == "nres"} {
        resetmol
    } else {
        updatemol
    }
}

proc ::ccbtools::asymcmdwrap {args} {

    ## Update the asymmetric parameters

    variable params

    lassign $args param idx val

    set params($param) [lset params($param) $idx $val]

    if {$param == "nres"} {
        resetmol
    } else {
        updatemol
    }
}

proc ::ccbtools::setasym {args} {

    variable params
    variable gui

    if {$params(asymmetric)} {

        ## Setup the necessary lists
        foreach x {nres rotation rpt zoff} {

            ## Adjust the array lengths...
            set N [llength $params($x)]

            while {$N <= $params(nhelix)} {
                lappend params($x) [lindex $params($x) 0]
                incr N
            }

            if {$N > $params(nhelix)} {
                set params($x) [lrange $params($x) 0 [expr {$params(nhelix) - 1}]]
            }
        }

    } else {

        ## Kill the lists, set to values of first helix
        foreach x {nres rotation rpt zoff} {
            set params($x) [lindex $params($x) 0]
        }

        ## Kill the asymmetric windows
        foreach x $gui(subwid) {
            destroy $x
        }
    }

    ## Reset the mol
    resetmol
}

## Actual GUI Stuff
proc ::ccbtools::asymbox {args} {

    variable sys
    variable params
    variable gui

    # Create a new window with the right number
    # of manipulators for each variable

    ## Check to make sure asymmetric is on
    if {!$params(asymmetric)} {
        return
    }

    ## Check for existing window
    if { [winfo exists $gui(wid).$args] } {
        wm deiconify $gui(wid).$args
        return
    }

    set wid [toplevel $gui(wid).$args]
    lappend gui(subwid) $wid

    wm title $wid $args
    wm resizable $wid 0 0

    ## Make Subwindow
    frame $wid.sub

    lassign $gui($args\_box) box_min box_max box_incr
    lassign $gui($args\_scl) scl_min scl_max scl_dgt

    for {set i 0} {$i < $params(nhelix)} {incr i} {
        #spinbox $wid.sub_box_$i -width 10 -from $box_min -to $box_max -increment $box_incr -format %10.2f\
                                                       #    -command [namespace code [list asymcmdwrap $args $i 2]]
        #grid $wid.sub_box_$i -row $i -column 3

        scale $wid.sub_scale_$i -label "$args $i:" -orient h -resolution 0 -digit $scl_dgt -from $scl_min\
            -to $scl_max -tickinterval 0 -length 300 -command [namespace code [list asymcmdwrap $args $i]]

        grid $wid.sub_scale_$i -row $i -column 1 -columnspan 2
    }
}

proc ::ccbtools::gui {args} {

    global M_PI

    variable wid
    variable sys
    variable gui

    ## Create a new mol
    newmol

    if { [winfo exists .ccb] } {
        wm deiconify $wid
        return
    }

    set wid [toplevel ".ccb"]
    wm title $wid "CCB"
    wm resizable $wid 0 0

    ##Main Window
    frame $wid.scales

    ##Sub Windows
    set gui(subwid) {}
    set gui(wid) $wid

    checkbutton $wid.scales.ap -text "Antiparallel" -variable ::ccbtools::params(antiparallel)\
        -onvalue 1 -offvalue 0 -command [namespace code updatemol]
    checkbutton $wid.scales.asym -text "Asymmetric" -variable ::ccbtools::params(asymmetric)\
        -onvalue 1 -offvalue 0 -command [namespace code setasym]

    button $wid.scales.new -text NEW -command [namespace code newmol]

    button $wid.scales.reset -text RESET -command [namespace code resetmol]

    button $wid.scales.update -text UPDATE -command [namespace code updatemol]

    ## Buttons for asymmetric commands
    button $wid.scales.asymN -text Asymmetric -command [namespace code {asymbox "nres"}]
    button $wid.scales.asymZ -text Asymmetric -command [namespace code {asymbox "zoff"}]
    button $wid.scales.asymC -text Asymmetric -command [namespace code {asymbox "rpt"}]
    button $wid.scales.asymH -text Asymmetric -command [namespace code {asymbox "rotation"}]

    spinbox $wid.scales.entM -width 10 -textvariable ccbtools::params(nhelix) -from 1 -to 20 -increment 1\
        -command [namespace code resetmol]

    spinbox $wid.scales.entN -width 10 -from 1 -to 200 -increment 1\
        -command [namespace code {cmdwrap "nres"}]

    spinbox $wid.scales.entP -width 10 -textvariable ccbtools::params(pitch) -from -1000.00 -to 1000.00 -increment 10.00 -format %10.2f\
        -command [namespace code updatemol]

    spinbox $wid.scales.entR -width 10 -textvariable ccbtools::params(radius) -from 0.01 -to 40.00 -increment 0.10 -format %10.2f\
        -command [namespace code updatemol]

    spinbox $wid.scales.entC -width 10 -from 3.00 -to 5.00 -increment 0.01 -format %10.2f\
        -command [namespace code {cmdwrap "rpt"}]

    spinbox $wid.scales.entH -width 10 -from -180.00 -to 180.00 -increment 1.0 -format %10.2f\
        -command [namespace code {cmdwrap "rotation"}]

    spinbox $wid.scales.entZ -width 10 -from -10.00 -to 10.00 -increment 1.0 -format %10.2f\
        -command [namespace code {cmdwrap "zoff"}]

    scale $wid.scales.sclM -label "Number of helices:" -orient h -digit 1 -from 1 -to 20\
        -tickinterval 0 -length 300 -command [namespace code resetmol]  -variable ccbtools::params(nhelix)

    scale $wid.scales.sclN -label "Number of residues:" -orient h -digit 1 -from 1 -to 200\
        -tickinterval 0 -length 300 -command [namespace code {cmdwrap "nres"}]

    scale $wid.scales.sclP -label "Pitch :" -orient h -resolution 0 -digit 5 -from -1000 -to 1000\
        -tickinterval 0 -length 300 -command [namespace code updatemol]  -variable ccbtools::params(pitch)

    scale $wid.scales.sclR -label "Radius :" -orient h -resolution 0 -digit 5 -from 0.01 -to 40.00\
        -tickinterval 0 -length 300 -command [namespace code updatemol]  -variable ccbtools::params(radius)

    scale $wid.scales.sclC -label "Residue Per Turn :" -orient h -resolution 0 -digit 5 -from 3 -to 5 \
        -tickinterval 0 -length 300 -command [namespace code {cmdwrap "rpt"}]

    scale $wid.scales.sclH -label "Helical Rotation :" -orient h -resolution 0 -digit 5 -from -180.00 -to 180.00\
        -tickinterval 0 -length 300 -command [namespace code {cmdwrap "rotation"}]

    scale $wid.scales.sclZ -label "Z-Offset:" -orient h -resolution 0 -digit 5 -from -10.00 -to 10.00\
        -tickinterval 0 -length 300 -command [namespace code {cmdwrap "zoff"}]

    entry $wid.ccbcommand -textvariable ccbtools::sys(opts) -width 60

    grid $wid.scales -row 0 -column 0
    grid $wid.scales.new    -row 8 -column 1  ;# New Button
    grid $wid.scales.update -row 8 -column 2  ;# Update Button
    grid $wid.scales.reset  -row 8 -column 3  ;# Reset Button
    grid $wid.scales.ap     -row 9 -column 1  ;# Antiparallel Checkbutton
    grid $wid.scales.asym     -row 9 -column 2  ;# asym button
    grid $wid.scales.sclM   -row 1 -column 1 -columnspan 2
    grid $wid.scales.sclN   -row 2 -column 1 -columnspan 2
    grid $wid.scales.sclP   -row 3 -column 1 -columnspan 2
    grid $wid.scales.sclR   -row 4 -column 1 -columnspan 2
    grid $wid.scales.sclC   -row 5 -column 1 -columnspan 2
    grid $wid.scales.sclH   -row 6 -column 1 -columnspan 2
    grid $wid.scales.sclZ   -row 7 -column 1 -columnspan 2

    grid $wid.scales.entM   -row 1  -column 3
    grid $wid.scales.entN   -row 2  -column 3
    grid $wid.scales.entP   -row 3  -column 3
    grid $wid.scales.entR   -row 4  -column 3
    grid $wid.scales.entC   -row 5  -column 3
    grid $wid.scales.entH   -row 6  -column 3
    grid $wid.scales.entZ   -row 7  -column 3

    grid $wid.ccbcommand    -row 10 -rowspan 3

    ## Asymmetric Buttons
    grid $wid.scales.asymN -row 2 -column 4;# nres asym Button
    grid $wid.scales.asymC -row 5 -column 4;# rpt asym Button
    grid $wid.scales.asymH -row 6 -column 4;# rotation asym Button
    grid $wid.scales.asymZ -row 7 -column 4;# zoff asym Button
}
