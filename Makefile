## Simple master makefile for CCB. A more advanced
## makefile is avaialable in src

.PHONY:

#PLUGINDIR = /opt/apps/vmd-latest/lib/plugins
PLUGINDIR = $(addprefix $(HOME)/,.vmdplugins)

CCBROOT=ccb
CCBVERSION=1.0

help :
	@echo ''
	@echo 'make clean-all           delete all object files'
	@echo 'make clean-arch          delete object files for particular arch'
	@echo ''
	@echo 'make distrib-arch        install plugin to $(PLUGINDIR); see arch list below '
	@echo ''
	@echo 'make arch                build libccb.so where arch is one of:'
	@echo ''
	@files="`ls src/MAKE/Makefile.*`"; \
	  for file in $$files; do head -1 $$file; done
	@echo ''


.DEFAULT :
		cd src; make yes-user-coiledcoil yes-user-tcl &&\
		make makeshlib &&\
		make -f Makefile.shlib $@

clean-all :
	cd src; $(MAKE) clean-all

clean-% :
	cd src; $(MAKE) clean-$(@:clean-%=%)

archive :
	git archive --prefix=$(CCBROOT)$(CCBVERSION)/ HEAD -o $(CCBROOT)$(CCBVERSION).zip

archive-% :
	arch=$(@:archive-%=%);\
	dir=$(CCBROOT)$(CCBVERSION)_$$arch;\
	$(MAKE) $$arch;\
	mkdir -p $$dir;\
	rsync -RKL src/lib$(CCBROOT).so \
	tcl/ccbtools/pkgIndex.tcl \
	tcl/ccbtools/ccbtools.tcl \
	Makefile $$dir;\

distrib-% :
	arch=$(@:distrib-%=%);\
	dir=`dirname $(PLUGINDIR)/lib_$$arch/tcl/$(CCBROOT)$(CCBVERSION)`;\
	mkdir -p $$dir;\
	cp src/lib$(CCBROOT).so $$dir;\
	cp tcl/ccbtools/pkgIndex.tcl $$dir;\
	cp tcl/ccbtools/ccbtools.tcl $$dir;\
