#######################################################
#
# check for GNU Multiprecision library
#
#######################################################
AC_DEFUN([AX_GMP],
[
AC_ARG_WITH([gmp],
            AS_HELP_STRING([--with-gmp=DIR],
                           [specify the root directory for the GMP library]),
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

succeeded=no
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


