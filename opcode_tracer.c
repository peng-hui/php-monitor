#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_opcode_tracer.h"
#include "zend_extensions.h"
#include <limits.h>

static FILE *trace_fp = NULL;
static user_opcode_handler_t original_handler = NULL;

static int opcode_tracer_handler(zend_execute_data *execute_data)
{
    if (trace_fp) {
        const zend_op *opline = execute_data->opline;
        const char *filename = NULL;
        
        if (execute_data && execute_data->func && ZEND_USER_CODE(execute_data->func->type)) {
            filename = ZSTR_VAL(execute_data->func->op_array.filename);
        }
        
        if (filename) {
            char resolved_path[PATH_MAX];
            if (realpath(filename, resolved_path)) {
                fprintf(trace_fp, "%s:%d\n", resolved_path, opline->lineno);
            } else {
                fprintf(trace_fp, "%s:%d\n", filename, opline->lineno);
            }
        }
    }
    
    if (original_handler) {
        return original_handler(execute_data);
    }
    return ZEND_USER_OPCODE_DISPATCH;
}

PHP_MINIT_FUNCTION(opcode_tracer)
{
    char *trace_log = getenv("OPCODE_TRACE");
    if (trace_log) {
        trace_fp = fopen(trace_log, "w");
        if (trace_fp) {
            setbuf(trace_fp, NULL);
        }
    }
    
    // Hook all opcodes
    int i;
    for (i = 0; i < 256; i++) {
        zend_set_user_opcode_handler(i, opcode_tracer_handler);
    }
    
    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(opcode_tracer)
{
    if (trace_fp) {
        fclose(trace_fp);
        trace_fp = NULL;
    }
    return SUCCESS;
}

PHP_MINFO_FUNCTION(opcode_tracer)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "opcode_tracer support", "enabled");
    php_info_print_table_row(2, "Version", PHP_OPCODE_TRACER_VERSION);
    php_info_print_table_end();
}

const zend_function_entry opcode_tracer_functions[] = {
    PHP_FE_END
};

zend_module_entry opcode_tracer_module_entry = {
    STANDARD_MODULE_HEADER,
    "opcode_tracer",
    opcode_tracer_functions,
    PHP_MINIT(opcode_tracer),
    PHP_MSHUTDOWN(opcode_tracer),
    NULL,
    NULL,
    PHP_MINFO(opcode_tracer),
    PHP_OPCODE_TRACER_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_OPCODE_TRACER
ZEND_GET_MODULE(opcode_tracer)
#endif
