#include <stdio.h>
#include <setjmp.h>
#include <stdint.h>
#include <iostream>

typedef enum
{
    COLOR_RBG,
    COLOR_RBGA,
    COLOR_CMYK,
} COLOR_MODELS;

struct ImageInfo
{
    uint32_t width;
    uint32_t height;
    int num_components;
    COLOR_MODELS color_model;
    uint8_t* data;
    void print_image_info()
    {
        std :: cerr << "Width:\t" << width <<
                    "\nHeight:\t" << height <<
                    "\nNum Components:\t" << num_components <<
                    "\nColor space:\t" << color_model << "\n";
    }
};
