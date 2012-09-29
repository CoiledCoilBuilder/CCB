## Instead of using annealing, this minimization
## is performed using conjugate directions implemented
## as powell's method.

package provide crick 1.0

package require ccb 1.0

namespace eval ::crick:: {
  namespace export crick 
}

proc crick { args } {
  eval ::crick::crick $args
}

proc ::crick::crick { args } {

    variable sys;     # Procedure parameters
    variable params;  # CCB parameters
    
    if {[info exists env(TMPDIR)]} {
	set sys(TMPDIR) $env(TMPDIR)
    } else {
	set sys(TMPDIR) /tmp
    }

  # Define initial params  
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

  # Generate mol
  newmol

    set params(R) 6.0

  # Update mol
  updatemol
    

}

proc ::crick::newmol { args } {

    variable params
    variable sys

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

    puts $opts

    # Generate structure
    if { [catch {eval $opts} err] } {
        vmdcon -error "ccb: Could not generate structure:\n $err"
        return -1
    }

    ## Load into VMD
    if {[catch {mol new $sys(TMPDIR)/ccb.pdb} sys(ccbid)]} {
	vmdcon -error "mergemols: could not create new molecule: $mol"
	return -1
    }

    ## Create a globalized selection
    set sys(sel_ccb_all) [atomselect $sys(ccbid) "all"]
    $sys(sel_ccb_all) global
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
	
crick
