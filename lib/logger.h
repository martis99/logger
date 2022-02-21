typedef struct Log Log;

typedef enum Level
{
	LOG_DEB,
	LOG_INFO,
	LOG_WARN,
	LOG_ERR
} Level;

int log_init(Log **log);
int log_write(Log *log, const char *proc, Level level, const char *msg);
int log_print_proc(Log *log, const char *proc);
int log_print_all(Log *log);
int log_free(Log *log);