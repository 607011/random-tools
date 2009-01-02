AC_INIT(bbs.cpp)
AM_CONFIG_HEADER(config.h)

GENERIC_LIBRARY_NAME=librng

GENERIC_MAJOR_VERSION=0
GENERIC_MINOR_VERSION=1
GENERIC_MICRO_VERSION=0

GENERIC_API_VERSION=1.0
AC_SUBST(GENERIC_API_VERSION)

GENERIC_LIBRARY_VERSION=0:1:0

AC_SUBST(GENERIC_LIBRARY_VERSION)

PACKAGE=$GENERIC_LIBRARY_NAME
AC_SUBST(GENERIC_LIBRARY_NAME)

GENERIC_VERSION=$GENERIC_MAJOR_VERSION.$GENERIC_MINOR_VERSION.$GENERIC_MICRO_VERSION
GENERIC_RELEASE=$GENERIC_MAJOR_VERSION.$GENERIC_MINOR_VERSION
AC_SUBST(GENERIC_RELEASE)
AC_SUBST(GENERIC_VERSION)

VERSION=$GENERIC_VERSION

AM_INIT_AUTOMAKE($PACKAGE, $VERSION, [ola@ctmagazin.de])

AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_LANG_CPLUSPLUS
AC_C_CONST
AC_C_INLINE
AC_HEADER_STDBOOL
AC_PROG_CC
AC_STRUCT_TM
AC_TYPE_SIZE_T

AC_MSG_CHECKING([for a Padlock unit on processor])
if grep -q "VIA Nehemiah" /proc/cpuinfo
then
   	AC_MSG_RESULT([yes])
    AC_DEFINE([HAVE_PADLOCK], [1], [VIA Nehemiah processor present.])
else
    AC_MSG_RESULT([no])
    AC_MSG_WARN([VIA Nehemiah processor not found. Disabling Padlock support.])
fi


AC_DEFUN([AX_GCC_X86_CPUID],
[AC_REQUIRE([AC_PROG_CC])
AC_LANG_PUSH([C])
AC_CACHE_CHECK(for x86 cpuid $1 output, ax_cv_gcc_x86_cpuid_$1,
 [AC_RUN_IFELSE([AC_LANG_PROGRAM([#include <stdio.h>], [
     int op = $1, eax, ebx, ecx, edx;
     FILE *f;
      __asm__("cpuid"
        : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx)
        : "a" (op));
     f = fopen("conftest_cpuid", "w"); if (!f) return 1;
     fprintf(f, "%x:%x:%x:%x\n", eax, ebx, ecx, edx);
     fclose(f);
     return 0;
])],
     [ax_cv_gcc_x86_cpuid_$1=`cat conftest_cpuid`; rm -f conftest_cpuid],
     [ax_cv_gcc_x86_cpuid_$1=unknown; rm -f conftest_cpuid],
     [ax_cv_gcc_x86_cpuid_$1=unknown])])
AC_LANG_POP([C])
])

AC_DEFUN([AX_CPU_VENDOR],
[
 AC_REQUIRE([AX_GCC_X86_CPUID])
 AX_GCC_X86_CPUID(0x0)

 AC_CACHE_CHECK(for the processor vendor, ax_cpu_vendor,
 [
   ax_cpuid_maxid=`echo $ax_cv_gcc_x86_cpuid_0x0 | cut -d ":" -f 1` 
   vendor=`echo $ax_cv_gcc_x86_cpuid_0x0 | cut -d ":" -f 2`

   case $vendor in
     756e6547*)
       ax_cpu_vendor="Intel"
       ;;
     68747541*)
       ax_cpu_vendor="AMD"
       ;;
     69727943*)
       ax_cpu_vendor="Cyrix"
       ;;
     746e6543*)
       ax_cpu_vendor="Centaur"
       ;;
     646f6547*)
       ax_cpu_vendor="Natsemi Geode"
       ;;
     52697365*)
       ax_cpu_vendor="Rise"
       ;;
     65736952*)
       ax_cpu_vendor="Rise"
       ;;
     20536953*)
       ax_cpu_vendor="SiS"
       ;;
     *)
       ax_cpu_vendor="Unknown"
       ;;
   esac
 ])

])

AX_CPU_VENDOR

AM_SANITY_CHECK
AC_OUTPUT(Makefile)
