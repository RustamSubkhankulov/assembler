#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <limits.h>

#include "assembler.h"
#include "../stack/stackconfig.h"

//===================================================================

static char operation_name_buf[OPERATION_NAME_BUF_SIZE] = { 0 };

//===================================================================

int file_ptr_check(FILE* fp) {

	if (fp == NULL) {

		set_global_error_code(INV_FILE_PTR);
		global_error_process();

		return 0;
	}

	else
		return 1;
}

//===================================================================

int text_ptr_check(struct Text* text) {

	if (text == NULL) {

		set_global_error_code(INV_TEXT_PTR);
		global_error_process();

		return 0;
	}

	else
		return 1;
}

//===================================================================

int header_ptr_check(struct Header* header) {

	if (header == NULL) {

		set_global_error_code(INV_HDR_PTR);
		global_error_process();

		return 0;
	}

	else
		return 1;
}

//===================================================================

int string_ptr_check(const char* string) {

	if (string == NULL) {

		set_global_error_code(NULL_STRING_TEXT);
		global_error_process();

		return 0;
	}

	else
		return 1;
}

//===================================================================

int text_remove_comments_(struct Text* text, FUNC_FILE_LINE_PARAMS) {

	log_report();

	if (text_ptr_check(text) == 0)
		return -1;

	for (int str_count = 0; str_count < text->strings_number; str_count++) {

		char* ptr = strchr(text->strings[str_count].data, ';');

		if (ptr != NULL)
			*ptr = '\0';
	}
	
	return 0;
}

//===================================================================

int process_string_(const char* string, FILE* fp, struct Header* header, 
				                                  FUNC_FILE_LINE_PARAMS) {

	log_report();

	if (file_ptr_check(fp)       == 0)
		return -1;

	if (header_ptr_check(header) == 0)
		return -1;

	if (string_ptr_check(string) == 0)
		return -1;

	extern char operation_name_buf[];
	elem_t value = 0;

	int sscanf_ret = 
		sscanf(string, "%s " ELEM_SPEC, operation_name_buf, &value);
	
	if (sscanf_ret == 0) {

		set_global_error_code(INV_INSTRUCTION_STR);
		global_error_process();

		return -1;
	}
	
	int oper_code = get_operation_code(operation_name_buf);

	if (oper_code == -1) {

		set_global_error_code(INV_INSTRUCTION_STR);
		global_error_process();

		return -1;
	}

	if  (((  oper_code  == push)
		&&   sscanf_ret != 2   )
		||(( oper_code  != push)
		&&   sscanf_ret == 2  )) {

		set_global_error_code(INV_INSTRUCTION_STR);
		global_error_process();

		return -1;
	}

	if  (( oper_code  != push)
		&& sscanf_ret == 2 ) {

		set_global_error_code(INV_INSTRUCTION_STR);
		global_error_process();

		return -1;
	}

	int prev_pos = ftell(fp);

	if (sscanf_ret >= 1) {

		int fwrite_ret = 
			fwrite(&oper_code, sizeof(char), 1, fp);

		header->commands_number++;

		if (fwrite_ret != sizeof(char)) {

			set_global_error_code(FWRITE_ERR);
			global_error_process();

			return -1;
		}
	}

	if (sscanf_ret == 2) {
	
		int fwrite_ret =
			fwrite(&value, sizeof(char), sizeof(elem_t), fp);

		header->arg_commands_number++;

		if (fwrite_ret != sizeof(elem_t)) {

			set_global_error_code(FWRITE_ERR);
			global_error_process();

			return -1;
		}
	}

	int ret_val = write_listing(oper_code, operation_name_buf, 
										      value, prev_pos);

	if (ret_val == -1)
		return -1;

	clear_opernamebuf();

	return 0;
}

//==================================================================

int clear_opernamebuf_(FUNC_FILE_LINE_PARAMS) {

	log_report();

	extern char operation_name_buf[];

	for (int ct = 0; ct < OPERATION_NAME_BUF_SIZE; ct++)
		operation_name_buf[ct] = 0;

	return 0;
}

//===================================================================

int get_operation_code_(char* string, FUNC_FILE_LINE_PARAMS) {

	log_report();

	if (string_ptr_check(string) == 0)
		return -1;

	if (strncmp(string, "hlt",   OPERATION_NAME_BUF_SIZE) == 0) {

		return hlt;
	}

	if (strncmp(string, "start", OPERATION_NAME_BUF_SIZE) == 0) {

		return start;
	}

	if (strncmp(string, "add",   OPERATION_NAME_BUF_SIZE) == 0) {

		return add;
	}

	if (strncmp(string, "sub",   OPERATION_NAME_BUF_SIZE) == 0) {

		return sub;
	}

	if (strncmp(string, "div",   OPERATION_NAME_BUF_SIZE) == 0) {

		return div;
	}

	if (strncmp(string, "mul",   OPERATION_NAME_BUF_SIZE) == 0) {

		return mul;
	}

	if (strncmp(string, "push",  OPERATION_NAME_BUF_SIZE) == 0) {

		return push;
	}

	if (strncmp(string, "pop",   OPERATION_NAME_BUF_SIZE) == 0) {

		return pop;
	}

	if (strncmp(string, "in",    OPERATION_NAME_BUF_SIZE) == 0) {

		return in;
	}
	if (strncmp(string, "out",   OPERATION_NAME_BUF_SIZE) == 0) {

		return out;
	}

	return 0;
	
}

//===================================================================

int convert_operations_to_binary_(struct Text* text, FILE* fp, 
								  struct Header* header,
								  FUNC_FILE_LINE_PARAMS) {

	log_report();

	open_listing_file();

	if (text_ptr_check(text)     == 0)
		return -1;

	if (file_ptr_check(fp)       == 0)
		return -1;

	if (header_ptr_check(header) == 0)
		return -1;


	if (text->strings_number <= 0) {

		set_global_error_code(TEXT_NO_STRINGS);
		global_error_process();

		return -1;
	}
	
	fseek(fp, SEEK_SET, 0);

	char array[HDRSIZE] = { };
	fwrite(array, sizeof(char), HDRSIZE, fp);

	for (int str_count = 0; str_count < text->strings_number; str_count++) {

		int value = process_string(text->strings[str_count].data, fp , header);

		if (value == -1) return -1;
	}

	close_listing_file();

	return 0;
}

//===================================================================

FILE* open_code_file_(const char* filename, const char* mode, FUNC_FILE_LINE_PARAMS) {

	log_report();

	if (string_ptr_check(filename) == 0)
		return NULL;

	if (string_ptr_check(mode) == 0)
		return NULL;

	FILE* fp = fopen(filename, mode);

	if (file_ptr_check(fp) == 0)
		return NULL;

	return fp;
}

//===================================================================

int close_code_file_(FILE* fp, FUNC_FILE_LINE_PARAMS) {

	log_report();

	if (file_ptr_check(fp) == 0)
		return -1;

	int fclose_return_value = fclose(fp);

	if (fclose_return_value == EOF) {

		set_global_error_code(FCLOSE_ERROR);
		global_error_process();

		return -1;
	}

	return 0;
}

//===================================================================

int header_make_(struct Header* header, FILE* fp, FUNC_FILE_LINE_PARAMS) {

	log_report();

	if (header_ptr_check(header) == 0)
		return -1;

	if (file_ptr_check(fp)	     == 0)
		return -1;

	int ret_val = init_header(header, fp);

	if (ret_val == -1)
		return -1;

	ret_val = code_file_write_header(fp, header);

	if (ret_val == -1)
		return -1;

	return 0;
}

//===================================================================

int init_header_(struct Header* header, FILE* fp, FUNC_FILE_LINE_PARAMS) {

	log_report();

	if (header_ptr_check(header) == 0)
		return -1;
	if (file_ptr_check(fp) == 0)
		return -1;

	header->signature = SIGN;
	header->version = VERSION;

	fseek(fp, 0, SEEK_END);
	
	long file_size = ftell(fp);

	if (file_size == -1l) {

		set_global_error_code(CODE_FILE_SIZE_ERR);
		global_error_process();

		return -1;
	}

	header->file_size = file_size;

	if (file_size != sizeof(struct Header)
			       + header->arg_commands_number * sizeof(elem_t)
				   + header->commands_number     * sizeof(char)) {

		set_global_error_code(CODE_FILE_SIZE_ERR);
		global_error_process();

		return -1;
	}

	return 0;
}

//===================================================================

int code_file_write_header_(FILE* fp, struct Header* header,
									  FUNC_FILE_LINE_PARAMS) {
	
	log_report();

	if (header_ptr_check(header) == 0)
		return -1;

	if (file_ptr_check(fp) == 0)
		return -1;

	fseek(fp, 0, SEEK_SET);
	int fwrite_ret_val = 
		fwrite(header, sizeof(char), sizeof(struct Header), fp);

	if (fwrite_ret_val != sizeof(struct Header)) {

		set_global_error_code(FWRITE_ERR);
		global_error_process();

		return -1;
	}

	return 0;
}

//====================================================

int open_listing_file(void) {

	extern FILE* asm_listing_file;

	FILE* fp = fopen(ASM_LISTING_FILENAME, "wb");

	if (fp == NULL) {

		set_global_error_code(FOPEN_ERROR);
		global_error_process();

		return -1;
	}

	asm_listing_file = fp;

	return 0;
}

//====================================================

int close_listing_file(void) {

	extern FILE* asm_listing_file;

	int fclose_ret = fclose(asm_listing_file);

	if (fclose_ret == EOF) {

		set_global_error_code(FCLOSE_ERROR);
		global_error_process();

		return -1;
	}

	return 0;
}

//====================================================

int write_listing(int oper_code, const char* oper_name,
	elem_t value, int prev_pos) {

	extern FILE* asm_listing_file;

	if (file_ptr_check(asm_listing_file) == 0)
		return -1;

	if (string_ptr_check(oper_name) == 0)
		return -1;

	if (oper_code == 6) {

		fprintf(asm_listing_file, "\n %#x %s " ELEM_SPEC " %#x ", prev_pos,
			oper_name,
			value, oper_code);

		for (int ct = 0; ct <= sizeof(elem_t) - 1; ct++)
			fprintf(asm_listing_file, "%#x ", (char)(value << CHAR_BIT * ct));
	}

	else
		fprintf(asm_listing_file, "\n %#x %s %#x", prev_pos,
			oper_name,
			oper_code);

	fprintf(asm_listing_file, "\n\n");

	return 0;
}