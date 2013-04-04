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

# Get the Crick Parameters for
# a particular input structure

package require crick 1.0

# For a particular file, in vmd
# do a crick fit, aggregate the 
# parameters, write an output structure
# and reset
proc get_params {pdb} {

    set prefix [file rootname [file tail $pdb]]
    set outfile [open "$prefix\_crick.out" w]

    set molid [mol new $pdb type pdb waitfor all]

    if {[catch {crick -molid $molid -params {pitch radius rotation rpt zoff} -asymmetric} errmsg]} {
	return $errmsg
    }

    $::crick::sys(sel_ccb_all) writepdb $prefix\_fit.pdb

    set formatline {}
    set nfloat [expr {$crick::sys(nhelix) * 3}]
    
    ## Variable number of rotations, rpts, zoffs
    set formatline [lrepeat $nfloat "%.4f"]

    ## Number of residues per chain
    set formatline [concat $formatline [lrepeat $crick::sys(nhelix) "%d"]]
    set formatline [join $formatline ","]

    puts $outfile "#!/usr/bin/tclsh8.5"
    puts $outfile [format "#RESULTS:,%s,%d,%.4f,%.4f,%.4f,$formatline"\
		       $prefix $crick::sys(nhelix) $crick::sys(rmsd)\
		       $crick::params(pitch)\
		       $crick::params(radius)\
		       {*}$crick::params(rotation)\
		       {*}$crick::params(rpt)\
		       {*}$crick::params(zoff)\
		       {*}$crick::sys(nres)]

    ## Output the arrays to a file so they can be DIRECTLY imported back in
    ## so structures can be regenerated and fits redone with optimized
    ## starting results, and to do data analysis if necessary
    puts $outfile "proc loadparams args \{"
    puts $outfile "namespace eval ::crick:: \{"
    puts $outfile "array set ::crick::sys \{[array get ::crick::sys]\}"
    puts $outfile "array set ::crick::params \{[array get ::crick::params]\}"
    puts $outfile "\}"
    puts $outfile "\}"

    ##clean up
    close $outfile
    ::crick::veryclean
    mol delete $molid
}

proc get_params_batch {indir} {

    glob pdbfile [glob -director $indir *.pdb]

    foreach pdb $pdbfile {
	get_params $pdb
    }
}

proc get_params_trajectory {psf dcd seltext {stride 1}} {

  ## For a trajectory, determine the 
  ## crick parameters for each frame

  ## Make sure we have pbc tools available
  package require pbctools

  catch {file mkdir fits}

  set prefix [file rootname [file tail $psf]]
  set outfile [open "$prefix\_crick.out" w]

  ## load the psf
  set molid [mol new $psf type psf waitfor all]

  ## Load the dcd file in its entirety 
  mol addfile $dcd type dcd waitfor all

  ## Unwrap the coordinates
  pbc unwrap -sel "not water"

  # Number of frames
  set N [molinfo top get numframes]

  ## Loop over frames getting the params for each one
  for {set i 0} {$i < $N} {incr i $stride} {

    ## Update the frame
    molinfo top set frame $i

    crick -molid $molid -params {pitch radius rotation rpt zoff} -text $seltext

    $::crick::sys(sel_ccb_all) writepdb fits/$i\_$prefix\_fit.pdb

    set formatline {}
    if {$::crick::params(asymmetric)} {
      set nfloat [expr {$crick::sys(nhelix) * 3}]
    } else {
      set nfloat 3
    }

    ## Variable number of rotations, rpts, zoffs
    set formatline [lrepeat $nfloat "%.4f"]

    ## Number of residues per chain
    set formatline [concat $formatline [lrepeat $crick::sys(nhelix) "%d"]]
    set formatline [join $formatline ","]

    puts $outfile [format "#RESULTS:,%s,%d,%d,%.4f,%.4f,%.4f,$formatline"\
                 $prefix $i $crick::sys(nhelix) $crick::sys(rmsd)\
                 $crick::params(pitch)\
                 $crick::params(radius)\
                 {*}$crick::params(rotation)\
                 {*}$crick::params(rpt)\
                 {*}$crick::params(zoff)\
                 {*}$crick::sys(nres)]

                 parray ::crick::sys
                 parray ::crick::params

    ## Cleanup after each frame
    ::crick::veryclean
  }

  ##clean up
  close $outfile
  ::crick::veryclean
  mol delete $molid
}

## For cli runs
if {0} {
       
        lassign $argv psf dcd

        set seltext "protein and resid 8 to 28"
        
        get_params_trajectory $psf $dcd $seltext 1 
}
