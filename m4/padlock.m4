#######################################################
#
# check for Padlock-capable processor
#
#######################################################
AC_DEFUN([AX_PADLOCK],
[
  AC_MSG_CHECKING([for a Padlock unit on processor])
  if grep -q "VIA Nehemiah" /proc/cpuinfo
  then
	 AC_MSG_RESULT([yes])
	 AC_DEFINE([HAVE_PADLOCK], [yes], [VIA Nehemiah processor present.])
  else
	 AC_MSG_RESULT([no])
  fi

  AM_CONDITIONAL(HAVE_PADLOCK, test "$HAVE_PADLOCK" = yes)
])
