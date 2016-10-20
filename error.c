#include <stdio.h>
#include <stdlib.h>
#include "error.h"

/**
 * I've only added error types listed in the assignment so far,
 * many errors are still missing or are "in the phase of development :D".
 * I don't know which exact errors we'll need.
 * I'll also have another look on the line_number function, I'm not sure
 * how to call it.
 * Plus it might be a good idea to add an exact token on which the error occurs.
 * Like instead of: "Error in line: 37, variable already declared.",
 * something like this: "Error in line: 37, "int count", variable already
 * declared.". (I'm not sure if it's necessary, though.)
 * **/

unsigned long line_number(FILE *f) {
	unsigned long file_pos = ftell(f);
	fseek(f, 0, SEEK_SET);

	unsigned long line_count = 1;
	unsigned long curr_pos = 0;
	char is_EOL;

	while(curr_pos < file_pos) {
		is_EOL = fgetc(f);

		if(is_EOL != '\n') {
			curr_pos++;
		}
		
		else {
			line_count++;
		}
	}

	return line_count;
}

void error(error_type type) {
	unsigned return_value;
	unsigned long err_line = line_number(FILE *f);
	char *err_msg;

	switch(type) {

		case err_lexical:
			err_msg = "Invalid lexeme";
			return_value = 1;
			break;

		case err_syntax:
                        err_msg = "Invalid synatx";
                        return_value = 2;
                        break;

		case err_class_undefined:
                        err_msg = "Undefined class";
                        return_value = 3;
                        break;

		case err_function_undefined:
                        err_msg = "Undefined function";
                        return_value = 3;
                        break;

                case err_variable_undefined:
                        err_msg = "Undefined variable";
                        return_value = 3;
                        break;

                case err_class_already_defined:
                        err_msg = "Class already defined";
                        return_value = 3;
                        break;

                case err_function_already_defined:
                        err_msg = "Function already defined";
                        return_value = 3;
                        break;

                case err_variable_already_defined:
                        err_msg = "Variable already defined";
                        return_value = 3;
                        break;

                case err_incompatible_types:
                        err_msg = "Incompatible types";
                        return_value = 4;
                        break;

                case err_wrong_argument_count:
                        err_msg = "Wrong argument count";
                        return_value = 4;
                        break;

                case err_wrong_argument_type:
                        err_msg = "Wrong argument type";
                        return_value = 4;
                        break;

                case err_semantic_other:
                        err_msg = "Semantic error";
                        return_value = 6;
                        break;

                case err_input_value:
                        err_msg = "Input value error";
                        return_value = 7;
                        break;

                case err_variable_uninicialized:
                        err_msg = "Variable uninitialized";
                        return_value = 8;
                        break;

                case err_divide_by_zero:
                        err_msg = "Divide by zero";
                        return_value = 9;
                        break;

                case err_run_other:
                        err_msg = "Run error";
                        return_value = 10;
                        break;

                case err_memory_allocation:
                        err_msg = "Memory allocation error";
                        return_value = 99;
                        break;

                case err_opening_file:
                        err_msg = "Error opening file";
                        return_value = 99;
                        break;

                case err_command_line_parameters:
                        err_msg = "Wrong command line parameters";
                        return_value = 99;
                        break;
	}

	fprintf(stderr, "Error in line: '%lu'\n'%s'.\n", err_line, err_msg);
	exit(return_value);
}
