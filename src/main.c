#include <stdio.h>
#include <unistd.h>

#include "logger.h"

int main(int argc, char **argv)
{
	char *proc = "";
	char ch;

	while ((ch = getopt(argc, argv, "P:")) != EOF)
	{
		switch (ch)
		{
		case 'P':
			proc = optarg;
			break;
		default:
			return 1;
		}
	}

	log_print_proc(proc);
	return 0;
}
