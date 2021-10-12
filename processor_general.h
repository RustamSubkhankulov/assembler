#pragma once

#include <stdio.h>

//разделить логи и дебаг на разные дефайны

//===================================================================

#define SIGN 'SRR!'

#define VERSION 1.0

//===================================================================

struct Header {

	int signature;
	float version;

	long file_size;
	int commands_number;
	int arg_commands_number;
};

#define HDRSIZE sizeof(struct Header)

//===================================================================

enum operations {

	hlt = 0,
	start = 1,
	add = 2,
	sub = 3,
	mul = 4,
	div = 5,
	push = 6,
	pop = 7,
	out = 8,
	in = 9
};

//===================================================================

//ASSEMBLER OPTIONS

#define OPERATION_NAME_BUF_SIZE 10

static FILE* asm_listing_file = NULL;

static const char* ASM_LISTING_FILENAME = "listing.txt";

//===================================================================

//PROCESSOR

static FILE* proc_output = stdout;