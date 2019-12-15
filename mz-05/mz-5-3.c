void
normalize_path(char *buf)
{
    char *tmp = buf;
    while (*tmp) {
        *buf++ = *tmp;
        if (*tmp != '/') {
            tmp++;
        } else {
            while(*tmp  == '/') {
                tmp++;
            }
        }
    }
    *buf = 0;
}