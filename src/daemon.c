#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void daemonize(void) {
	pid_t pid = fork();
	if (pid < 0) {
		fprintf(stderr, "plugd: fork failed: %s", strerror(errno));
		exit(1);
	}
	if (pid > 0) {
		// parent exits, child continues in background
		exit(0);
	}
}
