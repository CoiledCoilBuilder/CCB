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


  cp plugin.h ..
  cp ccb_compass.h ..
  cp ccb_compass.cpp ..
  cp plugin_baseclass.h ..
  cp plugin_baseclass.cpp ..

elif (test $1 = 0) then

  rm -f ../plugin.h 
  rm -f ../ccb_compass.h
  rm -f ../ccb_compass.cpp
  rm -r ../plugin_baseclass.h
  rm -r ../plugin_baseclass.cpp

fi
