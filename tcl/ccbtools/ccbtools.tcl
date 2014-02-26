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

## ccbtools Helper Tools and VMD GUI

## Requires the ccb package
package require ccb

namespace eval ::ccbtools:: {
    namespace export ccbtools

    variable sys
    variable params

    variable version 1.0; #Plugin Version

    set sys(directselect) 0 ;# Enable it if using "ccb -sel $sel"

    ## Default parameters for the helix
    set params(nhelix) 2
    set params(nres) 28
    set params(pitch) 179
    set params(radius) 4.5
    set params(rpt) 3.64
    set params(rotation) 0.0
    set params(zoff) 0.0
    set params(z) 0.0
    set params(square) 0.0
    set params(rpr) 1.5
    set params(antiparallel) 0
    set params(asymmetric) 0
    set params(frasermacrae) 0

    ## Gui parameters
    set gui(nres_box) {1 200 1}
    set gui(rpt_box) {3.00 5.00 0.01}
    set gui(rotation_box) {-180.00 180.00 1.0}
    set gui(square_box) {-180.00 180.00 1.0}
    set gui(zoff_box) {-20.00 20.00 0.5}
    set gui(z_box) {-20.00 20.00 0.5}

    set gui(nres_scl) {1 200 1}
    set gui(rpt_scl) {3.00 5.00 5}
    set gui(rotation_scl) {-180.00 180.00 5}
    set gui(square_scl) {-180.00 180.00 5}
    set gui(zoff_scl) {-20.00 20.00 5}
    set gui(z_scl) {-20.00 20.00 5}
}


proc ccbgui { args } {
    eval ::ccbtools::gui $args
}

proc ::ccbtools::newmol { args } {

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

    if {$params(frasermacrae)} {
        lappend opts "-frasermacrae"
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
}

proc ::ccbtools::updatemol { args } {

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

    if {$params(frasermacrae)} {
        lappend opts "-frasermacrae"
    }

    ## Check for passing the selection directly or returning through
    ## tcl lists
    if {$sys(directselect)} {
        lappend opts -sel $sys(sel_ccb_all)
    } else {
        lappend opts "-vmd"
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

proc ::ccbtools::resetparams {} {

    variable params

    set params(nhelix) 2
    set params(nres) 28
    set params(pitch) 179
    set params(radius) 4.5
    set params(rpt) 3.64
    set params(rotation) 0.0
    set params(zoff) 0.0
    set params(z) 0.0
    set params(square) 0.0
    set params(rpr) 1.5
    set params(antiparallel) 0
    set params(asymmetric) 0
    set params(frasermacrae) 0

}

## Just changes the frist value,
## useful for sym commands like pitch radius
proc ::ccbtools::cmdwrap {args} {

    variable params

    lassign $args param val

    ## Make sure the val is an actual number
    if {[scan $val %f val] != 1} {set val 0}

    lset params($param) 0 $val

    if {$param == "nres"} {
        resetmol
    } else {
        updatemol
    }
}

## Changes all the asymmetric values symmetrically
proc ::ccbtools::symcmdwrap {args} {

    variable params

    lassign $args param val

    ## Make sure the val is an actual number
    if {[scan $val %f val] != 1} {set val 0}

    set params($param) [lrepeat $params(nhelix) $val]

    if {$param == "nres"} {
        resetmol
    } else {
        updatemol
    }
}

## Individual manipulation of asymmetric values
proc ::ccbtools::asymcmdwrap {args} {

    ## Update the asymmetric parameters

    variable params

    lassign $args param idx val

    ## Make sure the val is an actual number
    if {[scan $val %f val] != 1} {set val 0}

    set params($param) [lset params($param) $idx $val]

    if {$param == "nres"} {
        resetmol
    } else {
        updatemol
    }
}

## Turn on asymmetry and setup the lists
## Make sure that the list always has the correct number of elements
proc ::ccbtools::setasym {args} {

    variable params
    variable gui

    ## If we're doing asymmetric stuff, make lists
    if {$params(asymmetric)} {

        ## Setup the necessary lists with N values
        foreach x {nres rotation square rpt zoff z} {

            set N [llength $params($x)]

            ## Make longer
            while {$N < $params(nhelix)} {
                lappend params($x) [lindex $params($x) 0]
                incr N
            }

            ## Or make shorter
            if {$N > $params(nhelix)} {
                set params($x) [lrange $params($x) 0 [expr {$params(nhelix) - 1}]]
            }
        }

        #update the windows if necessary
        asymwid_update

    } else {

        ## If we're not asymmetric, only need the first value, but
        ## all fields should match for consistency

        ## Set to values of first helix
        foreach x {nres rotation square rpt zoff z} {
            set params($x) [lrepeat $params(nhelix) [lindex $params($x) 0]]
        }

        ## Kill the asymmetric windows
        foreach x $gui(subwid) {
            destroy $x
        }
        set gui(subwid) {}
    }
}

# emulate the behavior of loading a molecule through
# the regular "mol new" command. the options $selmod
# argument allows to append an additional modified to
# the selection, e.g. 'user > 0.1' for variable number
# particle xyz trajectories. (Thanks Axel!)
proc ::ccbtools::adddefaultrep {mol {selmod none}} {
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

## Actual GUI Stuff
proc ::ccbtools::mod_radius {args} {

    variable sys
    variable params
    variable gui

    ## Check for existing window
    if { [winfo exists $gui(wid).radius] } {
        wm deiconify $gui(wid).radius
        return
    }

    set wid [toplevel $gui(wid).radius]

    wm title $wid $args
    wm resizable $wid 0 0

    ## Make Subwindow
    frame $wid.sub

    ## Calculate the maximum number of residues in all helices
    lassign [lsort -decreasing -real $params(nres)] maxres

    ## Set radius list
    set curR [lindex $params(radius) 0]
    set params(radius) [list $curR $curR 1 1]

    ## scale limits
    lassign {5 0.01 40.00} scl_dgt scl_min scl_max
    lassign {0.10 0.01 40.00} box_incr box_min box_max

    spinbox $wid.sub_box_0 -width 10 -from $box_min -to $box_max -increment $box_incr -format %10.2f\
        -command [namespace code [list asymcmdwrap "radius" 0 %s]]

    spinbox $wid.sub_box_1 -width 10 -from $box_min -to $box_max -increment $box_incr -format %10.2f\
        -command [namespace code [list asymcmdwrap "radius" 1 %s]]

    spinbox $wid.sub_box_2 -width 10 -from $box_min -to $box_max -increment $box_incr -format %10.2f\
        -command [namespace code [list asymcmdwrap "radius" 2 %s]]

    spinbox $wid.sub_box_3 -width 10 -from $box_min -to $box_max -increment $box_incr -format %10.2f\
        -command [namespace code [list asymcmdwrap "radius" 3 %s]]

    scale $wid.sub_scl_0 -label "radius R_i:" -orient h -resolution 0 -digit $scl_dgt -from $scl_min\
        -to $scl_max -tickinterval 0 -length 300 -command [namespace code {asymcmdwrap "radius" 0}]

    scale $wid.sub_scl_1 -label "radius R_f:" -orient h -resolution 0 -digit $scl_dgt -from $scl_min\
        -to $scl_max -tickinterval 0 -length 300 -command [namespace code {asymcmdwrap "radius" 1}]

    scale $wid.sub_scl_2 -label "resid start:" -orient h -resolution 0 -digit 0 -from 1\
        -to $maxres -tickinterval 0 -length 300 -command [namespace code {asymcmdwrap "radius" 2}]

    scale $wid.sub_scl_3 -label "resid_end:" -orient h -resolution 0 -digit 0 -from 1\
        -to $maxres -tickinterval 0 -length 300 -command [namespace code {asymcmdwrap "radius" 3}]

    foreach i {0 1 2 3} {

        $wid.sub_box_$i set [lindex $params(radius) $i]
        $wid.sub_scl_$i set [lindex $params(radius) $i]

        grid $wid.sub_box_$i -row $i -column 3
        grid $wid.sub_scl_$i -row $i -column 1 -columnspan 2
    }
}

proc ::ccbtools::asymwid {args} {

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

        spinbox $wid.sub_box_$i -width 10 -from $box_min -to $box_max -increment $box_incr -format %10.2f\
            -command [namespace code [list asymcmdwrap $args $i %s]]
        $wid.sub_box_$i set [lindex $params($args) $i]

        scale $wid.sub_scale_$i -label "$args $i:" -orient h -resolution 0 -digit $scl_dgt -from $scl_min\
            -to $scl_max -tickinterval 0 -length 300 -command [namespace code [list asymcmdwrap $args $i]]
        $wid.sub_scale_$i set [lindex $params($args) $i]

        grid $wid.sub_box_$i -row $i -column 3
        grid $wid.sub_scale_$i -row $i -column 1 -columnspan 2
    }
}

## If we change the number of helices, the windows
## should reflect that change, this command is attached
## to params(nhelix) via a trace
proc ::ccbtools::asymwid_update {args} {

    variable gui

    foreach wid $gui(subwid) {
        lassign [split $wid "."] main sub param

        ## destroy the window
        destroy $wid

        ## Make a new window
        asymwid $param
    }
}

proc ::ccbtools::gui {args} {

    global M_PI

    variable params
    variable wid
    variable sys
    variable gui

    ## Set the coiled-coil params to their defaults
    resetparams

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

    checkbutton $wid.scales.fm -text "Fraser-MacRae" -variable ::ccbtools::params(frasermacrae)\
        -onvalue 1 -offvalue 0 -command [namespace code updatemol]

    button $wid.scales.new -text NEW -command [namespace code newmol]
    button $wid.scales.reset -text RESET -command [namespace code resetmol]
    button $wid.scales.update -text UPDATE -command [namespace code updatemol]

    ## Button for modulated radius
    button $wid.scales.modR -text Asymmetric -command [namespace code mod_radius]

    ## Buttons for asymmetric commands
    button $wid.scales.asymN -text Asymmetric -command [namespace code {asymwid "nres"}]
    button $wid.scales.azymZoff -text Asymmetric -command [namespace code {asymwid "zoff"}]
    button $wid.scales.azymZ -text Asymmetric -command [namespace code {asymwid "z"}]
    button $wid.scales.asymC -text Asymmetric -command [namespace code {asymwid "rpt"}]
    button $wid.scales.asymH -text Asymmetric -command [namespace code {asymwid "rotation"}]
    button $wid.scales.asymS -text Asymmetric -command [namespace code {asymwid "square"}]

    ## Spinboxes for main window
    spinbox $wid.scales.box_nhelix -width 10 -textvariable ccbtools::params(nhelix) -from 1 -to 10 -increment 1\
        -command [namespace code resetmol]

    spinbox $wid.scales.box_nres -width 10 -from 1 -to 100 -increment 1\
        -command [namespace code {symcmdwrap "nres" %s}]

    spinbox $wid.scales.box_pitch -width 10 -from -2000.00 -to 2000.00 -increment 10.00 -format %10.2f\
        -command [namespace code {cmdwrap "pitch" %s}]

    spinbox $wid.scales.box_radius -width 10 -from 0.01 -to 40.00 -increment 0.10 -format %10.2f\
        -command [namespace code {cmdwrap "radius" %s}]

    spinbox $wid.scales.box_rpt -width 10 -from 3.00 -to 5.00 -increment 0.01 -format %10.2f\
        -command [namespace code {symcmdwrap "rpt" %s}]

    spinbox $wid.scales.box_rotation -width 10 -from -180.00 -to 180.00 -increment 1.0 -format %10.2f\
        -command [namespace code {symcmdwrap "rotation" %s}]

    spinbox $wid.scales.box_square -width 10 -from -180.00 -to 180.00 -increment 1.0 -format %10.2f\
        -command [namespace code {symcmdwrap "square" %s}]

    spinbox $wid.scales.box_zoff -width 10 -from -20.00 -to 20.00 -increment 1.0 -format %10.2f\
        -command [namespace code {symcmdwrap "zoff" %s}]

    spinbox $wid.scales.box_z -width 10 -from -20.00 -to 20.00 -increment 1.0 -format %10.2f\
        -command [namespace code {symcmdwrap "z" %s}]

    ## Scales for main window
    scale $wid.scales.scl_nhelix -label "Number of helices:" -orient h -digit 1 -from 1 -to 10\
        -tickinterval 0 -length 300 -command [namespace code resetmol]  -variable ccbtools::params(nhelix)

    scale $wid.scales.scl_nres -label "Number of residues:" -orient h -digit 1 -from 1 -to 100\
        -tickinterval 0 -length 300 -command [namespace code {symcmdwrap "nres"}]

    scale $wid.scales.scl_pitch -label "Pitch :" -orient h -resolution 0 -digit 5 -from -2000 -to 2000\
        -tickinterval 0 -length 300 -command [namespace code {cmdwrap "pitch"}]

    scale $wid.scales.scl_radius -label "Radius :" -orient h -resolution 0 -digit 5 -from 0.01 -to 40.00\
        -tickinterval 0 -length 300 -command [namespace code {cmdwrap "radius"}]

    scale $wid.scales.scl_rpt -label "Residue Per Turn :" -orient h -resolution 0 -digit 5 -from 3 -to 5 \
        -tickinterval 0 -length 300 -command [namespace code {symcmdwrap "rpt"}]

    scale $wid.scales.scl_rotation -label "Helical Rotation :" -orient h -resolution 0 -digit 5 -from -180.00 -to 180.00\
        -tickinterval 0 -length 300 -command [namespace code {symcmdwrap "rotation"}]

    scale $wid.scales.scl_square -label "Square :" -orient h -resolution 0 -digit 5 -from -180.00 -to 180.00\
        -tickinterval 0 -length 300 -command [namespace code {symcmdwrap "square"}]

    scale $wid.scales.scl_zoff -label "Z-Offset (helices):" -orient h -resolution 0 -digit 5 -from -20.00 -to 20.00\
        -tickinterval 0 -length 300 -command [namespace code {symcmdwrap "zoff"}]

    scale $wid.scales.scl_z -label "Z-Offset (coiled-coil):" -orient h -resolution 0 -digit 5 -from -20.00 -to 20.00\
        -tickinterval 0 -length 300 -command [namespace code {symcmdwrap "z"}]

    ## Set initial params
    foreach x {nhelix nres pitch radius rpt rotation square zoff z} {
        $wid.scales.box_$x set [lindex $params($x) 0]
        $wid.scales.scl_$x set [lindex $params($x) 0]
    }

    entry $wid.ccbcommand -textvariable ccbtools::sys(opts) -width 60

    ## Buttons
    grid $wid.scales -row 0 -column 0
    grid $wid.scales.new    -row 10  -column 1  ;# New Button
    grid $wid.scales.update -row 10  -column 2  ;# Update Button
    grid $wid.scales.reset  -row 10  -column 3  ;# Reset Button
    grid $wid.scales.ap     -row 11  -column 1  ;# Antiparallel Checkbutton
    grid $wid.scales.asym   -row 11  -column 2  ;# asym button
    grid $wid.scales.fm     -row 11  -column 3  ;# Fraser-MacRae Constraint

    ##Scales
    grid $wid.scales.scl_nhelix     -row 1 -column 1 -columnspan 2
    grid $wid.scales.scl_nres       -row 2 -column 1 -columnspan 2
    grid $wid.scales.scl_pitch      -row 3 -column 1 -columnspan 2
    grid $wid.scales.scl_radius     -row 4 -column 1 -columnspan 2
    grid $wid.scales.scl_rpt        -row 5 -column 1 -columnspan 2
    grid $wid.scales.scl_rotation   -row 6 -column 1 -columnspan 2
    grid $wid.scales.scl_square     -row 7 -column 1 -columnspan 2
    grid $wid.scales.scl_zoff       -row 8 -column 1 -columnspan 2
    grid $wid.scales.scl_z          -row 9 -column 1 -columnspan 2

    ## Spinboxes
    grid $wid.scales.box_nhelix     -row 1  -column 3
    grid $wid.scales.box_nres       -row 2  -column 3
    grid $wid.scales.box_pitch      -row 3  -column 3
    grid $wid.scales.box_radius     -row 4  -column 3
    grid $wid.scales.box_rpt        -row 5  -column 3
    grid $wid.scales.box_rotation   -row 6  -column 3
    grid $wid.scales.box_square     -row 7  -column 3
    grid $wid.scales.box_zoff       -row 8  -column 3
    grid $wid.scales.box_z          -row 9  -column 3

    grid $wid.ccbcommand    -row 12 -rowspan 3

    ## Mod radius button
    grid $wid.scales.modR -row 4 -column 4;# nres asym Button

    ## Asymmetric Buttons
    grid $wid.scales.asymN    -row 2 -column 4;# nres asym Button
    grid $wid.scales.asymC    -row 5 -column 4;# rpt asym Button
    grid $wid.scales.asymH    -row 6 -column 4;# rotation asym Button
    grid $wid.scales.asymS    -row 7 -column 4;# square asym Button
    grid $wid.scales.azymZoff -row 8 -column 4;# zoff asym Button
    grid $wid.scales.azymZ    -row 9 -column 4;# z asym Button

    ## Help Menu
    frame $wid.scales.menubar -relief raised -bd 2 -padx 10
    grid  $wid.scales.menubar -padx 1 -column 0 -columnspan 5 -row 0 -sticky ew
    menubutton $wid.scales.menubar.help -text "Help" -underline 0 \
        -menu $wid.scales.menubar.help.menu
    $wid.scales.menubar.help config -width 5
    pack $wid.scales.menubar.help -side right
    menu $wid.scales.menubar.help.menu -tearoff no

    $wid.scales.menubar.help.menu add command -label "About" \ -command
    {tk_messageBox -type ok -title "About Coiled-Coil Builder" \
    -message "Tool for building coiled-coil structures.\n\nVersion
    $::ccbtools::version\n\n(c) 2012-2014 \nby Chris M. MacDermaid\n
    <chris.macdermaid@gmail.com>\nand\n Jeffery G.
    Saven\n<saven@sas.upenn.edu>"}

    $wid.scales.menubar.help.menu add command -label "Help..." \ -command
    "vmd_open_url [string trimright [vmdinfo www] /]/plugins/coiledcoil"

    ## Traces

    ## Put a trace on the nhelix param to update the asymmetric
    ## elements when the number of helices changes
    catch {trace vdelete ::ccbtools::params(nhelix) w ::ccbtools::setasym}
    trace var ::ccbtools::params(nhelix) w ::ccbtools::setasym
}

## Add the plugin to the extensions menu
vmd_install_extension ccb ::ccbgui "Modeling/CCB"
