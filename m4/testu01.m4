#######################################################
#
# check for TestU01 library
#
#######################################################
AC_DEFUN([AX_TESTU01],
[
AC_ARG_WITH([testu01],
            AS_HELP_STRING([--with-testu01=DIR],
                           [specify the root directory for the TestU01 library]),
            [
                if test "$withval" = "no"; then
                    want_testu01="no"
                elif test "$withval" = "yes"; then
                    want_testu01="yes"
                    ac_testu01_path=""
                else
                    want_testu01="yes"
                    ac_testu01_path="$withval"
                fi
            ],
            [want_testu01="yes"]
)

if test "x$want_testu01" = "xyes"; then
   AC_MSG_CHECKING(for TestU01)
   AC_REQUIRE([AC_PROG_CC])
   succeeded=no
   if test "$ac_testu01_path" != ""; then
       if test -f "$ac_testu01_path/include/unif01.h"; then
       	  TESTU01_LDFLAGS="-L$ac_testu01_path/lib"
       	  TESTU01_CPPFLAGS="-I$ac_testu01_path/include"
    	  succeeded=yes
       fi
   else
       for ac_testu01_path_tmp in /usr /usr/local /opt ; do
           if test -f "$ac_testu01_path_tmp/include/unif01.h"; then
               echo "testing $ac_testu01_path_tmp/include/unif01.h ..."
               TESTU01_LDFLAGS="-L$ac_testu01_path_tmp/lib"
               TESTU01_CPPFLAGS="-I$ac_testu01_path_tmp/include"
               succeeded=yes
               break;
           fi
       done
   fi
   if test "$succeeded" = "yes"; then
      AC_MSG_RESULT(yes)
      TESTU01_LDFLAGS="$TESTU01_LDFLAGS -lmylib -lprobdist -ltestu01"
      AC_SUBST(TESTU01_CPPFLAGS)
      AC_SUBST(TESTU01_LDFLAGS)
      AC_DEFINE(HAVE_TESTU01,,[define if the TestU01 library is available])
   else
      AC_MSG_RESULT(no)
   fi
fi

AM_CONDITIONAL(BUILD_CT_RNG_ANALYZER, test "$succeeded" = "yes")

])
