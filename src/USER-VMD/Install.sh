# Install/unInstall package files in LAMMPS
# edit 2 Makefile.package files to include/exclude package info

if (test $1 = 1) then

  if (test -e ../Makefile.package) then
    sed -i -e 's/[^ \t]*VMD[^ \t]* //g' ../Makefile.package
    sed -i -e 's|^PKG_INC =[ \t]*|&-I..\/USER-VMD\/srcinc |' ../Makefile.package
    sed -i -e 's|^PKG_INC =[ \t]*|&-I..\/USER-VMD\/include |' ../Makefile.package
  fi
  
  cp vmd_ccb.cpp ..

 elif (test $1 = 0) then

  if (test -e ../Makefile.package) then
    sed -i -e 's/[^ \t]*VMD[^ \t]* //g' ../Makefile.package
  fi

  rm -f ../vmd_ccb.cpp 

fi
