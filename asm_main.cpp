#include "assembler.h"

#define $ if (err_val == -1) return -1;

int main(int argc, char* argv[]) {
	
	int err_val = 0;
	struct Text text = { 0 };

	char* buf = text_init(argv[1], &text);
	text_remove_comments(&text); $

	FILE* fp = open_code_file(argv[2], "wb");
	if (fp == NULL) 
		return -1;

	struct Header header = { 0 };

	err_val = convert_operations_to_binary(&text, fp, &header); $
	err_val = header_make(&header, fp); $


	close_code_file(fp); $

	text_clear_mem(&text, buf);

	return 0;
}