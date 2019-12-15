#include <sys/stat.h>
#include <unistd.h>

struct Task
{
    unsigned uid;
    int gid_count;
    unsigned *gids;
};

enum
{
    GID_OFF_T = 3,
    UID_OFF_T = 6,
};

static int
check_mode(unsigned mode, int access)
{
    return (access & mode) == access;
}

static int
check_uid(unsigned stb_uid, unsigned task_uid)
{
    return stb_uid == task_uid;
}

static int
check_gid(unsigned stb_gid, int gid_count, unsigned *gids)
{
    for (int i = 0; i < gid_count; i++) {
        if (gids[i] == stb_gid) {
            return 1;
        }
    }
    return 0;
}

int
myaccess(const struct stat *stb, const struct Task *task, int access)
{
    if (!(task->uid)) {
        return 1;
    }

    if (check_uid(stb->st_uid, task->uid)) {
        return check_mode((stb->st_mode & S_IRWXU) >> UID_OFF_T, access);
    }

    if (check_gid(stb->st_gid, task->gid_count, task->gids)) {
        return check_mode((stb->st_mode & S_IRWXG) >> GID_OFF_T, access);
    }

    return check_mode(stb->st_mode & S_IRWXO, access);
}