// ebtry.c
#include "minicrt.h"

extern int main(int argc, char *argv[]);
void exit(int);

static void crt_fatal_error(const char *msg)
{
    // printf("faltal error: %s", msg);
    exit(1);
}

void mini_crt_entry(void)
{
    int ret;
    int argc;
    char **argv;
    char *ebp_reg = 0;
    asm(
        "movl %%ebp, %0 \n\t"
        : "=r"(ebp_reg));
    argc = *(int *)(ebp_reg + 4);
    argv = (char **)(ebp_reg + 8);

    if (!mini_crt_heap_init())
        crt_fatal_error("heap initialize failed");
    if (!mini_crt_io_init())
        crt_fatal_error("IO initialize failed");

    ret = main(argc, argv);
    exit(ret);
}

void exit(int exitCode)
{
    // mini_crt_call_exit_routine();
    asm(
        "movl %0, %%ebx \n\t"
        "movl $1, %%eax \n\t"
        "int $0x80  \n\t"
        "hlt    \n\t"
        :
        : "m"(exitCode)
        : "%ebx", "%eax");
}