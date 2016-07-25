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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_MatrixX.h"

/* If you declare any globals in php_MatrixX.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(MatrixX)
*/

/* True global resources - no need for thread safety here */
static int le_MatrixX;

void matrix_dtor(zend_rsrc_list_entry* rsrc TSRMLS_DC)
{
        Matrix_rsrc* matrix;
        matrix = (Matrix_rsrc*)rsrc->ptr;           
        for (ulong i = 0; i < matrix->_n; i++) 
            efree(matrix->_matrix[i]);
        efree(matrix->_matrix);
        efree(matrix);
}

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("MatrixX.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_MatrixX_globals, MatrixX_globals)
    STD_PHP_INI_ENTRY("MatrixX.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_MatrixX_globals, MatrixX_globals)
PHP_INI_END()
*/
/* }}} */


/* {{{ php_MatrixX_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_MatrixX_init_globals(zend_MatrixX_globals *MatrixX_globals)
{
	MatrixX_globals->global_value = 0;
	MatrixX_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(MatrixX)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
    le_MatrixX = zend_register_list_destructors_ex(matrix_dtor, NULL, "MatrixX resource", module_number);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(MatrixX)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(MatrixX)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(MatrixX)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(MatrixX)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "MatrixX support", "enabled");
    php_info_print_table_row(2, "Version", PHP_MATRIXX_VERSION);
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/**
Define the arguments types of methods.
**/

ZEND_BEGIN_ARG_INFO(arg_construct, 1)
ZEND_ARG_INFO(0, var_initialize)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arg_three_param, 1)
ZEND_ARG_INFO(0, var_operation1)
ZEND_ARG_INFO(0, var_operation2)
ZEND_ARG_INFO(0, var_operation3)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arg_two_param, 1)
ZEND_ARG_INFO(0, var_operation1)         
ZEND_ARG_INFO(0, var_operation2)
ZEND_END_ARG_INFO()


ZEND_BEGIN_ARG_INFO(arg_one_param, 1)
ZEND_ARG_INFO(0, var_operation)
ZEND_END_ARG_INFO()

/* Remove the following function when you have successfully modified config.m4
so that your module can be compiled into PHP, it exists only for testing
purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_MatrixX_compiled(string arg)
Return a string to confirm that the module is compiled in */

PHP_FUNCTION(matrix_init) 
{
    zval *init;
    Matrix_rsrc *rs = emalloc(sizeof(Matrix_rsrc));
    rs->_n = 0;
    rs->_m = 0;

    switch (ZEND_NUM_ARGS())
    {
    case 0:                     //In the case if there was no param in construct
        WRONG_PARAM_COUNT;
        RETURN_FALSE;
        break;
    case 1:
        MAKE_STD_ZVAL(init);
        const char *active_class_const = "";        // Is for the 1st param of get_active_class_name

        if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z/", &init) == SUCCESS) {
            switch (Z_TYPE_P(init)) {
            case IS_ARRAY:

                //Input is an array
                zend_hash_internal_pointer_reset(Z_ARRVAL_P(init));
                uint num = zend_hash_num_elements(Z_ARRVAL_P(init));

                zval **temp;                        //Save temp zval of every elements in init
                                                    //Map the array
                for (uint i = 0; i < num; i++) {
                    zend_hash_get_current_data(Z_ARRVAL_P(init), (void**)&temp);
                    if (Z_TYPE_PP(temp) != IS_ARRAY) {
                        //If the current zval is not an array
                        if (Z_TYPE_PP(temp) == IS_LONG || Z_TYPE_PP(temp) == IS_DOUBLE) {  //If dimension = 1
                            zval *temp_n;       //Get _n property
                            MAKE_STD_ZVAL(temp_n);
                            ZVAL_LONG(temp_n, rs->_m);

                            if (Z_LVAL_P(temp_n) > 1) {             //Check if the dimension is NOT 1
                                zend_error(E_WARNING, "Input array is not a valid matrix in %s()", get_active_function_name(TSRMLS_C));
                                //Reset _n
                                rs->_m = 0;
                                zend_hash_internal_pointer_reset(Z_ARRVAL_P(init));
                                RETURN_FALSE;
                            }

                            if (Z_LVAL_P(temp_n) == 0)              //If _n has not been set
                                rs->_m = 1;
                            convert_to_double_ex(temp);
                            convert_to_array_ex(temp);
                        }
                        else {
                            zend_error(E_WARNING, "Error type of elements in array in %s()", get_active_function_name(TSRMLS_C));
                            //Reset _n
                            rs->_m = 0;
                            zend_hash_internal_pointer_reset(Z_ARRVAL_P(init));
                            RETURN_FALSE;
                        }
                    }
                    else {
                        //If the current zval is an array equals the dimension > 1
                        uint n = zend_hash_num_elements(Z_ARRVAL_PP(temp));
                        zend_hash_internal_pointer_reset(Z_ARRVAL_PP(temp));
                        zval **temp2;           //Save elements in temp
                                                //Map temp
                        for (uint j = 0; j < n; j++) {
                            zend_hash_get_current_data(Z_ARRVAL_PP(temp), (void**)&temp2);
                            if (Z_TYPE_PP(temp2) != IS_LONG && Z_TYPE_PP(temp2) != IS_DOUBLE) {     //If element is invalid
                                zend_error(E_WARNING, "Error type of elements in array in %s()", get_active_function_name(TSRMLS_C));
                                //Reset _n
                                rs->_m = 0;
                                //Reset pointer in each array 
                                zend_hash_internal_pointer_reset(Z_ARRVAL_PP(temp));
                                zend_hash_internal_pointer_reset(Z_ARRVAL_P(init));
                                RETURN_FALSE;
                            }
                            convert_to_double_ex(temp2);
                            zend_hash_move_forward(Z_ARRVAL_PP(temp));
                        }
                        zend_hash_internal_pointer_reset(Z_ARRVAL_PP(temp));

                        zval *temp_n;       //Get _n property
                        MAKE_STD_ZVAL(temp_n);
                        ZVAL_LONG(temp_n, rs->_m);

                        if (Z_LVAL_P(temp_n) == 0)              //Set _n to n if _n equals 0
                            rs->_m = n;
                        else
                            if (Z_LVAL_P(temp_n) != n) {             //Check if the _n is initialized & NOT n
                                zend_error(E_WARNING, "Input array is not a valid matrix in %s()", get_active_function_name(TSRMLS_C));
                                //Reset _n
                                rs->_m = 0;
                                zend_hash_internal_pointer_reset(Z_ARRVAL_P(init));
                                RETURN_FALSE;
                            }
                    }

                    zend_hash_move_forward(Z_ARRVAL_P(init));        //Move forwards the pointer in init
                }
                //End map and the input matrix is valid
                zend_hash_internal_pointer_reset(Z_ARRVAL_P(init));
                rs->_n = num;
                //Apply memory space
                rs->_matrix = (double **)ecalloc(num, sizeof(double*));
                if (!rs->_matrix) {
                    zend_error(E_WARNING, "Need enough memory", get_active_function_name(TSRMLS_C));
                    RETURN_FALSE;
                }

                for (ulong i = 0; i < num; i++)
                {
                    rs->_matrix[i] = (double *)ecalloc(rs->_m, sizeof(double));
                    if (!rs->_matrix[i]) {
                        zend_error(E_WARNING, "Need enough memory", get_active_function_name(TSRMLS_C));
                        RETURN_FALSE;
                    }
                    //Map init and save val in double[][]
                    zend_hash_get_current_data(Z_ARRVAL_P(init), (void**)&temp);
                    zend_hash_internal_pointer_reset(Z_ARRVAL_PP(temp)); 
                    zval **temp2;           //Save elements in temp
                    for (ulong j = 0; j < rs->_m; j++) {
                        zend_hash_get_current_data(Z_ARRVAL_PP(temp), (void**)&temp2);

                        rs->_matrix[i][j] = Z_DVAL_PP(temp2);

                        zend_hash_move_forward(Z_ARRVAL_PP(temp));
                    }
                    zend_hash_move_forward(Z_ARRVAL_P(init));
                }
                zend_hash_internal_pointer_reset(Z_ARRVAL_P(init));
                rs->_rank = 0;
                ZEND_REGISTER_RESOURCE(return_value, rs, le_MatrixX);
                break;
            case IS_RESOURCE:
                ;                               //Just make the next line no error in VS2015
                Matrix_rsrc *rc_temp;
                //Read resource
                ZEND_FETCH_RESOURCE(rc_temp, Matrix_rsrc *, &init, -1, "Matrix resource", le_MatrixX);
                if (rc_temp == NULL) {
                    RETURN_FALSE;
                }
                //Apply memory space
                rs->_matrix = (double **)ecalloc(rc_temp->_n, sizeof(double*));
                if (!rs->_matrix) {
                    zend_error(E_WARNING, "Need enough memory", get_active_function_name(TSRMLS_C));
                    RETURN_FALSE;
                }
                for (ulong i = 0; i < rc_temp->_n; i++)
                {
                    rs->_matrix[i] = (double *)ecalloc(rc_temp->_m, sizeof(double));
                    if (!rs->_matrix[i]) {
                        zend_error(E_WARNING, "Need enough memory", get_active_function_name(TSRMLS_C));
                        RETURN_FALSE;
                    }
                    for (ulong j = 0; j < rc_temp->_m; j++)
                        rs->_matrix[i][j] = rc_temp->_matrix[i][j];
                }

                //Set other vars and return
                rs->_n = rc_temp->_n;
                rs->_m = rc_temp->_m;
                rs->_rank = rc_temp->_rank;
                ZEND_REGISTER_RESOURCE(return_value, rs, le_MatrixX);
                break;
            default:
                zend_error(E_WARNING, "Error type input, %s() expect a Matrix resource or an array", get_active_function_name(TSRMLS_C));
                RETURN_FALSE;
                break;
            }
        }
        else {
            RETURN_FALSE;
        }
        break;
    default:
        WRONG_PARAM_COUNT;
        RETURN_FALSE;
        break;
    }
}

PHP_FUNCTION(matrix_add)
{
    CHECK_ARG_NUMS(2)
    zval *param1;
    zval *param2;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z/z/", &param1, &param2) == SUCCESS) {
        if (Z_TYPE_P(param1) == IS_RESOURCE && Z_TYPE_P(param2) == IS_RESOURCE) {           //Case param1 and 2 are both resource
            Matrix_rsrc* rs1;
            Matrix_rsrc* rs2;
            //Read resource
            ZEND_FETCH_RESOURCE(rs1, Matrix_rsrc *, &param1, -1, "Matrix resource", le_MatrixX);
            ZEND_FETCH_RESOURCE(rs2, Matrix_rsrc *, &param2, -1, "Matrix resource", le_MatrixX);
            if (rs1 == NULL || rs2 == NULL) {
                RETURN_FALSE;
            }
            if (rs1->_n != rs2->_n || rs1->_m != rs2->_m) {                   //The addition based on 2 matrices with same rows and columns
                zend_error(E_WARNING, "The matrices input should have the same rows and columns in %s()", get_active_function_name(TSRMLS_C));
                RETURN_FALSE;
            }
            else {
                ZEND_REGISTER_RESOURCE(return_value, matrix_add_matrix(rs1, rs2), le_MatrixX);
            }
        }else
            //Case param1 is resource and param2 is real
            if (Z_TYPE_P(param1) == IS_RESOURCE && (Z_TYPE_P(param2) == IS_LONG || Z_TYPE_P(param2) == IS_DOUBLE)) {
                Matrix_rsrc* rs1;
                double d2;
                ZEND_FETCH_RESOURCE(rs1, Matrix_rsrc *, &param1, -1, "Matrix resource", le_MatrixX);
                if (rs1 == NULL) {
                    RETURN_FALSE;
                }
                convert_to_double_ex(&param2);
                d2 = Z_DVAL_P(param2);
                ZEND_REGISTER_RESOURCE(return_value, matrix_add_real(rs1, d2), le_MatrixX);
            }else
                if (Z_TYPE_P(param2) == IS_RESOURCE && (Z_TYPE_P(param1) == IS_LONG || Z_TYPE_P(param1) == IS_DOUBLE)) {
                    //The case param1 is real and param2 is a resource
                    Matrix_rsrc* rs2;
                    double d1;
                    ZEND_FETCH_RESOURCE(rs2, Matrix_rsrc *, &param2, -1, "Matrix resource", le_MatrixX);
                    if (rs2 == NULL) {
                        RETURN_FALSE;
                    }
                    convert_to_double_ex(&param1);
                    d1 = Z_DVAL_P(param1);
                    ZEND_REGISTER_RESOURCE(return_value, matrix_add_real(rs2, d1), le_MatrixX);
                }
                else {
                    zend_error(E_WARNING, "Error input type in %s()", get_active_function_name(TSRMLS_C));
                    RETURN_FALSE;
                }
    }
    else {
        RETURN_FALSE;
    }
}

PHP_FUNCTION(matrix_subtract)
{
    CHECK_ARG_NUMS(2)
    zval *param1;
    zval *param2;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z/z/", &param1, &param2) == SUCCESS) {
        if (Z_TYPE_P(param1) == IS_RESOURCE && Z_TYPE_P(param2) == IS_RESOURCE) {           //Case param1 and 2 are both resource
            Matrix_rsrc* rs1;
            Matrix_rsrc* rs2;
            //Read resource
            ZEND_FETCH_RESOURCE(rs1, Matrix_rsrc *, &param1, -1, "Matrix resource", le_MatrixX);
            ZEND_FETCH_RESOURCE(rs2, Matrix_rsrc *, &param2, -1, "Matrix resource", le_MatrixX);
            if (rs1 == NULL || rs2 == NULL) {
                RETURN_FALSE;
            }
            if (rs1->_n != rs2->_n || rs1->_m != rs2->_m) {                   //The addition based on 2 matrices with same rows and columns
                zend_error(E_WARNING, "The matrices input should have the same rows and columns in %s()", get_active_function_name(TSRMLS_C));
                RETURN_FALSE;
            }
            else {
                ZEND_REGISTER_RESOURCE(return_value, matrix_add_matrix(rs1, matrix_product_real(rs2, -1)), le_MatrixX);
            }
        }
        else
            //Case param1 is resource and param2 is real
            if (Z_TYPE_P(param1) == IS_RESOURCE && (Z_TYPE_P(param2) == IS_LONG || Z_TYPE_P(param2) == IS_DOUBLE)) {
                Matrix_rsrc* rs1;
                double d2;
                ZEND_FETCH_RESOURCE(rs1, Matrix_rsrc *, &param1, -1, "Matrix resource", le_MatrixX);
                if (rs1 == NULL) {
                    RETURN_FALSE;
                }
                convert_to_double_ex(&param2);
                d2 = Z_DVAL_P(param2);
                ZEND_REGISTER_RESOURCE(return_value, matrix_add_real(rs1, -1 * d2), le_MatrixX);
            }
            else
                if (Z_TYPE_P(param2) == IS_RESOURCE && (Z_TYPE_P(param1) == IS_LONG || Z_TYPE_P(param1) == IS_DOUBLE)) {
                    //The case param1 is real and param2 is a resource
                    Matrix_rsrc* rs2;
                    double d1;
                    ZEND_FETCH_RESOURCE(rs2, Matrix_rsrc *, &param2, -1, "Matrix resource", le_MatrixX);
                    if (rs2 == NULL) {
                        RETURN_FALSE;
                    }
                    convert_to_double_ex(&param1);
                    d1 = Z_DVAL_P(param1);
                    ZEND_REGISTER_RESOURCE(return_value, matrix_add_real(matrix_product_real(rs2, -1), d1), le_MatrixX);
                }
                else {
                    zend_error(E_WARNING, "Error input type in %s()", get_active_function_name(TSRMLS_C));
                    RETURN_FALSE;
                }
    }
    else {
        RETURN_FALSE;
    }
}

PHP_FUNCTION(matrix_product)
{
    CHECK_ARG_NUMS(2)
    zval *param1;
    zval *param2;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z/z/", &param1, &param2) == SUCCESS) {
        if (Z_TYPE_P(param1) == IS_RESOURCE && Z_TYPE_P(param2) == IS_RESOURCE) {           //Case param1 and 2 are both resource
            Matrix_rsrc* rs1;
            Matrix_rsrc* rs2;
            //Read resource
            ZEND_FETCH_RESOURCE(rs1, Matrix_rsrc *, &param1, -1, "Matrix resource", le_MatrixX);
            ZEND_FETCH_RESOURCE(rs2, Matrix_rsrc *, &param2, -1, "Matrix resource", le_MatrixX);
            if (rs1 == NULL || rs2 == NULL) {
                RETURN_FALSE;
            }
            if (rs1->_n != rs2->_m) {                   //Check if they could be taken production
                zend_error(E_WARNING, "The matrices input can not do production in %s()", get_active_function_name(TSRMLS_C));
                RETURN_FALSE;
            }
            else {
                ZEND_REGISTER_RESOURCE(return_value, matrix_product_matrix(rs1, rs2), le_MatrixX);
            }
        }
        else
            //Case param1 is resource and param2 is real
            if (Z_TYPE_P(param1) == IS_RESOURCE && (Z_TYPE_P(param2) == IS_LONG || Z_TYPE_P(param2) == IS_DOUBLE)) {
                Matrix_rsrc* rs1;
                double d2;
                ZEND_FETCH_RESOURCE(rs1, Matrix_rsrc *, &param1, -1, "Matrix resource", le_MatrixX);
                if (rs1 == NULL) {
                    RETURN_FALSE;
                }
                convert_to_double_ex(&param2);
                d2 = Z_DVAL_P(param2);
                ZEND_REGISTER_RESOURCE(return_value, matrix_product_real(rs1, d2), le_MatrixX);
            }
            else
                if (Z_TYPE_P(param2) == IS_RESOURCE && (Z_TYPE_P(param1) == IS_LONG || Z_TYPE_P(param1) == IS_DOUBLE)) {
                    //The case param1 is real and param2 is a resource
                    Matrix_rsrc* rs2;
                    double d1;
                    ZEND_FETCH_RESOURCE(rs2, Matrix_rsrc *, &param2, -1, "Matrix resource", le_MatrixX);
                    if (rs2 == NULL) {
                        RETURN_FALSE;
                    }
                    convert_to_double_ex(&param1);
                    d1 = Z_DVAL_P(param1);
                    ZEND_REGISTER_RESOURCE(return_value, matrix_product_real(rs2, d1), le_MatrixX);
                }
                else {
                    zend_error(E_WARNING, "Error input type in %s()", get_active_function_name(TSRMLS_C));
                    RETURN_FALSE;
                }
    }
    else {
        RETURN_FALSE;
    }

}

PHP_FUNCTION(matrix_divide)
{
    

}

PHP_FUNCTION(matrix_transpose)
{
    CHECK_ARG_NUMS(1)
    zval *param;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &param) == SUCCESS) {
        Matrix_rsrc* rs;
        ZEND_FETCH_RESOURCE(rs, Matrix_rsrc *, &param, -1, "Matrix resource", le_MatrixX);
        if (rs == NULL) {
            RETURN_FALSE;
        }
        Matrix_rsrc* result = emalloc(sizeof(Matrix_rsrc));
        result->_m = rs->_n;
        result->_n = rs->_m;
        result->_rank = rs->_rank;
        result->_matrix = (double**)ecalloc(rs->_m, sizeof(double*));
        for (ulong i = 0; i < rs->_m; i++) {
            result->_matrix[i] = (double*)ecalloc(rs->_n, sizeof(double));
            for (ulong j = 0; j < rs->_n; j++) {
                result->_matrix[i][j] = rs->_matrix[j][i];
            }
        }
        ZEND_REGISTER_RESOURCE(return_value, result, le_MatrixX);
    }
    else {
        RETURN_FALSE;
    }
}

PHP_FUNCTION(matrix_inverse)
{

}

PHP_FUNCTION(matrix_rank)
{

}

PHP_FUNCTION(matrix_dump)
{
    if (ZEND_NUM_ARGS() != 1) {
        WRONG_PARAM_COUNT;
        RETURN_FALSE;
    }
    zval *param;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r/", &param) == SUCCESS) {
        Matrix_rsrc* rc_temp;
        //Read resource
        ZEND_FETCH_RESOURCE(rc_temp, Matrix_rsrc *, &param, -1, "Matrix resource", le_MatrixX);
        if (rc_temp == NULL) {
            RETURN_FALSE;
        }
        php_printf("<table>");
        for (ulong i = 0; i < rc_temp->_n; i++) {
            for (ulong j = 0; j < rc_temp->_m; j++) {
                php_printf("%f ", rc_temp->_matrix[i][j]);
            }
            php_printf("<br>");
        }
        php_printf("</table>");
        php_printf("m:%d<br>", rc_temp->_n);
        php_printf("n:%d<br>", rc_temp->_m);
    }
    else {
        RETURN_FALSE;
    }
    RETURN_TRUE;
}

PHP_FUNCTION(matrix_destroy)
{
    CHECK_ARG_NUMS(1)
    zval* param;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &param) == SUCCESS) {
        if (zend_list_delete(Z_RESVAL_P(param)) == FAILURE) {
            RETURN_FALSE;
        }
        else
            RETURN_TRUE;    
    }
    RETURN_FALSE;
}

PHP_FUNCTION(matrix_rows)
{
    if (ZEND_NUM_ARGS() != 1) {
        WRONG_PARAM_COUNT;
        RETURN_FALSE;
    }
    zval *param;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r/", &param) == SUCCESS) {
        Matrix_rsrc* rc_temp;
        //Read resource
        ZEND_FETCH_RESOURCE(rc_temp, Matrix_rsrc *, &param, -1, "Matrix resource", le_MatrixX);
        if (rc_temp == NULL) {
            RETURN_FALSE;
        }
        RETURN_LONG(rc_temp->_m);
    }
    else {
        RETURN_FALSE;
    }
}

PHP_FUNCTION(matrix_cols)
{
    if (ZEND_NUM_ARGS() != 1) {
        WRONG_PARAM_COUNT;
        RETURN_FALSE;
    }
    zval *param;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r/", &param) == SUCCESS) {
        Matrix_rsrc* rc_temp;
        //Read resource
        ZEND_FETCH_RESOURCE(rc_temp, Matrix_rsrc *, &param, -1, "Matrix resource", le_MatrixX);
        if (rc_temp == NULL) {
            RETURN_FALSE;
        }
        RETURN_LONG(rc_temp->_n);
    }
    else {
        RETURN_FALSE;
    }

}

PHP_FUNCTION(matrix_current)
{
    if (ZEND_NUM_ARGS() != 1) {
        WRONG_PARAM_COUNT;
        RETURN_FALSE;
    }
    zval *param;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r/", &param) == SUCCESS) {
        Matrix_rsrc* rc_temp;
        zval* result;
        MAKE_STD_ZVAL(result);
        array_init(result);

        //Read resource
        ZEND_FETCH_RESOURCE(rc_temp, Matrix_rsrc *, &param, -1, "Matrix resource", le_MatrixX);
        if (rc_temp == NULL) {
            RETURN_FALSE;
        }
        zval** element;
        element = ecalloc(rc_temp->_n, sizeof(zval*));
        for (ulong i = 0; i < rc_temp->_n; i++) {
            MAKE_STD_ZVAL(element[i]);
            array_init(element[i]);
            for (ulong j = 0; j < rc_temp->_m; j++) {
                add_next_index_double(element[i], rc_temp->_matrix[i][j]);
            }
            add_next_index_zval(result, element[i]);
        }
        RETURN_ZVAL(result, 1, 0);
    }
    else {
        RETURN_FALSE;
    }

}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
unfold functions in source code. See the corresponding marks just before
function definition, where the functions purpose is also documented. Please
follow this convention for the convenience of others editing your code.
*/




/* {{{ MatrixX_functions[]
 *
 * Every user visible function must have an entry in MatrixX_functions[].
 */
const zend_function_entry MatrixX_functions[] = {
    PHP_FE(matrix_init, arg_construct)

    PHP_FE(matrix_dump, arg_one_param)
    PHP_FE(matrix_add, arg_two_param)
    PHP_FE(matrix_subtract, arg_two_param)
    PHP_FE(matrix_product, arg_two_param)
    PHP_FE(matrix_divide, arg_two_param)
    PHP_FE(matrix_transpose, arg_one_param)
    PHP_FE(matrix_inverse, arg_one_param)
    PHP_FE(matrix_rank, arg_one_param)

    PHP_FE(matrix_destroy, arg_one_param)
    PHP_FE(matrix_rows, arg_one_param)
    PHP_FE(matrix_cols, arg_one_param)
    PHP_FE(matrix_current, arg_three_param)

	PHP_FE_END	/* Must be the last line in MatrixX_functions[] */
};
/* }}} */

/* {{{ MatrixX_module_entry
 */
zend_module_entry MatrixX_module_entry = {
	STANDARD_MODULE_HEADER,
	"MatrixX",
	MatrixX_functions,
	PHP_MINIT(MatrixX),
	PHP_MSHUTDOWN(MatrixX),
	PHP_RINIT(MatrixX),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(MatrixX),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(MatrixX),
	PHP_MATRIXX_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_MATRIXX
ZEND_GET_MODULE(MatrixX)
#endif

/**************************************************************
Here implements all functions of matrix
***************************************************************/
Matrix_rsrc* matrix_add_matrix(Matrix_rsrc* a, Matrix_rsrc* b) {
    Matrix_rsrc* result = emalloc(sizeof(Matrix_rsrc));
    result->_n = a->_n;
    result->_m = a->_m;
    result->_rank = 0;
    result->_matrix = (double**)ecalloc(result->_n, sizeof(double*));
    if (a->_m != b->_m || a->_n != b->_n)
        return result;
    for (ulong i = 0; i < result->_n; i++) {
        result->_matrix[i] = (double*)ecalloc(result->_m, sizeof(double));
        for (ulong j = 0; j < result->_m; j++)
            result->_matrix[i][j] = a->_matrix[i][j] + b->_matrix[i][j];
    }
    return result;
}
//A matrix add a double var, equals every elements in a add this double
Matrix_rsrc* matrix_add_real(Matrix_rsrc* a, double b) {
    Matrix_rsrc* result = emalloc(sizeof(Matrix_rsrc));
    result->_n = a->_n;
    result->_m = a->_m;
    result->_rank = 0;
    result->_matrix = (double**)ecalloc(result->_n, sizeof(double*));
    for (ulong i = 0; i < result->_n; i++) {
        result->_matrix[i] = (double*)ecalloc(result->_m, sizeof(double));
        for (ulong j = 0; j < result->_m; j++)
            result->_matrix[i][j] = a->_matrix[i][j] + b;
    }
    return result;
}

Matrix_rsrc* matrix_product_matrix(Matrix_rsrc* a, Matrix_rsrc* b) {
    Matrix_rsrc* result = emalloc(sizeof(Matrix_rsrc));
    result->_n = b->_n;
    result->_m = a->_m;
    result->_rank = 0;
    result->_matrix = (double**)ecalloc(result->_n, sizeof(double*));
    if (a->_n != b->_m)
        return result;
    double sum = 0.0;
    for (ulong i = 0; i < result->_n; i++) {
        result->_matrix[i] = (double*)ecalloc(result->_m, sizeof(double));
        for (ulong j = 0; j < result->_m; j++) {
            for (ulong k = 0; k < a->_n; k++) {
                sum += (a->_matrix[k][j]) * (b->_matrix[i][k]);
            }
            result->_matrix[i][j] = sum;
            sum = 0.0;
        }
    }
    return result;
}

Matrix_rsrc* matrix_product_real(Matrix_rsrc* a, double b) {
    Matrix_rsrc* result = emalloc(sizeof(Matrix_rsrc));
    result->_n = a->_n;
    result->_m = a->_m;
    result->_rank = 0;
    result->_matrix = (double**)ecalloc(result->_n, sizeof(double*));
    for (ulong i = 0; i < result->_n; i++) {
        result->_matrix[i] = (double*)ecalloc(result->_m, sizeof(double));
        for (ulong j = 0; j < result->_m; j++)
            result->_matrix[i][j] = a->_matrix[i][j] * b;
    }
    return result;
}
/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
