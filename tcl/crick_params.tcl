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




