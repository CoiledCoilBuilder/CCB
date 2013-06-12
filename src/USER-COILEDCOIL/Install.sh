# Install/unInstall package files in LAMMPS
# do not install child files if parent does not exist

if (test $1 = 1) then

  cp backbone_coiledcoil.h ..
  cp backbone_coiledcoil.cpp ..

elif (test $1 = 0) then

  rm -f ../backbone_coiledcoil.h
  rm -f ../backbone_coiledcoil.cpp

fi
