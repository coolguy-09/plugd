#include <unistd.h>
#include <glob.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sysmacros.h>  // for major/minor macros
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <daemon.h>
#include <other.h>
#include <debug.h>

int main(int argc, char *argv[]) {
	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], "--daemon") == 0) {
			daemonize();
		}
		if (strcmp(argv[i], "--help") == 0) {
			printf("Usage for plugd: \n");
			printf("  plugd [OPTIONS]\n");
			printf("  Options:\n");
			printf("    --help: Show this help dialouge.\n");
			printf("    --version: Show the version of plugd.\n");
			printf("    --daemon: Run in the background.\n");
			printf("    --debug: Show debug logs.\n");
			return 0;
		}
		if (strcmp(argv[i], "--version") == 0) {
			printf("plugd v1.1\n");
			return 0;
		}
		if (strcmp(argv[i], "--debug") == 0) {
			debug = 1;
		}
	}

	glob_t g;

	if (mkdir("/dev/block", 0755) == -1) {
        if (errno != EEXIST && errno != EACCES) { 
            fprintf(stderr, "plugd: mkdir failed: %s\n", strerror(errno));
			return 1; // Critical error
		}
    }

	while(1) {
		if (access("/dev/stdin", F_OK) != 0) { if (access("/proc/self/fd/0", F_OK) == 0) { symlink("/proc/self/fd/0", "/dev/stdin"); } }
		if (access("/dev/stdout", F_OK) != 0) { if (access("/proc/self/fd/1", F_OK) == 0) { symlink("/proc/self/fd/1", "/dev/stdout"); } }
		if (access("/dev/stderr", F_OK) != 0) { if (access("/proc/self/fd/2", F_OK) == 0) { symlink("/proc/self/fd/2", "/dev/stderr"); } }
		if (access("/dev/core", F_OK) != 0) { if (access("/proc/kcore", F_OK) == 0) { symlink("/proc/kcore", "/dev/core"); } }
		if (access("/dev/fd", F_OK) != 0) { if (access("/proc/self/fd", F_OK) == 0) { symlink("/proc/self/fd", "/dev/fd"); } }
		if (access("/dev/rtc", F_OK) != 0) { if (access("/dev/rtc0", F_OK) == 0) { symlink("/dev/rtc0", "/dev/rtc"); } }
		if (access("/dev/cdrom", F_OK) != 0) { if (access("/dev/sr0", F_OK) == 0) { symlink("/dev/sr0", "/dev/cdrom"); } }
		const char *patterns[] = {"/dev/sd*", "/dev/hd*", "/dev/zram*", "/dev/fd*", "/dev/nvme*n*"};
		int num_patterns = sizeof(patterns) / sizeof(patterns[0]); // For convinience, so don't have to manually edit for loop
        
        for (int p = 0; p < num_patterns; p++) {
            if (glob(patterns[p], 0, NULL, &g) == 0) {
                for (size_t i = 0; i < g.gl_pathc; i++) {
                    // Skip the /dev/fd directory itself if pattern is /dev/fd*
                    if (strcmp(g.gl_pathv[i], "/dev/fd") == 0) continue;
                    
                    create_block_symlink(g.gl_pathv[i]);
                }
                globfree(&g);
            }
        }
		sleep(3);
	}
	return 0;
}
