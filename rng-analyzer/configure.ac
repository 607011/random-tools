AC_INIT([ct-rng-analyzer], [1.0], [ola@ctmagazin.de])
AC_COPYRIGHT([Copyright (c) 2008 Oliver Lau, Heise Zeitschriften Verlag])

AM_INIT_AUTOMAKE([ct-rng-analyzer], [1.0], [ola@ctmagazin.de])
AM_CONFIG_HEADER([config.h])

AC_PROG_CC
AC_C_INLINE
AC_PROG_INSTALL
AC_PROG_LIBTOOL

AC_CHECK_LIB([probdist], [fdist_Unif])
AC_CHECK_LIB([testu01], [bbattery_Rabbit])
AC_CHECK_LIB([mylib], [ufile_CreateReadBin])

AC_ARG_ENABLE([debug],
              [AS_HELP_STRING([--enable-debug],[build debug version without optimization [no]])],
              [debug=$enableval],
              [debug=no])

AC_MSG_CHECKING([whether to build debug version (no optimization)])
if test x"$debug" = "xyes"; then
    AC_MSG_RESULT([yes])
    if test x"$ac_cv_c_compiler_gnu" = "xyes"; then
        CPPFLAGS="-DDEBUG -D_DEBUG $CPPFLAGS"
        DEFAULT_CXXFLAGS="-ggdb -O0 $DEFAULT_CXXFLAGS"
    fi
else
    AC_MSG_RESULT([no])
    AC_MSG_NOTICE( TARGET=$target )
    case $target in
    i*86-apple-*)
        if test x"$ac_cv_c_compiler_gnu" = "xyes"; then
            DEFAULT_CXXFLAGS="-g -march=prescott -ffast-math -fomit-frame-pointer $DEFAULT_CXXFLAGS"
        fi
        ;;
    i686-*-*)
        if test x"$ac_cv_c_compiler_gnu" = "xyes"; then
          DEFAULT_CXXFLAGS="-g -march=i686 -ffast-math -fomit-frame-pointer $DEFAULT_CXXFLAGS"
        fi
        ;;
    amd64-*-* | x86_64-*-*)
        if test x"$ac_cv_c_compiler_gnu" = "xyes"; then
          DEFAULT_CXXFLAGS="-g $DEFAULT_CXXFLAGS"
        fi
        ;;    
    powerpc-*-*)
        if test x"$ac_cv_c_compiler_gnu" = "xyes"; then
            DEFAULT_CXXFLAGS="-g -mcpu=G3 -mtune=G5 -fomit-frame-pointer $DEFAULT_CXXFLAGS"
        fi
        ;;
    *-*-*)
        if test x"$ac_cv_c_compiler_gnu" = "xyes"; then
            DEFAULT_CXXFLAGS="-fomit-frame-pointer $DEFAULT_CXXFLAGS"
        fi
        ;;
    esac
    CPPFLAGS="-DNDEBUG $CPPFLAGS"
    DEFAULT_CXXFLAGS="-O3 $DEFAULT_CXXFLAGS"
fi



AM_SANITY_CHECK
AC_OUTPUT(Makefile)
