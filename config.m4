dnl $Id$
dnl config.m4 for extension ocqprint

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(ocqprint, for ocqprint support,
dnl Make sure that the comment is aligned:
dnl [  --with-ocqprint             Include ocqprint support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(ocqprint, whether to enable ocqprint support,
dnl Make sure that the comment is aligned:
dnl [  --enable-ocqprint           Enable ocqprint support])

if test "$PHP_OCQPRINT" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-ocqprint -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/ocqprint.h"  # you most likely want to change this
  dnl if test -r $PHP_OCQPRINT/$SEARCH_FOR; then # path given as parameter
  dnl   OCQPRINT_DIR=$PHP_OCQPRINT
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for ocqprint files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       OCQPRINT_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$OCQPRINT_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the ocqprint distribution])
  dnl fi

  dnl # --with-ocqprint -> add include path
  dnl PHP_ADD_INCLUDE($OCQPRINT_DIR/include)

  dnl # --with-ocqprint -> check for lib and symbol presence
  dnl LIBNAME=ocqprint # you may want to change this
  dnl LIBSYMBOL=ocqprint # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $OCQPRINT_DIR/lib, OCQPRINT_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_OCQPRINTLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong ocqprint lib version or lib not found])
  dnl ],[
  dnl   -L$OCQPRINT_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(OCQPRINT_SHARED_LIBADD)

  PHP_NEW_EXTENSION(ocqprint, ocqprint.c, $ext_shared)
fi
