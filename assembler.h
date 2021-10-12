#pragma once

#include "processor_general.h"
#include "text_processing.h"
#include "../stack/errors_and_logs.h"
#include "../stack/errors.h"

//===================================================================

int open_listing_file(void);

int close_listing_file(void);

int write_listing(int oper_code, const char* oper_name,
	elem_t value, int prev_pos);

//===================================================================

#define clear_opernamebuf() \
		clear_opernamebuf_(FUNC_FILE_LINE_GET_ARGS)

#define text_remove_comments(text) \
		text_remove_comments_(text, FUNC_FILE_LINE_GET_ARGS)

//===================================================================

#define convert_operations_to_binary(text, fp, header)  \
		convert_operations_to_binary_(text, fp, header, \
							   FUNC_FILE_LINE_GET_ARGS) \

#define process_string(string, fp, header) \
		process_string_(string, fp, header, FUNC_FILE_LINE_GET_ARGS)

#define get_operation_code(opername) \
		get_operation_code_(opername, FUNC_FILE_LINE_GET_ARGS)

//===================================================================

#define open_code_file(filename, mode) \
		open_code_file_(filename , mode, FUNC_FILE_LINE_GET_ARGS)

#define close_code_file(fp) \
		close_code_file_(fp, FUNC_FILE_LINE_GET_ARGS)

//===================================================================

#define header_make(header, fp) \
		header_make_(header, fp ,FUNC_FILE_LINE_GET_ARGS)

#define init_header(header, fp) \
		init_header_(header, fp, FUNC_FILE_LINE_GET_ARGS)

#define code_file_write_header(fp, header)  \
		code_file_write_header_(fp, header, \
				   FUNC_FILE_LINE_GET_ARGS)

//===================================================================

int clear_opernamebuf_(FUNC_FILE_LINE_PARAMS);

int text_ptr_check(struct Text* text);

int header_ptr_check(struct Header* header);

int file_ptr_check(FILE* fp);

int string_ptr_check(const char* string);

//===================================================================

int text_remove_comments_(struct Text* text, FUNC_FILE_LINE_PARAMS);

int convert_operations_to_binary_(struct Text* text, FILE* fp,
								   struct Header* header, 
								   FUNC_FILE_LINE_PARAMS);

//===================================================================

FILE* open_code_file_(const char* filename, const char* mode,
									  FUNC_FILE_LINE_PARAMS);

int close_code_file_(FILE* fp, FUNC_FILE_LINE_PARAMS);

//===================================================================

int header_make_(struct Header* header, FILE* fp, FUNC_FILE_LINE_PARAMS);

int init_header_(struct Header* header, FILE* fp, FUNC_FILE_LINE_PARAMS);

int code_file_write_header_(FILE* fp, struct Header* header, 
	                                 FUNC_FILE_LINE_PARAMS);

//===================================================================

int process_string_(const char* string, FILE* fp, 
				   struct Header* header, FUNC_FILE_LINE_PARAMS);

int get_operation_code_(char* opername, FUNC_FILE_LINE_PARAMS);