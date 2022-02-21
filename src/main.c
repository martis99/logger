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

	Log *log;
	log_init(&log);
	log_print_proc(log, proc);
	log_free(log);
	return 0;
}
