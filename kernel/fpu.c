#include "fpu.h"

void fpu_init(void)
{
    __asm__ volatile ("fninit");
}

void fpu_save_env(fpu_env_t *env)
{
    __asm__ volatile (
        "fnstenv (%0)"
        :
        : "r"(env)
        : "memory"
    );
}

void fpu_restore_env(const fpu_env_t *env)
{
    __asm__ volatile (
        "fldenv (%0)"
        :
        : "r"(env)
        : "memory"
    );
}

uint16_t fpu_get_control_word(void)
{
    uint16_t cw;

    __asm__ volatile (
        "fnstcw %0"
        : "=m"(cw)
    );

    return cw;
}

uint16_t fpu_get_status_word(void)
{
    uint16_t sw;

    __asm__ volatile (
        "fnstsw %0"
        : "=m"(sw)
    );

    return sw;
}
