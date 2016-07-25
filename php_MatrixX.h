/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2015 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_MATRIXX_H
#define PHP_MATRIXX_H

extern zend_module_entry MatrixX_module_entry;
#define phpext_MatrixX_ptr &MatrixX_module_entry

#define PHP_MATRIXX_VERSION "0.0.1" 

#ifdef PHP_WIN32
#	define PHP_MATRIXX_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_MATRIXX_API __attribute__ ((visibility("default")))
#else
#	define PHP_MATRIXX_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

/*
    Declare any global variables you may need between the BEGIN
    and END macros here:

ZEND_BEGIN_MODULE_GLOBALS(MatrixX)
    long  global_value;
    char *global_string;
ZEND_END_MODULE_GLOBALS(MatrixX)
*/

/* In every utility function you add that needs to use variables
   in php_MatrixX_globals, call TSRMLS_FETCH(); after declaring other
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as MATRIXX_G(variable).  You are
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define MATRIXX_G(v) TSRMG(MatrixX_globals_id, zend_MatrixX_globals *, v)
#else
#define MATRIXX_G(v) (MatrixX_globals.v)
#endif

#endif	/* PHP_MATRIXX_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

#define CHECK_ARG_NUMS(v) if (ZEND_NUM_ARGS() != v) {\
WRONG_PARAM_COUNT;\
RETURN_FALSE;\
}\


typedef struct _matrix_rsrc {
    double** _matrix;
    ulong _m;
    ulong _n;
    ulong _rank;
} Matrix_rsrc;

Matrix_rsrc* matrix_add_matrix(Matrix_rsrc*, Matrix_rsrc*);
Matrix_rsrc* matrix_add_real(Matrix_rsrc*, double);

Matrix_rsrc* matrix_product_matrix(Matrix_rsrc*, Matrix_rsrc*);
Matrix_rsrc* matrix_product_real(Matrix_rsrc*, double);