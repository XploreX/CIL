#include <CIL/ImageInfo.hpp>
#include <CIL/JPEG/JPEGHandler.hpp>

namespace CIL {
    void ImageInfo::print_image_info()
    {
        std ::cerr << "Width:\t" << width << "\nHeight:\t" << height
                   << "\nNum Components:\t" << num_components
                   << "\nColor space:\t" << static_cast<int>(color_model)
                   << "\n";
    }
    void ImageInfo::destroy(ImageInfo* img_info)
    {
        JPEG::JpegHandler::destroy(img_info);
        delete[] img_info->data;
        delete img_info;
    }
} // namespace CIL
