# CCB multiple-machine Makefile
# More or less the same as the LAMMPS makefile
# routines

# Definitions

ROOT =  ccb
EXE =	$(ROOT)_$@
SRC =	$(wildcard *.cpp)
INC =	$(wildcard *.h)
OBJ = 	$(SRC:.cpp=.o)

# Package variables

PACKAGE = 

PACKUSER = user-coiledcoil

PACKALL = $(PACKAGE) $(PACKUSER)

PACKAGEUC = $(shell echo $(PACKAGE) | tr a-z A-Z)
PACKUSERUC = $(shell echo $(PACKUSER) | tr a-z A-Z)

YESDIR = $(shell echo $(@:yes-%=%) | tr a-z A-Z)
NODIR  = $(shell echo $(@:no-%=%) | tr a-z A-Z)

# List of all targets

help:
	@echo ''
	@echo 'make clean-all           delete all object files'
	@echo 'make clean-machine       delete object files for one machine'
	@echo ''
	@echo 'make package             list available packages'
	@echo 'make package-status      status of all packages'
	@echo 'make yes-package         install a single package in src dir'
	@echo 'make no-package          remove a single package from src dir'
	@echo 'make yes-all             install all packages in src dir'
	@echo 'make no-all              remove all packages from src dir'
	@echo 'make yes-standard        install all standard packages'
	@echo 'make no-standard         remove all standard packages'
	@echo 'make yes-user            install all user packages'
	@echo 'make no-user             remove all user packages'
	@echo ''
	@echo 'make makelib             create Makefile.lib'
	@echo 'make makeshlib           create Makefile.shlib'
	@echo 'make -f Makefile.lib     compile static lib: libccb.a'
	@echo 'make -f Makefile.shlib   compile shared object lib: libccb.so required for TCL'     
	@echo '' 
	@echo 'make package-update      replace src files with package files'
	@echo 'make package-overwrite   replace package files with src files'
	@echo 'make package-diff        diff src files against package files'
	@echo ''
	@echo 'make bench               benchmark menu'
	@echo 'make clean-bench         clean up benchmark files'
	@echo 'make bench-machine       bench a particular build'    
	@echo ''
	@echo 'make machine             build CCB where machine is one of:'
	@echo ''
	@files="`ls MAKE/Makefile.*`"; \
	  for file in $$files; do head -1 $$file; done
	@echo ''

# Build the code

.DEFAULT:
	@test -f MAKE/Makefile.$@
	@if [ ! -d Obj_$@ ]; then mkdir Obj_$@; fi
	@$(SHELL) Make.sh style
	@cp -p *.cpp *.h Obj_$@
	@cp MAKE/Makefile.$@ Obj_$@/Makefile
	@if [ ! -e Makefile.package ]; \
	  then cp Makefile.package.empty Makefile.package; fi
	@if [ ! -e Makefile.package.settings ]; \
	  then cp Makefile.package.settings.empty Makefile.package.settings; fi
	@cp Makefile.package Makefile.package.settings Obj_$@
	@cd Obj_$@; \
	$(MAKE) $(MFLAGS) "OBJ = $(OBJ)" "INC = $(INC)" \
	  "EXE = ../$(EXE)" ../$(EXE)
	@if [ -d Obj_$@ ]; then cd Obj_$@; rm -f $(SRC) $(INC) Makefile*; fi

# Remove machine-specific object files
clean:
	@echo 'make clean-all           delete all object files and documentation'
	@echo 'make clean-machine       delete object files for one machine'
	@echo 'make clean-docs          delete documentation files'

clean-all:
	rm -rf Obj_* latex $(ROOT)-docs.pdf doxygen.log

clean-docs:	
	rm -rf latex $(ROOT)-docs.pdf doxygen.log

clean-%:
	rm -rf Obj_$(@:clean-%=%)

# Update Makefile.lib and Makefile.list
makelib:                                                                                                                                         
	@$(SHELL) Make.sh style
	@$(SHELL) Make.sh Makefile.lib


makeshlib:
	@$(SHELL) Make.sh style
	@$(SHELL) Make.sh Makefile.shlib

# Package management

package:
	@echo 'Standard packages:' $(PACKAGE)
	@echo ''
	@echo 'User-contributed packages:' $(PACKUSER)
	@echo ''
	@echo 'make package             list available packages'
	@echo 'make package-status      status of all packages'
	@echo 'make yes-package         install a single package in src dir'
	@echo 'make no-package          remove a single package from src dir'
	@echo 'make yes-all             install all packages in src dir'
	@echo 'make no-all              remove all packages from src dir'
	@echo 'make yes-standard        install all standard packages'
	@echo 'make no-standard         remove all standard packages'
	@echo 'make yes-user            install all user packages'
	@echo 'make no-user             remove all user packages'
	@echo ''
	@echo 'make package-update      replace src files with package files'
	@echo 'make package-overwrite   replace package files with src files'
	@echo 'make package-diff        diff src files against package file'

yes-all:
	@for p in $(PACKALL); do $(MAKE) yes-$$p; done

no-all:
	@for p in $(PACKALL); do $(MAKE) no-$$p; done

yes-standard:
	@for p in $(PACKAGE); do $(MAKE) yes-$$p; done

no-standard:
	@for p in $(PACKAGE); do $(MAKE) no-$$p; done

yes-user:
	@for p in $(PACKUSER); do $(MAKE) yes-$$p; done

no-user:
	@for p in $(PACKUSER); do $(MAKE) no-$$p; done

yes-%:
	@if [ ! -e Makefile.package ]; \
	  then cp Makefile.package.empty Makefile.package; fi
	@if [ ! -e Makefile.package.settings ]; \
	  then cp Makefile.package.settings.empty Makefile.package.settings; fi
	@if [ ! -e $(YESDIR) ]; then \
	  echo "Package $(@:yes-%=%) does not exist"; \
	else \
	  echo "Installing package $(@:yes-%=%)"; \
	  cd $(YESDIR); $(SHELL) Install.sh 1; cd ..; $(SHELL) Depend.sh 1; \
	fi;

no-%:
	@if [ ! -e $(NODIR) ]; then \
	  echo "Package $(@:no-%=%) does not exist"; \
	else \
	  echo "Uninstalling package $(@:no-%=%)"; \
	  cd $(NODIR); $(SHELL) Install.sh 0; cd ..; $(SHELL) Depend.sh 0; \
        fi;

# status = list src files that differ from package files
# update = replace src files with newer package files
# overwrite = overwrite package files with newer src files
# diff = show differences between src and package files

package-status:
	@for p in $(PACKAGEUC); do $(SHELL) Package.sh $$p status; done
	@echo ''
	@for p in $(PACKUSERUC); do $(SHELL) Package.sh $$p status; done

package-update:
	@for p in $(PACKAGEUC); do $(SHELL) Package.sh $$p update; done
	@echo ''
	@for p in $(PACKUSERUC); do $(SHELL) Package.sh $$p update; done

package-overwrite:
	@for p in $(PACKAGEUC); do $(SHELL) Package.sh $$p overwrite; done
	@echo ''
	@for p in $(PACKUSERUC); do $(SHELL) Package.sh $$p overwrite; done

package-diff:
	@for p in $(PACKAGEUC); do $(SHELL) Package.sh $$p diff; done
	@echo ''
	@for p in $(PACKUSERUC); do $(SHELL) Package.sh $$p diff; done

# Benchmarks
.SUFFIXES:
.PHONY: input clean-bench bench docs

INPUTS_GCN4= gcn4.pdb ctlfile.gcn4 ctlfile.gcn4_2

$(INPUTS_GCN4): 
	ln -s ../benchmark/gcn4/$@ $@

INPUTS= $(INPUTS_GCN4) 

NUMTHREADS= 1 2 4 8 16 32

input: $(INPUTS)

clean-bench:
	rm -f ctlfile.gcn4 ctlfile.gcn4_2 gcn4*

bench:
	@echo 'Benchmarks'
	@echo ''
	@echo 'make bench             this menu'
	@echo 'make clean-bench       cleanup benchmark files'
	@echo 'make bench-machine     benchmark a particular build where machine is'
	@echo ''
	@files="`ls MAKE/Makefile.*`"; \
	  for file in $$files; do head -1 $$file; done
	@echo ''

#bench-%: $(INPUTS)
#	@test -f MAKE/Makefile.$(@:bench-%=%)
#	@if [ ! -d Obj_$(@:bench-%=%) ]; then mkdir Obj_$(@:bench-%=%); fi
#	@$(SHELL) Make.sh style
#	@cp -p *.cpp *.h Obj_$(@:bench-%=%)
#	@cp MAKE/Makefile.$(@:bench-%=%) Obj_$(@:bench-%=%)/Makefile
#	@if [ ! -e Makefile.package ]; \
#	  then cp Makefile.package.empty Makefile.package; fi
#	@if [ ! -e Makefile.package.settings ]; \
#	  then cp Makefile.package.settings.empty Makefile.package.settings; fi
#	@cp Makefile.package Makefile.package.settings Obj_$(@:bench-%=%)
#	@cd Obj_$(@:bench-%=%); \
#	$(MAKE) $(MFLAGS) "OBJ = $(OBJ)" "INC = $(INC)" \
#	  "EXE = ../$(ROOT)_$(@:bench-%=%)" ../$(ROOT)_$(@:bench-%=%)
#	@if [ -d Obj_$(@:bench-%=%) ]; then cd Obj_$(@:bench-%=%); rm -f $(SRC) $(INC) Makefile*; fi
#	@echo ========================================================  > compass.gcn4.time
#	@for p in $(NUMTHREADS); do \
#	echo OMP_NUM_THREADS=$$p compass_$(@:bench-%=%) ctlfile.gcn4   >> compass.gcn4.time; \
#	( time OMP_NUM_THREADS=$$p mpirun -np 1 ./compass_$(@:bench-%=%) ctlfile.gcn4 )  2>> compass.gcn4.time; \
#	echo ======================================================== >> compass.gcn4.time; \
#	done

docs: $(ROOT)-docs.pdf

$(ROOT)-docs.pdf:
	doxygen Doxyfile.in || rm -f $@ 
	make -C latex rm -f $@
	cp -p latex/refman.pdf $@ || rm -f $@

#Emacs on-the-fly syntax checker
check-syntax:
	$(CC) -o /dev/null $(SCADS_INC) $(CCFLAGS) -S ${CHK_SOURCES}
