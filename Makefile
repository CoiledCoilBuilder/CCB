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
	@echo 'make archive-arch        Make an archive with all install'
	@echo '                         files in a single directory'
	@echo ''
	@echo 'make release-arch        Make an archive installable by make distrib'
	@echo ''
	@echo 'make distrib             install plugin to $(PLUGINDIR);'
	@echo ''
	@echo 'make arch                build libccb.so where arch is one of:'
	@echo ''
	@files="`ls src/MAKE/Makefile.*`"; \
	  for file in $$files; do head -1 $$file; done;
	@echo ''


.DEFAULT :
		cd src; make yes-user-coiledcoil yes-user-tcl &&\
		make makeshlib &&\
		make -f Makefile.shlib $@

clean-all :
	cd src; $(MAKE) clean-all

clean-% :
	cd src; $(MAKE) clean-$(@:clean-%=%)

distrib :
	for arch in `find . -name "libccb_*" -print |sed 's/.\/src\/libccb\_\(.*\).so/\1/g'`; do \
		dir=$(PLUGINDIR)/$$arch/tcl/$(CCBROOT)$(CCBVERSION);\
		mkdir -p $$dir;\
		cp src/lib$(CCBROOT)_$$arch\.so $$dir/libccb.so;\
		cp tcl/ccbtools/pkgIndex.tcl $$dir;\
		cp tcl/ccbtools/ccbtools.tcl $$dir;\
	done

# Make installable archive via copy/paste into VMD plugins directory
archive-% :
	arch=$(@:archive-%=%);\
	dir=$(CCBROOT)$(CCBVERSION);\
	$(MAKE) $$arch;\
	mkdir -p $$dir;\
	cp src/lib$(CCBROOT)_$$arch\.so $$dir/libccb.so;\
	cp tcl/ccbtools/pkgIndex.tcl $$dir;\
	cp tcl/ccbtools/ccbtools.tcl $$dir;\
	cp LICENSE $$dir;\
	cp README.md $$dir;\
	cp INSTALL $$dir;\
	zip -r $(CCBROOT)$(CCBVERSION)_$$arch\.zip $$dir;\
	rm -rf $$dir

# Make installable archive for use with make distrib directly
release-% :
	arch=$(@:release-%=%);\
	dir=$(CCBROOT)$(CCBVERSION)_$$arch;\
	$(MAKE) $$arch;\
	mkdir -p $$dir;\
	rsync -RKL src/lib$(CCBROOT)_$$arch\.so \
	tcl/ccbtools/pkgIndex.tcl \
	tcl/ccbtools/ccbtools.tcl \
	LICENSE $$dir \
	README.md $$dir \
    INSTALL $$dir \
	Makefile $$dir;\
	zip -r $(CCBROOT)$(CCBVERSION)_$$arch\.zip $$dir;\
	rm -rf $$dir
