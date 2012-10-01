#!/usr/bin/tclsh

## Minimization Routines
## Numerical Recipies 2nd Ed.

## Provided by Chris MacDermaid (Saven Group, University of Pennsylvania, MAY 2009)
## Version 1.0


package provide minimize 1.0

namespace eval ::minimize:: {
  namespace export minimize 

  ## MNBRAK
  variable GOLD 1.618034
  variable GLIMIT 100
  variable TINY [expr {1.0e-20}] 

  ## Brent
  variable ITMAX 100
  variable CGOLD 0.3819660
  variable ZEPS [expr {1.0e-10}]
  
  ## Downhill Simplex
  variable NMAX 250 

  ## Linmin / Powell / f1dim 
  variable TOL [expr {1.0e-10}]
  variable ITMAX_POWELL 200
  variable pcom
  variable xicom
  variable ncom
  variable nrfunc
}

proc ::minimize::do_banana {} {

  ## Rosenbrock banana function test

#  ## Generate trial points 
#  set p0 [list 1 -4]
#  set p1 [list 0 8]
#  set p2 [list -2 1]
#
#  ## Evaluate function at each point p
#  lappend y [banana $p0]  
#  lappend y [banana $p1]  
#  lappend y [banana $p2]  
#
#  set p [list $p0 $p1 $p2]
#
#  ## Run Optimization
#  amoeba $p $y 2 0.001 banana


#powell {p2 xi2 n ftol fret2 iter2 func}

   set p [list 1 4]
   set xi [list {1 0} {0 1}] 
   
   set tol 0.001 
   set iter {}
   set fret {}

   powell p xi 2 $tol fret iter banana  

   puts "$iter $fret"
   puts "$p $xi"
}


proc ::minimize::brent {ax2 bx2 cx2 tol func xmin2} {
  
  ## minimizes a function using brent's
  ## method
  ## Numerical Recipies 2nd ed. 404/405  

  ## Pass by reference
  upvar 1 $xmin2 xmin $ax2 ax $bx2 bx $cx2 cx
  
  variable ITMAX
  variable CGOLD
  variable ZEPS

  set e 0.0

  array set ca {} 

  ## sort inputs
  set a [expr {$ax < $cx ? $ax : $cx}] 
  set b [expr {$ax > $cx ? $ax : $cx}] 

  #initilize some variables
  set x $bx; set w $bx; set v $bx
  set fx [$func x]; set fw $fx; set fv $fx

  for {set iter 0} {$iter < $ITMAX} {incr iter} {
    
    set xm [expr {0.5 * ($a + $b)}]
    set tol1 [expr {$tol * (abs($x)+$ZEPS)}]
    set tol2 [expr {2.0 * $tol1}]
    
    if {[expr {abs($x - $xm) <= $tol2-0.5*($b-$a)}]} {
      # Test for doneness
      
#      puts "Itterations: $iter"
      
      set xmin $x  
      return $fx ; # the value of the function at x 
    }  
  
    if {[expr {abs($e) > $tol1}]} {
  
#      puts "$x $w $fx $fv" 
#      flush stdout
      
      ## Something goes wonky here
      set r [expr { ($x - $w) * ($fx - $fv) }]
      set q [expr { ($x - $v) * ($fx - $fw) }]
      set p [expr { ($x - $v)*$q - ($x - $w)*$r }]
      set q [expr { 2.0 * ($q - $r) }]   
  

      if {[expr {$q > 0.0}]} {set p [expr {-1*$p}]}   
      set q [expr {abs($q)}]

      set etemp $e
      set e $d

      if {[expr {abs($p) >= abs(0.5*$q*$etemp) ||\
          $p <= $q*($a - $x) ||\
          $p >= $q*($b - $x)}]} {
        
        set e [expr {$x >= $xm ? $a - $x : $b - $x}]
        set d [expr {$CGOLD * $e}]
      
      } else {

        set d [expr {$p / $q}]
        set u [expr {$x + $d}]

        if {[expr {$u - $a < $tol2 || $b - $u < $tol2}]} {
          set d [SIGN $tol1 [expr {$xm - $x}]]
        }
      }
    } else {

      set e [expr { ($x >= $xm) ? $a - $x : $b - $x}]  
      set d [expr {$CGOLD * $e}]
    }
   
   set u [expr {abs($d) >= $tol1 ? $x + $d : $x + [SIGN $tol1 $d]}]    
  
    ## Eval. function
    set fu [$func u]

   if {[expr {$fu <= $fx}]} {
      
      if {[expr {$u >= $x}]} {
        set a $x
      } else { 
        set b $x
      }

      SHFT v w x u
      SHFT fv fw fx fu

   } else { 
      
      if {[expr {$u < $x}]} {
        set a $u
      } else {
        set b $u
      }  
    
      if {[expr {$fu <= $fw || $w == $x}]} {
        set v $w
        set w $u
        set fv $fw
        set fw $fu
      } elseif {[expr {$fu <= $fv || $v == $x || $v == $w}]} {
        set v $u
        set fv $fu
      }  
   }
  
  }

  puts "Too many iterations in brent: $iter"
  set xmin $x  
  return $fx
}

proc ::minimize::mnbrak {ax2 bx2 cx2 fa2 fb2 fc2 func} {

  ## Brackets a minimum quickly
  ## Numerical Recipies 2nd ed. 400/401 

  upvar 1 $ax2 ax $bx2 bx $cx2 cx $fa2 fa $fb2 fb $fc2 fc

  variable GOLD
  variable GLIMIT
  variable TINY 

  set dum {};# just a dummy variable
  
  set fa [$func ax]
  set fb [$func bx]
  
  if {[expr {$fb > $fa}]} {
    SHFT dum ax bx dum
    SHFT dum fb fa dum
  }  

  set cx [expr {$bx + $GOLD*($bx - $ax)}]

  set fc [$func cx]

  while {[expr {$fb > $fc}]} {
    
    set r [expr { ($bx - $ax)*($fb - $fc) }] 
    set q [expr { ($bx - $cx)*($fb - $fa) }] 
    set u [expr { ($bx) - (($bx - $cx)*$q - ($bx - $ax)*$r)\
                  / (2.0 * [SIGN [FMAX [expr {abs($q - $r)}] $TINY] [expr {$q - $r}]])}]  

    set ulim [expr { ($bx) + $GLIMIT*($cx - $bx)}]  
    
    if {[expr { ($bx - $u) * ($u - $cx) > 0.0}]} {
      set fu [$func u]
      
      if {$fu < $fc} {
          set ax $bx
          set bx $u
          set fa $fb
          set fb $fu
          return 
      } elseif {[expr {$fu > $fb}]} {
          set cx $u
          set fc $fu
          return
      }

      set u [expr {$cx + $GOLD*($cx - $bx)}]
      set fu [$func u]
    
    } elseif {[expr {($cx - $u) * ($u - $ulim) > 0.0}]} {
        set fu [$func u]
        if {[expr {$fu < $fc}]} {
          
          set z [expr {$cx + $GOLD*($cx - $bx)}]
          SHFT bx cx u z 
          
          set func_u [$func u]
          SHFT fb fc fu func_u 
        
        }
    } elseif { [expr {(($u - $ulim)*($ulim - $cx)) >= 0.0}]} {
        set u $ulim
        set fu [$func u]
    } else {
      set u [expr {($cx) + $GOLD*($cx - $bx)}]
      set fu [$func u]
    }

    SHFT ax bx cx u
    SHFT fa fb fc fu
  }  

}

proc ::minimize::amoeba {p y ndim ftol funk} {

## Performs a multidimentional minimization
## using the downhill simplex method.

## Verified as working via Rosenbrock Banana Function.

## Speed enhancement may be possible
## by better porting to TCL.

  variable NMAX

  ## Counter for function evaluations 
  set nfunk 0
  set psum {} 
  set mpts [expr {$ndim +1}] 

  GET_PSUM p psum $ndim 


  while {1} {
    set ilo 1
    
   # Determine indicies of highest, next highest / lowest y values 
   ## Not sure if this is faster than the routine implemented in NR.
   set y_sort [lsort -increasing -real $y]  
   set ilo [lsearch -exact -real $y [lindex $y_sort 0]] 
   set inhi [lsearch -exact -real $y [lindex $y_sort end-1]] 
   set ihi [lsearch -exact -real $y [lindex $y_sort end]] 

   set rtol [expr {2.0 * abs( [lindex $y $ihi] - [lindex $y $ilo] )\
              / ( abs(  [lindex $y $ihi] ) + abs ( [lindex $y $ilo] ))}] 

   if {[expr {$rtol < $ftol}]} {
     
      #set low values to the first elements in p & y  
      SWAP_LIST y 0 $ilo
      SWAP_LIST p 0 $ilo

#      set y0 [lindex $y 0]
#      lset y 0 [lindex $y $ilo]
#      lset y $ilo $y0 
#
#      set a [lindex $p 0]
#      lset p 0 [lindex $p $ilo]
#      lset p $ilo $a
        
        break
        
   }
   
   if {$nfunk >= $NMAX} {puts "NMAX Exceeded"; break}

   incr nfunk 2

   set ytry [amotry p y psum $ndim $ihi -1.0 $funk]; #TRY 1 
   
   if {[expr {$ytry <= [lindex $y $ilo]}]} {

      set ytry [amotry p y psum $ndim $ihi 2.0 $funk]; #TRY 2 
   
   } elseif { [expr { $ytry >= [lindex $y $inhi] } ]} {

      set ysave [lindex $y $ihi]

      set ytry [amotry p y psum $ndim $ihi 0.5 $funk] ; # TRY 3

      if { [expr {$ytry >= $ysave}] } {

        for {set i 0} {$i < $mpts} {incr i} {
          if {[expr {$i != $ilo}]} {
            for {set j 0} {$j < $ndim} {incr j} {
              lset p [list $i $j] [expr { 0.5 * [lindex $p [list $i $j]] + [lindex $p [list $ilo $j]]}]
              lset psum $j [lindex $p [list $i $j]] 
            }
            
            lset y $i [$funk psum] 
            #lset y $i [banana $psum] 
          
          }  
        } 
         
         incr nfunk $ndim
         GET_PSUM p psum $ndim
      }
   
   } else { incr nfunk -1 }
 } 

puts $nfunk
}   

proc ::minimize::amotry {p y psum ndim ihi fac funk} {

  ## Pass by reference
  ## p = a
  ## psum = asum
  ## y = yy

  upvar 1 $p a $psum asum $y yy

  set fac1 [expr { (1.0 - $fac) / $ndim }]
  set fac2 [expr { $fac1 - $fac } ]

  set ptry {}

  for {set j 0} {$j < $ndim} {incr j} {
    lappend ptry [expr { [lindex $asum $j]*$fac1 - [lindex $a [list $ihi $j]] * $fac2}] 
  }
  
  ## Evaluate Function
  set ytry [$funk ptry] 

  if {[expr { $ytry < [lindex $yy $ihi]}]} {
    lset yy $ihi $ytry
    for {set j 0} {$j < $ndim} {incr j} {
      lset asum $j [expr { [lindex $asum $j] + ( [lindex $ptry $j] - [lindex $a [list $ihi $j]] ) }]
      lset a [list $ihi $j] [lindex $ptry $j]
    }
  }  
   
  return $ytry
}  

proc ::minimize::powell {p2 xi2 n ftol fret2 iter2 func} {
 # N-dimensional minimization using Powell's method  
 ## Verified as working via bananna function 

  variable ITMAX_POWELL


  upvar 1 $fret2 fret $iter2 iter $p2 p $xi2 xi

  set m [expr {$n - 1}]

  ## evaluate function at initial point p  
  set fret [$func p]
  
  ## Save initial point
  set pt $p
  
  for {set iter 0} {1} {incr iter} {
    set fp $fret
    set ibig 0
    set del 0.0
    
    for {set i 0} {$i < $n} {incr i} {
     ## get the i'th column for list xi
     set xit [lsearch -all -index $i -subindices -inline $xi *]
     set fptt $fret
     
     ## Minimize along it  
     linmin p xit $n fret $func 
     
     ## Is it the largest decrease?
     if {[expr { abs($fptt - $fret) > $del }]} {
      set del [expr { abs($fptt - $fret) }]
      set ibig $i
     }
    }   

    if {[expr { 2.0 * abs( $fp - $fret) <= $ftol*(abs($fp)+abs($fret))}]} {
      ## Terminate if tol is reached
      return
    }

    if {[expr { $iter == $ITMAX_POWELL }]} {puts "POWELL excedded maximum itterations: $ITMAX_POWELL"}
    
    set ptt {}
    for {set j 0} {$j < $n} {incr j} {
      lappend ptt [expr { 2.0 * [lindex $p $j] - [lindex $pt $j] }]
      lset xit $j [expr { [lindex $p $j] - [lindex $pt $j] }]
      lset pt $j [lindex $p $j]
    }  

    ## Eval function at extrapolated point
    set fptt [$func ptt]
    
    if {[expr { $fptt < $fp }]} {
      set t [expr { 2.0 * ($fp - 2.0 * $fret + $fptt) * pow($fp - $fret - $del,2) - $del*pow($fp - $fptt,2) }]
      if {[expr { $t < 0.0 }]} {
        linmin p xit $n fret $func
        for {set j 0} {$j < $n} {incr j} {
          lset xi [list $j $ibig] [lindex $xi [list $j $m]]
          lset xi [list $j $m] [lindex $xit $j]
        }
      }  
    }
  }
} 

proc ::minimize::linmin {p2 xi2 n fret2 func} {

  upvar 1 $p2 p $xi2 xi $fret2 fret 

  variable TOL
  
  ## These are passed to f1dim globally 
  variable pcom $p
  variable xicom $xi
  variable ncom $n
  variable nrfunc $func 

  set ax 0.0; set xx 1.0; set bx {}
  set fa {}; set fx {}; set fb {};
  set xmin {};

  mnbrak ax xx bx fa fx fb f1dim

  set fret [brent ax xx bx $TOL f1dim xmin]  
  
  
  for {set j 0} {$j < $n} {incr j} {
    lset xi $j [expr { [lindex $xi $j] * $xmin }]
    lset p $j [expr { [lindex $p $j] + [lindex $xi $j] }]
  }

}

proc ::minimize::f1dim x2 {
  
  variable pcom
  variable xicom
  variable ncom
  variable nrfunc

  upvar 1 $x2 x

  set xt {}
  
  for {set j 0} {$j < $ncom} {incr j} {
   lappend xt [expr { [lindex $pcom $j] + ($x * [lindex $xicom $j]) }] 
  }
 
  set f [$nrfunc xt]

  return $f
}
  

proc ::minimize::SHFT {a b c d} {
 
  upvar 1 $a w $b x $c y $d z 
  
  ## Uses pass by reference to shift variables. 
  ## the variables passed to this routine should be
  ## without a "$" as we're referencing the variables directly.

  set w $x
  set x $y
  set y $z

}

proc ::minimize::GET_PSUM {p psum ndim} {

  #pass list by reference
  upvar 1 $p a $psum asum

  ## clear psum
  set psum {} 
  
  ## returns element i of each sublist.  
  for {set i 0} {$i < $ndim} {incr i} {
    lappend asum [vecsum  [lsearch -all -index $i -subindices -inline $a *]]
  }

}

proc ::minimize::SQR a {
  return [expr { $a == 0 ? 0.0 : $a*$a }] 
}

proc ::minimize::SWAP {a b} {
  ## Swap variables a and b 
  upvar $a x $b y

  set swap x; set x $y; set y $swap
}  

proc ::minimize::SWAP_LIST {a i j} {
  ## Swap variables a and b 
  upvar $a x 

  #for list a, swap list i / j  

  set swap [lindex $x $i]; lset x $i [lindex $x $j]; lset x $j $swap
}

proc ::minimize::SIGN {a b} {
  return [expr {$b >= 0.0 ? abs($a) : -1*abs($a)}]
}

proc ::minimize::FMAX {a b} {
  return [expr {$a > $b ? $a : $b}]
}

proc ::minimize::banana Z2 {
  
  upvar 1 $Z2 Z

  lassign $Z x y
  
  return [expr { (1-$x)*(1-$x) + 100*($y - $x*$x)*($y - $x*$x) }]
}
