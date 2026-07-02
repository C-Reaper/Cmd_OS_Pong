#ifndef FPU_H
#define FPU_H

#include <stdint.h>

typedef struct
{
    uint8_t data[28];
} fpu_env_t;

void fpu_init(void);
void fpu_save_env(fpu_env_t *env);
void fpu_restore_env(const fpu_env_t *env);
uint16_t fpu_get_control_word(void);
uint16_t fpu_get_status_word(void);

#endif
