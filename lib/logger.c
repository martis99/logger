#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <stdarg.h>

#include "logger.h"

#define LOG_FILE_NAME "logger.db"

struct Log
{
	sqlite3 *db;
};

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

static int db_exec(Log *log, const char *sql, void (*callback)(sqlite3_stmt *stmt), int num_args, ...)
{
	int rc;

	sqlite3_stmt *stmt;
	rc = sqlite3_prepare_v2(log->db, sql, -1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "Cant' prepare statement: %s\n", sqlite3_errmsg(log->db));
		sqlite3_finalize(stmt);
		return rc;
	}

	va_list args;
	va_start(args, num_args);
	for (int i = 0; i < num_args; i += 2)
	{
		int type = va_arg(args, int);
		switch (type)
		{
		case 0:
		{
			int v = va_arg(args, int);
			rc = sqlite3_bind_int(stmt, i / 2 + 1, v);
			if (rc != SQLITE_OK)
			{
				fprintf(stderr, "Can't bind var: %s\n", sqlite3_errmsg(log->db));
				sqlite3_finalize(stmt);
				return rc;
			}
		}
		break;
		case 1:
		{
			const char *v = va_arg(args, const char *);
			rc = sqlite3_bind_text(stmt, i / 2 + 1, v, strlen(v), SQLITE_STATIC);
			if (rc != SQLITE_OK)
			{
				fprintf(stderr, "Can't bind var: %s\n", sqlite3_errmsg(log->db));
				sqlite3_finalize(stmt);
				return rc;
			}
		}
		break;
		}
	}
	va_end(args);

	while (1)
	{
		rc = sqlite3_step(stmt);
		if (rc != SQLITE_ROW)
		{
			break;
		}

		if (callback != NULL)
		{
			callback(stmt);
		}
	}

	if (rc != SQLITE_DONE)
	{
		fprintf(stderr, "Can't step statement: %s\n", sqlite3_errmsg(log->db));
		sqlite3_finalize(stmt);
		return rc;
	}

	rc = sqlite3_reset(stmt);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "Cant't reset statement: %s\n", sqlite3_errmsg(log->db));
		sqlite3_finalize(stmt);
		return rc;
	}
	rc = sqlite3_finalize(stmt);
	return rc;
}

int log_init(Log **plog)
{
	*plog = malloc(sizeof(Log));
	Log *log = *plog;
	int rc;

	char filename[128];
	get_path(filename, sizeof(filename));
	rc = sqlite3_open(filename, &log->db);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(log->db));
		sqlite3_close(log->db);
		log->db == NULL;
		return 1;
	}

	char *sql = "CREATE TABLE IF NOT EXISTS log ("
				"id INTEGER PRIMARY KEY AUTOINCREMENT,"
				"time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
				"level INTEGER NOT NULL,"
				"proc TEXT NOT NULL,"
				"msg TEXT NOT NULL);";
	rc = db_exec(log, sql, NULL, 0);
	if (rc != SQLITE_OK)
	{
		sqlite3_close(log->db);
		log->db = NULL;
		return 1;
	}
	return 0;
}

int log_write(Log *log, const char *proc, Level level, const char *msg)
{
	return db_exec(log, "INSERT INTO log(level, proc, msg) values(?, ?, ?)", NULL, 6, 0, level, 1, proc, 1, msg);
}

static void print_row(sqlite3_stmt *stmt)
{
	const unsigned char *time, *proc, *msg;
	int level;

	time = sqlite3_column_text(stmt, 0);
	level = sqlite3_column_int(stmt, 1);
	proc = sqlite3_column_text(stmt, 2);
	msg = sqlite3_column_text(stmt, 3);

	printf("%s %c %s: %s\n", time, level_to_char(level), proc, msg);
}

int log_print_proc(Log *log, const char *prc)
{
	printf("%-19s %s %s: %s\n", "Time", "L", "Proc", "Message");
	return strlen(prc) == 0 ? db_exec(log, "SELECT time, level, proc, msg FROM log", print_row, 0) : db_exec(log, "SELECT time, level, proc, msg FROM log WHERE proc = ?", print_row, 1, 1, prc);
}

int log_print_all(Log *log)
{
	return log_print_proc(log, "");
}

int log_free(Log *log)
{
	if (log->db != NULL)
	{
		sqlite3_close(log->db);
		log->db = NULL;
	}
	free(log);
}