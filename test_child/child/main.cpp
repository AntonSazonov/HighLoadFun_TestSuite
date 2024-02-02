#include <cstdio>

#include <sched.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/sysinfo.h>

int main() {
	printf( " Child sched_getcpu(): %d\n", sched_getcpu() );
	return 0;
}
