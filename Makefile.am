SUBDIRS = gen test ct-randcam ct-rng-assessor ct-rng-analyzer ct-rng

PROJ=randomtools
VERSION=1.0
BUILD=$(PROJ)-$(VERSION)-linux
FILES := $(foreach i,ct-randcam ct-rng-assessor ct-rng-analyzer ct-rng,$i/$i)
MKDIR=mkdir
CP=cp
STRIP=strip
TAR=tar

bindist: 
	if [ -d "$(BUILD)" ] ; then $(RM) -R $(BUILD); fi
	$(MKDIR) $(BUILD)
	for i in $(FILES); do \
	  $(CP) "$${i}" $(BUILD) \
	; \
	done
	$(STRIP) $(BUILD)/*
	$(CP) LICENSE.txt "$(BUILD)"
	$(TAR) -czvf $(BUILD).tar.gz $(BUILD)/*

bindist-bzip2:
	if [ -d "$(BUILD)" ] ; then $(RM) -R $(BUILD); fi
	$(MKDIR) $(BUILD)
	for i in $(FILES); do \
	  $(CP) "$${i}" $(BUILD) \
	; \
	done
	$(STRIP) $(BUILD)/*
	$(CP) LICENSE.txt "$(BUILD)"
	$(TAR) -cjvf $(BUILD).tar.bz2 $(BUILD)/*
