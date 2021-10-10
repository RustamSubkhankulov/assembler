#pragma once

#include <stdio.h>

//===================================================================

#define SIGN 'SRR!'

#define OPERATION_NAME_BUF_SIZE 10

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

