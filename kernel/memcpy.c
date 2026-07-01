#include "memcpy.h"

/*
 * ============================================================================
 * memcpy
 * ============================================================================
 */

void* memcpy(void* dest,
             const void* src,
             size_t size)
{
    uint8_t* d8 = (uint8_t*)dest;
    uint32_t* d32 = (uint32_t*)dest;
    
    const uint8_t* s8 = (const uint8_t*)src;
    const uint32_t* s32 = (const uint32_t*)src;
    const uint32_t count32 = size / sizeof(uint32_t);

    uint32_t i = 0U;

    for(;i<count32;i++)
    {
        d32[i] = s32[i];
    }
    for(i *= sizeof(uint32_t);i<size;i++)
    {
        d8[i] = s8[i];
    }

    return dest;
}

/*
 * ============================================================================
 * memset
 * ============================================================================
 */

void* memset(void* dest,
             uint8_t value,
             size_t size)
{
    uint8_t* d8 = (uint8_t*)dest;
    uint32_t* d32 = (uint32_t*)dest;
    
    const uint32_t value32 = (uint32_t)value | ((uint32_t)value << 8) | ((uint32_t)value << 16) | ((uint32_t)value << 24);
    const uint32_t count32 = size / sizeof(uint32_t);

    uint32_t i = 0U;

    for(;i<count32;i++)
    {
        d32[i] = value32;
    }
    for(i *= sizeof(uint32_t);i<size;i++)
    {
        d8[i] = value;
    }

    return dest;
}

/*
 * ============================================================================
 * memmove
 * ============================================================================
 */

void* memmove(void* dest,
              const void* src,
              size_t size)
{
    uint8_t* d = (uint8_t*)dest;
    const uint8_t* s = (const uint8_t*)src;

    if(d < s)
    {
        while(size--)
            *d++ = *s++;
    }
    else
    {
        d += size;
        s += size;

        while(size--)
            *(--d) = *(--s);
    }

    return dest;
}

/*
 * ============================================================================
 * memcmp
 * ============================================================================
 */

int memcmp(const void* a,
           const void* b,
           size_t size)
{
    const uint8_t* aa = (const uint8_t*)a;
    const uint8_t* bb = (const uint8_t*)b;

    while(size--)
    {
        if(*aa != *bb)
            return *aa - *bb;

        aa++;
        bb++;
    }

    return 0;
}