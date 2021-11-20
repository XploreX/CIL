#ifndef CIL_IMAGE_INFO_H
#define CIL_IMAGE_INFO_H
#include <iostream>
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>

namespace CIL {
    enum class COLOR_MODELS
    {
        COLOR_RBG,
        COLOR_RBGA,
        COLOR_CMYK,
        COLOR_GRAYSCALE,
    };

    struct ImageInfo
    {
        uint32_t width;
        uint32_t height;
        int num_components;
        COLOR_MODELS color_model;
        uint8_t* data;
        void print_image_info()
        {
            std ::cerr << "Width:\t" << width << "\nHeight:\t" << height
                       << "\nNum Components:\t" << num_components
                       << "\nColor space:\t" << static_cast<int>(color_model)
                       << "\n";
        }
    };

} // namespace CIL
#endif
