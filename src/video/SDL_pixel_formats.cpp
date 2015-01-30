// SDL_pixel_formats.cpp with thread safety support

extern "C"
{
	#include "../SDL_internal.h"
	#include "SDL_endian.h"
	#include "SDL_blit.h"
	#include "SDL_pixels_c.h"
}

#ifdef SDL_THREAD_SAFE

#include <mutex>

static std::recursive_mutex SDL_lock;

#endif

extern "C"
{

static SDL_PixelFormat *formats;

SDL_PixelFormat *
SDL_AllocFormat(Uint32 pixel_format)
{
#ifdef SDL_THREAD_SAFE
	std::lock_guard<std::recursive_mutex> guard(SDL_lock);
#endif

    SDL_PixelFormat *format;

    /* Look it up in our list of previously allocated formats */
    for (format = formats; format; format = format->next) {
        if (pixel_format == format->format) {
            ++format->refcount;
            return format;
        }
    }

    /* Allocate an empty pixel format structure, and initialize it */
    format = (SDL_PixelFormat *) SDL_malloc(sizeof(*format));
    if (format == NULL) {
        SDL_OutOfMemory();
        return NULL;
    }
    if (SDL_InitFormat(format, pixel_format) < 0) {
        SDL_free(format);
        SDL_InvalidParamError("format");
        return NULL;
    }

    if (!SDL_ISPIXELFORMAT_INDEXED(pixel_format)) {
        /* Cache the RGB formats */
        format->next = formats;
        formats = format;
    }
    return format;
}

void
SDL_FreeFormat(SDL_PixelFormat *format)
{
#ifdef SDL_THREAD_SAFE
	std::lock_guard<std::recursive_mutex> guard(SDL_lock);
#endif

    SDL_PixelFormat *prev;

    if (!format) {
        SDL_InvalidParamError("format");
        return;
    }
    if (--format->refcount > 0) {
        return;
    }

    /* Remove this format from our list */
    if (format == formats) {
        formats = format->next;
    } else if (formats) {
        for (prev = formats; prev->next; prev = prev->next) {
            if (prev->next == format) {
                prev->next = format->next;
                break;
            }
        }
    }

    if (format->palette) {
        SDL_FreePalette(format->palette);
    }
    SDL_free(format);
}

}
