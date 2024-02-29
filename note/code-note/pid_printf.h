void pid_printf(char *format,...)//自定义printf,在每行输出前插入进程ID
{
	va_list ap;
	va_start(ap,format);
	printf("[PID:%d]",getpid());//每行前插入
	vprintf(format,ap);
}
