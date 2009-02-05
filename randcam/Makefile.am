bin_PROGRAMS = randcam

randcam_SOURCES = randcam.cpp entropy.cpp disclaimer.cpp Masher.cpp md5/md5.cpp

DEFAULT_INCLUDES = -I. -I..

AM_LDFLAGS = -lm @OPENCV_LDFLAGS@

AM_CPPFLAGS = @OPENCV_CPPFLAGS@


distclean-local:
	rm -f *.dat
