#ifndef _INIT_LIB_H_
#define _INIT_LIB_H_

int wait(int pid);
int sleep(int seconds);
int execve(const char* pathname, char* const argv[], char* const envp[]);

#endif