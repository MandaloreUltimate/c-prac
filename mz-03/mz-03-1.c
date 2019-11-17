int
bitcount(STYPE value)
{
    int bit_cnt = 0;
    UTYPE u_value = (UTYPE)value;
    while (u_value) {
        bit_cnt += 1 & u_value;
        u_value >>= 1;
    }
    return bit_cnt;
}