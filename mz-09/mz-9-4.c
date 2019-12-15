#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

enum { MODE = 0777 };

int
main(int argc, char *argv[])
{
    char name[PATH_MAX + 2];
    const char *path;
    if (getenv("XDG_RUNTIME_DIR")) {
        path = getenv("XDG_RUNTIME_DIR");
    } else if (getenv("TMPDIR")) {
        path = getenv("TMPDIR");
    } else {
        path = "/tmp";
    }
    snprintf(name, sizeof(name), "%s/pyscript.py", path);

    int fd = open(name, O_WRONLY | O_TRUNC | O_CREAT, MODE);
    FILE *file = fdopen(fd, "w");

    fprintf(file, "#!/usr/bin/env python3\n");
    fprintf(file, "import os\n");
    fprintf(file, "print(%s", argv[1]);

    for (int i = 2; i < argc; i++) {
        fprintf(file, "*%s", argv[i]);
    }
    fprintf(file, ")\n");
    fprintf(file, "os.remove(\"%s\")\n", name);
    fflush(file);
    fclose(file);
    execl(name, name, NULL);
    return 1;
}