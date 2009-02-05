PROJ=randomtools
VERSION=1.1
BUILD=$(PROJ)-$(VERSION)-linux

ACLOCAL_AMFLAGS = -I m4

SUBDIRS  = gen test 
SUBDIRS += rng rng-assessor lotto

FILES  = lotto/lotto
FILES += rng/assessor/rng-assessor
FILES += rng/rng

if BUILD_RANDCAM
SUBDIRS += randcam
FILES += randcam/randcam
endif

if BUILD_RNG_ANALYZER
SUBDIRS += rng-analyzer
FILES += rng-analyzer/rng-analyzer
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
