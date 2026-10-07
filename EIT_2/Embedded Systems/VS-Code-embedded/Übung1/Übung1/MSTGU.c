#include "mystringutils.h"

uint32_t mystringlen(char str[])
{
    uint32_t len = 0;
    for (; str[len] != '\0'; len++);
    return len;
}

void toupper(char str[])
{
    for (int i = 0; str[i] != '\0'; i++) 
    {
        str[i] = toupper_char(str[i]);
    }
}

char toupper_char(char c)
{
    if (c >= 'a' && c <= 'z') 
    {
        return c - 32;
    }
    return c;
}

bool mystringcomp(char str1[], char str2[])
{
    for (int i = 0; str1[i] != '\0' || str2[i] != '\0'; i++) {
        if (str1[i] != str2[i]) {
            return false;
        }
    }
    return true;
}

char* mystringcpy(char original[])
{
    
    return ;
}