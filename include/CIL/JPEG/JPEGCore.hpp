#include <CIL/ImageInfo.hpp>
#include <iostream>
#include <jpeglib.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>

namespace CIL {
    namespace JPEG {
        typedef J_COLOR_SPACE JColorType;

        class ImageInfo
        {
            uint32_t m_width;
            uint32_t m_height;
            int m_num_components;
            JColorType m_color_model;
            uint8_t* m_data;

          public:
            // copy constructor
            ImageInfo(const JPEG::ImageInfo&);
            ImageInfo& operator=(const ImageInfo&);
            // move constructor
            ImageInfo(ImageInfo&& img);
            ImageInfo& operator=(ImageInfo&& img);

            CIL::ImageInfo toCILImage();
            static const JPEG::ImageInfo*
            fromCILImage(const CIL::ImageInfo* cil_imageinfo);
            uint32_t width() const { return m_width; }
            uint32_t height() const { return m_height; }
            uint8_t sampleDepth() const { return 8; }
            JColorType colorType() const { return m_color_type; };
            int numComponents() const { return m_num_channels; }
        };
    } // namespace JPEG
} // namespace CIL
