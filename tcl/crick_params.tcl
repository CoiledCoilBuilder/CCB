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

    crick -molid $molid -params {pitch radius rotation rpt zoff} -asymmetric

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

proc get_params_trajectory {psf dcd {stride 1}} {

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
  for {set i 0} {$i < N} {incr i $stride} {

    ## Update the frame
    molinfo top set frame $i

    crick -molid $molid -params {pitch radius rotation rpt zoff} -asymmetric

    $::crick::sys(sel_ccb_all) writepdb fits/$i\_$prefix\_fit.pdb

    set formatline {}
    set nfloat [expr {$crick::sys(nhelix) * 3}]
    
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
    ## ::crick::veryclean
  }

  ##clean up
  close $outfile
  ::crick::veryclean
  mol delete $molid
}

lassign $argv psf dcd
get_params_trajectory $psf $dcd
exit 0
