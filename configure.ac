AC_INIT([ctrandom], [1.0a], [oliver.lau@ct.heise.de])
AC_COPYRIGHT([Copyright (c) 2008 Oliver Lau, Heise Zeitschriften Verlag])

AM_INIT_AUTOMAKE([ctrandom], [1.0a], [oliver.lau@ct.heise.de])
AM_CONFIG_HEADER(config.h)


AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_LANG_CPLUSPLUS
AC_C_CONST
AC_C_INLINE
AC_PROG_CC

AC_HEADER_STDBOOL

AC_STRUCT_TM
AC_TYPE_SIZE_T

AC_CHECK_LIB([m], [pow])
AC_CHECK_LIB([ssl], [RAND_bytes])

AC_CHECK_LIB([cv], [cvSub])
AC_CHECK_LIB([highgui], [main])


#######################################################
#
# check for Padlock-capable processor
#
#######################################################
AC_MSG_CHECKING([for a Padlock unit on processor])
if grep -q "VIA Nehemiah" /proc/cpuinfo
then
	AC_MSG_RESULT([yes])
	AC_DEFINE([HAVE_PADLOCK], [yes], [VIA Nehemiah processor present.])
else
	AC_MSG_RESULT([no])
fi

AM_CONDITIONAL(HAVE_PADLOCK, test "$HAVE_PADLOCK" = yes)




#
# check for Boost library >= 1.37.0
#
AC_DEFUN([AX_BOOST],
[
AC_ARG_WITH([boost],
            AS_HELP_STRING([--with-boost=DIR],
            [use boost (default is yes) specify the root directory for boost library (optional)]),
            [
                if test "$withval" = "no"; then
                    want_boost="no"
                elif test "$withval" = "yes"; then
                    want_boost="yes"
                    ac_boost_path=""
                else
                    want_boost="yes"
                    ac_boost_path="$withval"
                fi
            ],
            [want_boost="yes"]
)

if test "x$want_boost" = "xyes"; then
   AC_REQUIRE([AC_PROG_CC])
   boost_lib_version_req=ifelse([$1], ,1.37.0,$1)
   boost_lib_version_req_shorten=`expr $boost_lib_version_req : '\([[0-9]]*\.[[0-9]]*\)'`
   boost_lib_version_req_major=`expr $boost_lib_version_req : '\([[0-9]]*\)'`
   boost_lib_version_req_minor=`expr $boost_lib_version_req : '[[0-9]]*\.\([[0-9]]*\)'`
   boost_lib_version_req_sub_minor=`expr $boost_lib_version_req : '[[0-9]]*\.[[0-9]]*\.\([[0-9]]*\)'`
   if test "x$boost_lib_version_req_sub_minor" = "x" ; then
        boost_lib_version_req_sub_minor="0"
   fi
   WANT_BOOST_VERSION=`expr $boost_lib_version_req_major \* 100000 \+  $boost_lib_version_req_minor \* 100 \+ $boost_lib_version_req_sub_minor`
   AC_MSG_CHECKING(for boostlib >= $boost_lib_version_req)
   succeeded=no
   if test "$ac_boost_path" != ""; then
       BOOST_LDFLAGS="-L$ac_boost_path/lib"
       BOOST_CPPFLAGS="-I$ac_boost_path/include"
   else
       for ac_boost_path_tmp in /usr /usr/local /opt ; do
           if test -d "$ac_boost_path_tmp/include/boost" && test -r "$ac_boost_path_tmp/include/boost"; then
               BOOST_LDFLAGS="-L$ac_boost_path_tmp/lib"
               BOOST_CPPFLAGS="-I$ac_boost_path_tmp/include"
               break;
           fi
       done
   fi
   CPPFLAGS_SAVED="$CPPFLAGS"
   CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"
   export CPPFLAGS
   LDFLAGS_SAVED="$LDFLAGS"
   LDFLAGS="$LDFLAGS $BOOST_LDFLAGS"
   export LDFLAGS
   AC_LANG_PUSH(C++)
   AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
@%:@include <boost/version.hpp>
]],
   [[
#if BOOST_VERSION >= $WANT_BOOST_VERSION
// Everything is okay
#else
#  error Boost version is too old
#endif
   ]])],
   [
      AC_MSG_RESULT(yes)
      succeeded=yes
      found_system=yes
      ifelse([$2], , :, [$2])
   ],
   [
   ])
   AC_LANG_POP([C++])
   if test "x$succeeded" != "xyes"; then
      _version=0
      if test "$ac_boost_path" != ""; then
         BOOST_LDFLAGS="-L$ac_boost_path/lib"
         if test -d "$ac_boost_path" && test -r "$ac_boost_path"; then
            for i in `ls -d $ac_boost_path/include/boost-* 2>/dev/null`; do
                _version_tmp=`echo $i | sed "s#$ac_boost_path##" | sed 's/\/include\/boost-//' | sed 's/_/./'`
                V_CHECK=`expr $_version_tmp \> $_version`
                if test "$V_CHECK" = "1" ; then
                   _version=$_version_tmp
                fi
                VERSION_UNDERSCORE=`echo $_version | sed 's/\./_/'`
                BOOST_CPPFLAGS="-I$ac_boost_path/include/boost-$VERSION_UNDERSCORE"
            done
         fi
      else
         for ac_boost_path in /usr /usr/local /opt ; do
             if test -d "$ac_boost_path" && test -r "$ac_boost_path"; then
                for i in `ls -d $ac_boost_path/include/boost-* 2>/dev/null`; do
                    _version_tmp=`echo $i | sed "s#$ac_boost_path##" | sed 's/\/include\/boost-//' | sed 's/_/./'`
                    V_CHECK=`expr $_version_tmp \> $_version`
                    if test "$V_CHECK" = "1" ; then
                       _version=$_version_tmp
                       best_path=$ac_boost_path
                    fi
                done
             fi
         done
         VERSION_UNDERSCORE=`echo $_version | sed 's/\./_/'`
         BOOST_CPPFLAGS="-I$best_path/include/boost-$VERSION_UNDERSCORE"
         BOOST_LDFLAGS="-L$best_path/lib"
         if test "x$BOOST_ROOT" != "x"; then
            if test -d "$BOOST_ROOT" && test -r "$BOOST_ROOT" && test -d "$BOOST_ROOT/stage/lib" && test -r "$BOOST_ROOT/stage/lib"; then
               version_dir=`expr //$BOOST_ROOT : '.*/\(.*\)'`
               stage_version=`echo $version_dir | sed 's/boost_//' | sed 's/_/./g'`
               stage_version_shorten=`expr $stage_version : '\([[0-9]]*\.[[0-9]]*\)'`
               V_CHECK=`expr $stage_version_shorten \>\= $_version`
               if test "$V_CHECK" = "1" ; then
                  AC_MSG_NOTICE(We will use a staged boost library from $BOOST_ROOT)
                  BOOST_CPPFLAGS="-I$BOOST_ROOT"
                  BOOST_LDFLAGS="-L$BOOST_ROOT/stage/lib"
               fi
            fi
         fi
      fi
      CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"
      export CPPFLAGS
      LDFLAGS="$LDFLAGS $BOOST_LDFLAGS"
      export LDFLAGS
      AC_LANG_PUSH(C++)
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
@%:@include <boost/version.hpp>
]],
      [[
#if BOOST_VERSION >= $WANT_BOOST_VERSION
// Everything is okay
#else
#  error Boost version is too old
#endif

      ]])],
      [
         AC_MSG_RESULT(yes ($_version))
         succeeded=yes
         ifelse([$2], , :, [$2])
      ],
      [
         AC_MSG_RESULT(no ($_version))
         ifelse([$3], , :, [$3])
      ])
      AC_LANG_POP([C++])
   fi

   if test "$succeeded" != "yes" ; then
      if test "$_version" = "0" ; then
         AC_MSG_ERROR([[We could not detect the boost libraries (version $boost_lib_version_req_shorten or higher). If you have a staged boost library (still not installed) please specify \$BOOST_ROOT in your environment and do not give a PATH to --with-boost option.  If you are sure you have boost installed, then check your version number looking in <boost/version.hpp>. See http://randspringer.de/boost for more documentation.]])
      else
         AC_MSG_ERROR('Your boost libraries seems to old (version $_version).  We need at least $boost_lib_version_shorten')
      fi
   else
      AC_SUBST(BOOST_CPPFLAGS)
      AC_SUBST(BOOST_LDFLAGS)
      AC_DEFINE(HAVE_BOOST,,[define if the Boost library is available])
   fi
   CPPFLAGS="$CPPFLAGS_SAVED"
   LDFLAGS="$LDFLAGS_SAVED"
fi
])





#######################################################
#
# check for GNU Multiprecision library
#
#######################################################
AC_DEFUN([AX_GMP],
[
AC_ARG_WITH([gmp],
            [AS_HELP_STRING([--with-gmp=DIR],
            [use GNU Multiprecision Library (default is yes) specifiy the root directory for GMP librariy (optional)])],
            [
                if test "$withval" = "no"; then
                    want_gmp="no"
                elif test "$withval" = "yes"; then
                    want_gmp="yes"
                    ac_gmp_path=""
                else
                    want_gmp="yes"
                    ac_gmp_path="$withval"
                fi
            ],
            [want_gmp="yes"]
)

if test "x$want_gmp" = "xyes"; then
   AC_REQUIRE([AC_PROG_CC])
   gmp_lib_version_req=ifelse([$1], ,4.1.0,$1)
   gmp_lib_version_req_shorten=`expr $gmp_lib_version_req : '\([[0-9]]*\.[[0-9]]*\)'`
   gmp_lib_version_req_major=`expr $gmp_lib_version_req : '\([[0-9]]*\)'`
   gmp_lib_version_req_minor=`expr $gmp_lib_version_req : '[[0-9]]*\.\([[0-9]]*\)'`
   gmp_lib_version_req_sub_minor=`expr $gmp_lib_version_req : '[[0-9]]*\.[[0-9]]*\.\([[0-9]]*\)'`
   if test "x$gmp_lib_version_req_sub_minor" = "x" ; then
        gmp_lib_version_req_sub_minor="0"
   fi
   AC_MSG_CHECKING(for gmplib >= $gmp_lib_version_req)
   succeeded=no
   if test "$ac_gmp_path" != ""; then
       GMP_LDFLAGS="-L$ac_gmp_path/lib"
       GMP_CPPFLAGS="-I$ac_gmp_path/include"
   else
       for ac_gmp_path_tmp in /usr /usr/local /opt ; do
           if test -d "$ac_gmp_path_tmp/include" && test -r "$ac_gmp_path_tmp/include"; then
               GMP_LDFLAGS="-L$ac_gmp_path_tmp/lib"
               GMP_CPPFLAGS="-I$ac_gmp_path_tmp/include"
               break;
           fi
       done
   fi
   CPPFLAGS_SAVED="$CPPFLAGS"
   CPPFLAGS="$CPPFLAGS $GMP_CPPFLAGS"
   export CPPFLAGS
   LDFLAGS_SAVED="$LDFLAGS"
   LDFLAGS="$LDFLAGS $GMP_LDFLAGS"
   export LDFLAGS
   AC_LANG_PUSH(C++)
   AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
@%:@include <gmp.h>
]],
   [[
#if (__GNU_MP_VERSION >= $gmp_lib_version_req_major) && (__GNU_MP_VERSION_MINOR >= $gmp_lib_version_req_minor)
// Everything is okay
#else
#  error GMP version is too old
#endif
   ]])],
   [
      AC_MSG_RESULT(yes)
      succeeded=yes
      found_system=yes
      ifelse([$2], , :, [$2])
   ],
   [
   ])
   AC_LANG_POP([C++])
   if test "$succeeded" != "yes" ; then
      AC_MSG_ERROR([[We could not detect the GMP libraries (version $gmp_lib_version_req_shorten or higher).]])
   else
      AC_SUBST(GMP_CPPFLAGS)
      AC_SUBST(GMP_LDFLAGS)
      AC_DEFINE(HAVE_GMP,,[define if the GNU Multiprecision library is available])
   fi
   CPPFLAGS="$CPPFLAGS_SAVED"
   LDFLAGS="$LDFLAGS_SAVED"
fi

]
)


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


AC_ARG_ENABLE([sse2],
              [AS_HELP_STRING([--enable-sse2],[enable SSE2 intrinsics [yes]])],
              [sse2=$enableval],
              [sse2=yes])

if test x"$sse2" = "xyes"; then
  if test x"$ac_cv_c_compiler_gnu" = "xyes"; then
    case $target in
    i*86-apple-* | i686-*-* | amd64-*-* | x86_64-*-*)
      DEFAULT_CXXFLAGS=" -msse2 $DEFAULT_CXXFLAGS"
        ;;
    *-*-*)
        ;;
    esac
  fi  
fi



have_openmp=no

AC_MSG_CHECKING(for OpenMP support)
AC_LINK_IFELSE([
AC_LANG_PROGRAM([[
#include <omp.h>
void f(int* a, int* b, int n) {
  int i;  
  #pragma parallel for
  for(i = 0; i < n; i++)
     a[i] += b[i];
}
]])],
[have_openmp=yes],
[have_openmp=no])

AC_MSG_RESULT($have_openmp)

AC_ARG_ENABLE([openmp],
              [AS_HELP_STRING([--enable-openmp],[enable OpenMP threading [automatic]])],
              [openmp=$enableval],
              [openmp=$have_openmp])

if test x"$openmp" = "xyes"; then
 if test x"$ac_cv_c_compiler_gnu" = "xyes"; then
    DEFAULT_CXXFLAGS=" -fopenmp $DEFAULT_CXXFLAGS"
    AC_CHECK_LIB([gomp], [omp_get_num_threads])
 fi
fi


AX_BOOST
AX_GMP


AC_OUTPUT([Makefile gen/Makefile test/Makefile ct-rng/Makefile ct-randcam/Makefile ct-rng-assessor/Makefile ct-rng-analyzer/Makefile])
