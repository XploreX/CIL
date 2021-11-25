#ifndef CIL_IMAGE_INFO_H
#define CIL_IMAGE_INFO_H
#include <iostream>
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>

#ifdef HAVE_CONFIG_H
#include <config.h> // when build by autotools, this will be included
#endif

namespace CIL {
    enum class ColorModel
    {
        COLOR_RGB,
        COLOR_RGBA,
        COLOR_CMYK,
        COLOR_GRAY,
        COLOR_GRAY_ALPHA,
        COLOR_PALETTE
    };

    enum class ImageType
    {
        JPEG,
        PNG,
        PPM
    };

    struct ImageInfo
    {
        uint32_t width;
        uint32_t height;
        int num_components;
        ColorModel color_model;
        ImageType image_type;
        uint8_t* data;
        uint8_t sample_depth;
        void print_image_info();
        const void* internal_info = nullptr;
        static void destroy(ImageInfo* img_info);
    };

} // namespace CIL
#endif
