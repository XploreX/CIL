#ifndef CIL_IMAGE_INFO_H
#define CIL_IMAGE_INFO_H
#include <iostream>
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>

namespace CIL {
    enum class ColorModels
    {
        COLOR_RGB,
        COLOR_RGBA,
        COLOR_CMYK,
        COLOR_GRAY,
        COLOR_GRAY_ALPHA,
        COLOR_PALETTE
    };

    struct ImageInfo
    {
        uint32_t width;
        uint32_t height;
        int num_components;
        ColorModels color_model;
        uint8_t* data;
        void print_image_info()
        {
            std ::cerr << "Width:\t" << width << "\nHeight:\t" << height
                       << "\nNum Components:\t" << num_components
                       << "\nColor space:\t" << static_cast<int>(color_model)
                       << "\n";
        }
        const void *internal_info = nullptr;
    };

} // namespace CIL
#endif
