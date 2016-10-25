#include <stdio.h>
#include <stdlib.h>


typedef enum {
	err_lexical,
	err_syntax,
	err_class_undefined,
	err_function_undefined,
	err_variable_undefined,
	err_class_already_defined,
	err_function_already_defined,
	err_variable_already_defined,
	err_incompatible_types,
	err_wrong_argument_count,
	err_wrong_argument_type,
	err_semantic_other,
	err_input_value,
	err_variable_uninicialized,
	err_divide_by_zero,
	err_run_other,
	err_memory_allocation,
	err_opening_file,
	err_command_line_parameters
} error_type;

unsigned long line_number(FILE *f);
void error(error_type type);

