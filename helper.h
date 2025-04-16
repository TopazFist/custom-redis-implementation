#pragma once  // or use include guards
#include <cstdint>
#include <cstddef> 

int32_t read_full(int fd, char *buf, size_t n);
int32_t write_all(int fd, const char *buf, size_t n);
void die(const char *msg);
void msg(const char *msg);