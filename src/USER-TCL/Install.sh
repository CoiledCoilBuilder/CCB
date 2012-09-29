# Install/unInstall package files in LAMMPS
# do not install child files if parent does not exist

if (test $1 = 1) then

#  if (test -e ../angle_harmonic.cpp) then
#    cp angle_cg_cmm.h ..
#    cp angle_cg_cmm.cpp ..
#
#    cp angle_sdk.h ..
#    cp angle_sdk.cpp ..
#  fi


  cp tcl_ccb.cpp ..
  cp pkgIndex.tcl ..

elif (test $1 = 0) then

  rm -f ../tcl_ccb.cpp 
  rm -f ../pkgIndex.tcl

fi
