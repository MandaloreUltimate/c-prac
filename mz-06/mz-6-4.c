#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/sysmacros.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    DIR *dir;
    if (!(dir = opendir(argv[1]))) {
        perror("Failed to open directory: ");
        exit(1);
    }

    struct stat sb;
    uid_t cur_uid = getuid();
    struct dirent *dd;
    char path[PATH_MAX];
    long long sum_size = 0;
    int concat_res;

    while ((dd = readdir(dir))) {
        concat_res = snprintf(path, sizeof(path), "%s/%s", argv[1], dd->d_name);
        if (concat_res < PATH_MAX && !stat(path, &sb) && S_ISREG(sb.st_mode) && (sb.st_uid == cur_uid)
            && isupper((unsigned char) dd->d_name[0])) {
            sum_size += sb.st_size;
        }
    }
    printf("%lld\n", sum_size);
    closedir(dir);
    return 0;
}
