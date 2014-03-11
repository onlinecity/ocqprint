/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
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
#include "php_ocqprint.h"

/* If you declare any globals in php_ocqprint.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(ocqprint)
*/

/* True global resources - no need for thread safety here */
static int le_ocqprint;

/* {{{ ocqprint_functions[]
 *
 * Every user visible function must have an entry in ocqprint_functions[].
 */
const zend_function_entry ocqprint_functions[] = {
	//PHP_FE(confirm_ocqprint_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(qprint,	NULL)
	PHP_FE_END	/* Must be the last line in ocqprint_functions[] */
};
/* }}} */

/* {{{ ocqprint_module_entry
 */
zend_module_entry ocqprint_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"ocqprint",
	ocqprint_functions,
	PHP_MINIT(ocqprint),
	PHP_MSHUTDOWN(ocqprint),
	NULL,	/* Replace with NULL if there's nothing to do at request start */
	NULL,	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(ocqprint),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_OCQPRINT_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_OCQPRINT
ZEND_GET_MODULE(ocqprint)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("ocqprint.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_ocqprint_globals, ocqprint_globals)
    STD_PHP_INI_ENTRY("ocqprint.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_ocqprint_globals, ocqprint_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_ocqprint_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_ocqprint_init_globals(zend_ocqprint_globals *ocqprint_globals)
{
	ocqprint_globals->global_value = 0;
	ocqprint_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(ocqprint)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(ocqprint)
{
	return SUCCESS;
}
/* }}} */


/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(ocqprint)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "ocqprint support", "enabled");
	php_info_print_table_end();
}
/* }}} */


/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_ocqprint_compiled(string arg)
   Return a string to confirm that the module is compiled in */
/*
PHP_FUNCTION(confirm_ocqprint_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "ocqprint", arg);
	RETURN_STRINGL(strg, len, 0);
}
*/
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/

/* {{{ proto string qprint(string arg)
   Returns a quoted-printable encoded version of arg */
PHP_FUNCTION(qprint)
{
	char *arg = NULL;
	int argc = ZEND_NUM_ARGS();
	int arg_len;

	if (zend_parse_parameters(argc TSRMLS_CC, "s/", &arg, &arg_len) == FAILURE)
		return;

	size_t max_output_len = arg_len*2; // Calculate with a 100% qprint overhead
	char *outbuf = emalloc(max_output_len), *ch;
	unsigned char c;
	int i, current_line_len;
	int bufpos = 0, lastnl = 0;
	if (!outbuf) {
		php_error(E_ERROR, "Failed to allocate");
		RETURN_NULL();
	}

	// Reference RFC2045: http://www.ietf.org/rfc/rfc2045.txt

	for(i=0, ch=arg; i<arg_len; i++, ch++) {
		c = (unsigned char) *ch;
		current_line_len = (bufpos-lastnl);

		// If we grow outside the initial alloc, double our allocation
		if (bufpos >= (max_output_len-4)) {  // must fit at least 4 new bytes per loop
			php_error(E_WARNING, "Had to re-allocate qprint buffer wrote %d bytes", bufpos);
			max_output_len = max_output_len * 2;
			outbuf = erealloc(outbuf,max_output_len);
			if (!outbuf) {
				php_error(E_ERROR, "Failed to re-allocate");
				RETURN_NULL();
			}
		}

		if (c == '\n') {
			*(outbuf + bufpos++) = c;
			lastnl = bufpos;
			continue;
		}

		if (c == ' ') {  // convert space to =20 at eol/eof only
			if (
				i==(arg_len-1) ||
				*(arg + i + 1) == '\n' ||
				(*(arg + i + 1) == '\r' && *(arg + i + 2) == '\n')) {
				// Softbreak if required
				if (current_line_len+3 >= 76) {
					*(outbuf + bufpos++) = '=';
					*(outbuf + bufpos++) = '\r';
					*(outbuf + bufpos++) = '\n';
					lastnl = bufpos;
				}
				*(outbuf + bufpos++) = '=';
				*(outbuf + bufpos++) = '2';
				*(outbuf + bufpos++) = '0';
			} else {
				// Softbreak if required
				if (current_line_len+1 >= 76) {
					*(outbuf + bufpos++) = '=';
					*(outbuf + bufpos++) = '\r';
					*(outbuf + bufpos++) = '\n';
					lastnl = bufpos;
				}
				*(outbuf + bufpos++) = c;
			}
		} else if ((c == 61) || (c < 32) || (c > 126)) {
			// Softbreak if required
			if (current_line_len+3 >= 76) {
				*(outbuf + bufpos++) = '=';
				*(outbuf + bufpos++) = '\r';
				*(outbuf + bufpos++) = '\n';
				lastnl = bufpos;
			}
			// always encode "\t", which is *not* required, but safe
			*(outbuf + bufpos++) = '=';
			*(outbuf + bufpos++) = "0123456789ABCDEF"[c / 0x10];
			*(outbuf + bufpos++) = "0123456789ABCDEF"[c % 0x10];
		} else {
			// Softbreak if required
			if (current_line_len+1 >= 76) {
				*(outbuf + bufpos++) = '=';
				*(outbuf + bufpos++) = '\r';
				*(outbuf + bufpos++) = '\n';
				lastnl = bufpos;
			}
			// First dot on the line must be converted to =2E
			if (c == '.' && bufpos==lastnl) {
				*(outbuf + bufpos++) = '=';
				*(outbuf + bufpos++) = '2';
				*(outbuf + bufpos++) = 'E';
			} else {
				*(outbuf + bufpos++) = c;
			}
		}
	}

	RETURN_STRINGL(outbuf,bufpos,0);
}
/* }}} */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
