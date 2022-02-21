#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "logger.h"

#define LOG_FILE_NAME "logger.log"
#define DELIMETER " "

static char *get_path(char *buffer, size_t maxlen)
{
	snprintf(buffer, maxlen, "%s/%s", getenv("HOME"), LOG_FILE_NAME);
	return buffer;
}

static char level_to_char(Level level)
{
	switch (level)
	{
	case LOG_DEB:
		return 'D';
	case LOG_INFO:
		return 'I';
	case LOG_WARN:
		return 'W';
	case LOG_ERR:
		return 'E';
	}
	return ' ';
}

int log_write(const char *proc, Level level, const char *msg)
{
	char filename[128];
	FILE *file = fopen(get_path(filename, sizeof(filename)), "a");
	if (file == NULL)
	{
		return 1;
	}
	time_t rawtime;
	time(&rawtime);
	char time[20];
	strftime(time, sizeof(time), "%Y-%m-%d %H:%M:%S", localtime(&rawtime));

	fprintf(file, "%s %c %s %s\n", time, level_to_char(level), proc, msg);
	fclose(file);
	return 0;
}

int log_print_proc(const char *prc)
{
	char filename[128];
	FILE *file = fopen(get_path(filename, sizeof(filename)), "r");
	if (file == NULL)
	{
		return 1;
	}

	char line[256];
	char *date, *time, *proc, *level, *msg;

	int p = strlen(prc);

	printf("%-19s %s %s: %s\n", "Time", "L", "Proc", "Message");
	while (fgets(line, sizeof(line), file))
	{
		date = strtok(line, DELIMETER);
		time = strtok(NULL, DELIMETER);
		level = strtok(NULL, DELIMETER);
		proc = strtok(NULL, DELIMETER);
		msg = strtok(NULL, "\n");

		if (p == 0 || strcmp(proc, prc) == 0)
		{
			printf("%s %s %s %s: %s\n", date, time, level, proc, msg);
		}
	}

	return 0;
}

int log_print_all(void)
{
	return log_print_proc("");
}