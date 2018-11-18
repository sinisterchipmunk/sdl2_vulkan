#ifndef SDL2_VULKAN_H
#define SDL2_VULKAN_H 1

#include "ruby.h"
#include "SDL2/SDL_vulkan.h"

#if SIZEOF_VOIDP == SIZEOF_LONG
# define PTR2NUM(x)   (LONG2NUM((long)(x)))
# define NUM2PTR(x)   ((void*)(NUM2ULONG(x)))
#else
# define PTR2NUM(x)   (LL2NUM((LONG_LONG)(x)))
# define NUM2PTR(x)   ((void*)(NUM2ULL(x)))
#endif

#endif /* SDL2_VULKAN_H */
