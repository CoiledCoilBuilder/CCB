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

    ## Cleanup
    cleanup

    ## Make a new coiled-coil, keeping the old one
    newmol
}

proc ::ccbtools::gui {args} {

  global M_PI

  variable wid

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
 
  checkbutton $wid.scales.ap -text "Antiparallel" -variable ::ccbtools::params(antiparallel)\
        -onvalue 1 -offvalue 0 -command [namespace code updatemol] 
  checkbutton $wid.scales.asym -text "Asymmetric" -variable ::ccbtools::params(asymmetric)\
        -onvalue 1 -offvalue 0 -command [namespace code updatemol] 

  button $wid.scales.new -text NEW -command [namespace code newmol] 
  
  button $wid.scales.reset -text RESET -command [namespace code resetmol] 
  
  button $wid.scales.update -text UPDATE -command [namespace code updatemol] 
 
  spinbox $wid.scales.entM -width 10 -textvariable ccbtools::params(nhelix) -from 1 -to 20 -increment 1\
                           -command [namespace code resetmol]

  spinbox $wid.scales.entN -width 10 -textvariable ccbtools::params(nres) -from 1 -to 200 -increment 1\
                           -command [namespace code resetmol]

  spinbox $wid.scales.entP -width 10 -textvariable ccbtools::params(pitch) -from -1000.00 -to 1000.00 -increment 10.00 -format %10.2f\
                           -command [namespace code updatemol]

  spinbox $wid.scales.entR -width 10 -textvariable ccbtools::params(radius) -from 0.00 -to 40.00 -increment 0.10 -format %10.2f\
                           -command [namespace code updatemol]

  spinbox $wid.scales.entC -width 10 -textvariable ccbtools::params(rpt) -from 0.01 -to 5.00 -increment 0.01 -format %10.2f\
                           -command [namespace code updatemol]

  spinbox $wid.scales.entH -width 10 -textvariable ccbtools::params(rotation) -from -180.00 -to 180.00 -increment 1.0 -format %10.2f\
                           -command [namespace code updatemol]
 
  spinbox $wid.scales.entZ -width 10 -textvariable ccbtools::params(zoff) -from -100.00 -to 100.00 -increment 1.0 -format %10.2f\
                           -command [namespace code updatemol]

  scale $wid.scales.sclM -label "Number of helices:" -orient h -digit 1 -from 1 -to 20\
        -tickinterval 0 -length 300 -command [namespace code resetmol]  -variable ccbtools::params(nhelix)
 
  scale $wid.scales.sclN -label "Number of residues:" -orient h -digit 1 -from 1 -to 200\
        -tickinterval 0 -length 300 -command [namespace code resetmol] -variable ccbtools::params(nres) 
  
  scale $wid.scales.sclP -label "Pitch :" -orient h -resolution 0 -digit 5 -from -1000 -to 1000\
        -tickinterval 0 -length 300 -command [namespace code updatemol]  -variable ccbtools::params(pitch)
  
  scale $wid.scales.sclR -label "Radius :" -orient h -resolution 0 -digit 5 -from 0.00 -to 40.00\
        -tickinterval 0 -length 300 -command [namespace code updatemol]  -variable ccbtools::params(radius)
 
  scale $wid.scales.sclC -label "Residue Per Turn :" -orient h -resolution 0 -digit 5 -from 0 -to 5 \
        -tickinterval 0 -length 300 -command [namespace code updatemol]  -variable ccbtools::params(rpt)
  
  scale $wid.scales.sclH -label "Helical Rotation :" -orient h -resolution 0 -digit 5 -from -180.00 -to 180.00\
        -tickinterval 0 -length 300 -command [namespace code updatemol]  -variable ccbtools::params(rotation)
 
  scale $wid.scales.sclZ -label "Z-Offset:" -orient h -resolution 0 -digit 5 -from -100.00 -to 100.00\
        -tickinterval 0 -length 300 -command [namespace code updatemol]  -variable ccbtools::params(zoff)
 
  grid $wid.scales -row 0 -column 0
  grid $wid.scales.new    -row 8 -column 1  ;# New Button 
  grid $wid.scales.update -row 8 -column 2  ;# Update Button 
  grid $wid.scales.reset  -row 8 -column 3  ;# Reset Button 
  grid $wid.scales.ap     -row 9 -column 1  ;# Antiparallel Checkbutton 
 
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
}
