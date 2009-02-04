#######################################################
#
# check for Padlock-capable processor
#
#######################################################
AC_DEFUN([AX_PADLOCK],
[
  ac_use_padlock=no
  AC_MSG_CHECKING([for a Padlock unit on processor])
  if grep -q "VIA Nehemiah" /proc/cpuinfo
  then
	 AC_MSG_RESULT([yes])
	 AC_DEFINE([HAVE_PADLOCK], [yes], [VIA Nehemiah processor present.])
	 ac_use_padlock=yes
  else
	 AC_MSG_RESULT([no])
  fi

  AM_CONDITIONAL(USE_PADLOCK, test "$ac_use_padlock" = "yes")
])
