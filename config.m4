dnl $Id$
dnl config.m4 for extension MatrixX

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(MatrixX, for MatrixX support,
dnl Make sure that the comment is aligned:
dnl [  --with-MatrixX             Include MatrixX support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(MatrixX, whether to enable MatrixX support,
dnl Make sure that the comment is aligned:
[  --enable-MatrixX           Enable MatrixX support])

if test "$PHP_MATRIXX" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-MatrixX -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/MatrixX.h"  # you most likely want to change this
  dnl if test -r $PHP_MATRIXX/$SEARCH_FOR; then # path given as parameter
  dnl   MATRIXX_DIR=$PHP_MATRIXX
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for MatrixX files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       MATRIXX_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$MATRIXX_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the MatrixX distribution])
  dnl fi

  dnl # --with-MatrixX -> add include path
  dnl PHP_ADD_INCLUDE($MATRIXX_DIR/include)

  dnl # --with-MatrixX -> check for lib and symbol presence
  dnl LIBNAME=MatrixX # you may want to change this
  dnl LIBSYMBOL=MatrixX # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $MATRIXX_DIR/$PHP_LIBDIR, MATRIXX_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_MATRIXXLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong MatrixX lib version or lib not found])
  dnl ],[
  dnl   -L$MATRIXX_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(MATRIXX_SHARED_LIBADD)

  PHP_NEW_EXTENSION(MatrixX, MatrixX.c, $ext_shared)
fi
