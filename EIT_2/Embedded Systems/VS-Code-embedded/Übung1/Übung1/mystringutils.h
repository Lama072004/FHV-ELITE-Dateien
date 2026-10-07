#pragma once

#ifndef MYSTRINGUTILS_H_
#define MYSTRINGUTILS_H_

#include <stdint.h>
#include <stdbool.h>

uint32_t mystringlen(char str[]);
void toupper(char str[]);
char toupper_char(char c);
bool mystringcomp(char str1[], char str2[]);
char* mystringcpy(char original[]);


#endif //MYSTRINGUTILS_H_