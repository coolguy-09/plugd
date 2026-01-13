#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <glob.h>
#include <string.h>
#include <errno.h>
#include <debug.h>

void create_block_symlink(const char *path) {
    struct stat st;
    char linkname[4096];
    char buf[4096];

    if (stat(path, &st) != 0) return;

    snprintf(linkname, sizeof(linkname), "/dev/block/%u:%u", 
             major(st.st_rdev), minor(st.st_rdev));

    if (lstat(linkname, &st) == 0) {
        if (S_ISLNK(st.st_mode)) {
            ssize_t len = readlink(linkname, buf, sizeof(buf) - 1);
            if (len != -1) {
                buf[len] = '\0';
                if (strcmp(buf, path) == 0) return;
            }
        }
        
        // If it's a directory, a file, or a wrong symlink, clear it out.
        if (remove(linkname) != 0) {
             fprintf(stderr, "plugd: cleaning wrong file/folder/symlink '%s' failed: %s\n", linkname, strerror(errno));
             return; 
        }
    }

    // Create the new link
    if (symlink(path, linkname) != 0) {
		if (errno == EEXIST) return;
        fprintf(stderr, "plugd: symlinking '%s' to '%s' failed: %s\n", path, linkname, strerror(errno));
		if (debug) fprintf(stderr, "[DEBUG] plugd: filename: '%s', symlink name: '%s'\n", path, linkname);
		return;
    } else {
		if (debug) fprintf(stderr, "[DEBUG] plugd: linked '%s' to '%s' successfully\n", path, linkname);
	}
}
