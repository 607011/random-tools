PROJ=randomtools
VERSION=1.0
BUILD=$(PROJ)-$(VERSION)-linux

ACLOCAL_AMFLAGS = -I m4

SUBDIRS  = gen test 
SUBDIRS += ct-rng ct-rng-assessor ct-lotto

FILES  = ct-lotto/ct-lotto
FILES += ct-rng/assessor/ct-rng-assessor
FILES += ct-rng-analyze/ct-rng-analyzer
FILES += ct-rng/ct-rng

if BUILD_RANDCAM
SUBDIRS += ct-randcam
FILES += ct-randcam/ct-randcam
endif

if BUILD_CT_RNG_ANALYZER
SUBDIRS += ct-rng-analyzer
FILES += ct-rng-analyzer/ct-rng-analyzer
endif

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
