#include <stdio.h>
#include "uprintf.h"

void
MyPutChar(
	char c
	)
{
	fprintf(stderr, "%c", c);
}

int main(int ac, char **av)
{
	uPrintfSetPutChar(MyPutChar);

	uPrintf("Im am here\n");
	uPrintf("format % 3d\n", 4);
	uPrintf("format %3d\n", 4);
	uPrintf("format %03d\n", 4);
	uPrintf("format %033d\n", 4231);
	uPrintf("format %s\n", "Insert string");

	return 0;
}

