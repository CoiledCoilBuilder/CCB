.PHONY: install install-latest update

VMDPLUGINS = $(addprefix $(HOME)/,.vmdplugins)

update: install

install: install-ccbtools install-minimize install-crick

install-latest: install-latest-ccbtools install-latest-minimize install-latest-crick

clean-all:
	cd src; make clean-all 

clean-%:
	cd src; make clean-$(@:clean-%=%);

makeshlib-%:
	cd src; make yes-user-coiledcoil yes-user-tcl 
	cd src; make makeshlib
	cd src; make -f Makefile.shlib $(@:makeshlib-%=%);

install-%:
	cd $(join tcl/,$(@:install-%=%)); make install

install-latest-%:
	cd $(join tcl/,$(@:install-latest-%=%)); make install-latest

installshlib-%:
	mkdir -p $(VMDPLUGINS)/ccb 2>/dev/null || :
	cp src/libccb_$(@:installshlib-%=%).so $(VMDPLUGINS)/ccb/libccb.so 2>/dev/null ||:
	cp src/pkgIndex.tcl $(VMDPLUGINS)/ccb 2>/dev/null ||:

archive:
	git archive --prefix=cgtools/ HEAD -o ccbtools-latest.zip 
