typedef enum Level
{
	LOG_DEB,
	LOG_INFO,
	LOG_WARN,
	LOG_ERR
} Level;

int log_write(const char *proc, Level level, const char *msg);
int log_print_proc(const char *proc);
int log_print_all(void);