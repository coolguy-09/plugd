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

int main(int argc, char *argv[]) {
	if (argc > 1 && strcmp(argv[1], "--daemon") == 0) {
		daemonize();
	}
	if (argc > 1 && strcmp(argv[1], "--help") == 0) {
		printf("Usage for plugd: \n");
		printf("  plugd [--help|--version|--daemon]\n");
		printf("    --help: Show this help dialouge.\n");
		printf("    --version: Show the version of plugd.\n");
		printf("    --daemon: Run in the background.\n");
		return 0;
	}
	if (argc > 1 && strcmp(argv[1], "--version") == 0) {
		printf("plugd v1.0\n");
		return 0;
	}

	if (access("/dev/stdin", F_OK) != 0) { if (access("/proc/self/fd/0", F_OK) == 0) { symlink("/proc/self/fd/0", "/dev/stdin"); } }
	if (access("/dev/stdout", F_OK) != 0) { if (access("/proc/self/fd/1", F_OK) == 0) { symlink("/proc/self/fd/1", "/dev/stdout"); } }
	if (access("/dev/stderr", F_OK) != 0) { if (access("/proc/self/fd/2", F_OK) == 0) { symlink("/proc/self/fd/2", "/dev/stderr"); } }
	if (access("/dev/core", F_OK) != 0) { if (access("/proc/kcore", F_OK) == 0) { symlink("/proc/kcore", "/dev/core"); } }
	if (access("/dev/fd", F_OK) != 0) { if (access("/proc/self/fd", F_OK) == 0) { symlink("/proc/self/fd", "/dev/fd"); } }
	if (access("/dev/rtc", F_OK) != 0) { if (access("/dev/rtc0", F_OK) == 0) { symlink("/dev/rtc0", "/dev/rtc"); } }
	if (access("/dev/cdrom", F_OK) != 0) { if (access("/dev/sr0", F_OK) == 0) { symlink("/dev/sr0", "/dev/cdrom"); } }

	glob_t g;
	struct stat st;
	char linkname[128];

	if (mkdir("/dev/block", 0755) == -1 && errno != EEXIST) {
		fprintf(stderr, "plugd: mkdir failed: %s\n", strerror(errno));
		return 1;
	}

	while(1) {
		if (glob("/dev/sd*", 0, NULL, &g) == 0) {
			for (size_t i = 0; i < g.gl_pathc; i++) {
				if (stat(g.gl_pathv[i], &st) == 0) {
					int maj = major(st.st_rdev);   // call major()
					int min = minor(st.st_rdev);   // call minor()
					snprintf(linkname, sizeof(linkname), "/dev/block/%d:%d", maj, min);
					if (access(linkname, F_OK) == 0) {
						if (unlink(linkname) != 0) { fprintf(stderr, "plugd: unlink failed: %s\n", strerror(errno)); }
						if (symlink(g.gl_pathv[i], linkname) != 0) { fprintf(stderr, "plugd: symlink failed: %s\n", strerror(errno)); }
					} else {
						if (symlink(g.gl_pathv[i], linkname) != 0) { fprintf(stderr, "plugd: symlink failed: %s\n", strerror(errno)); }
					}
				} else {
					fprintf(stderr, "plugd: perror failed: %s\n", strerror(errno));
					return 1;
				}
			}
		}
		globfree(&g);
		if (glob("/dev/hd*", 0, NULL, &g) == 0) {
			for (size_t i = 0; i < g.gl_pathc; i++) {
				if (stat(g.gl_pathv[i], &st) == 0) {
					int maj = major(st.st_rdev);   // call major()
					int min = minor(st.st_rdev);   // call minor()
					snprintf(linkname, sizeof(linkname), "/dev/block/%d:%d", maj, min);
					if (access(linkname, F_OK) == 0) {
						if (unlink(linkname) != 0) { fprintf(stderr, "plugd: unlink failed: %s\n", strerror(errno)); }
						if (symlink(g.gl_pathv[i], linkname) != 0) { fprintf(stderr, "plugd: symlink failed: %s\n", strerror(errno)); }
					} else {
						if (symlink(g.gl_pathv[i], linkname) != 0) { fprintf(stderr, "plugd: symlink failed: %s\n", strerror(errno)); }
					}
				} else {
					fprintf(stderr, "plugd: perror failed: %s\n", strerror(errno));
					return 1;
				}
			}
		}
		globfree(&g);
		if (glob("/dev/zram*", 0, NULL, &g) == 0) {
			for (size_t i = 0; i < g.gl_pathc; i++) {
				if (stat(g.gl_pathv[i], &st) == 0) {
					int maj = major(st.st_rdev);   // call major()
					int min = minor(st.st_rdev);   // call minor()
					snprintf(linkname, sizeof(linkname), "/dev/block/%d:%d", maj, min);
					if (access(linkname, F_OK) == 0) {
						if (unlink(linkname) != 0) { fprintf(stderr, "plugd: unlink failed: %s\n", strerror(errno)); }
						if (symlink(g.gl_pathv[i], linkname) != 0) { fprintf(stderr, "plugd: symlink failed: %s\n", strerror(errno)); }
					} else {
						if (symlink(g.gl_pathv[i], linkname) != 0) { fprintf(stderr, "plugd: symlink failed: %s\n", strerror(errno)); }
					}
				} else {
					fprintf(stderr, "plugd: perror failed: %s\n", strerror(errno));
					return 1;
				}
			}
		}
		globfree(&g);
		if (glob("/dev/fd*", 0, NULL, &g) == 0) {
			for (size_t i = 0; i < g.gl_pathc; i++) {
				if (strcmp(g.gl_pathv[i], "/dev/fd") == 0) { continue; } else {
					if (stat(g.gl_pathv[i], &st) == 0) {
						int maj = major(st.st_rdev);   // call major()
						int min = minor(st.st_rdev);   // call minor()
						snprintf(linkname, sizeof(linkname), "/dev/block/%d:%d", maj, min);
						if (access(linkname, F_OK) == 0) {
							if (unlink(linkname) != 0) { fprintf(stderr, "plugd: unlink failed: %s\n", strerror(errno)); }
							if (symlink(g.gl_pathv[i], linkname) != 0) { fprintf(stderr, "plugd: symlink failed: %s\n", strerror(errno)); }
						} else {
							if (symlink(g.gl_pathv[i], linkname) != 0) { fprintf(stderr, "plugd: symlink failed: %s\n", strerror(errno)); }
						}
					} else {
						fprintf(stderr, "plugd: perror failed: %s\n", strerror(errno));
						return 1;
					}
				}
			}
		}
		globfree(&g);
		sleep(1);
	}
	return 0;
}
