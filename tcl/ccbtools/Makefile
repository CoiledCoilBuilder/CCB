.PHONY: update install install-latest archive

PLUGINDIR = ccb
VMDPLUGINS = $(addprefix $(HOME)/,.vmdplugins)
INSTALLDIR = $(addprefix $(VMDPLUGINS)/,$(PLUGINDIR))

update: install

install:
	mkdir -p $(INSTALLDIR) 2>/dev/null || :
	rsync -avR --exclude-from="./.rsync-ignore" `git ls-files` $(INSTALLDIR) 

install-latest:
	mkdir -p $(INSTALLDIR) 2>/dev/null || :
	git archive HEAD | tar -x -C $(INSTALLDIR)

archive:
	git archive --prefix=minimize/ HEAD -o $(PLUGINDIR)-latest.zip 
