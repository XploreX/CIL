#ifndef CIL_IMAGE_HANDLER_H
#define CIL_IMAGE_HANDLER_H

#include <string>

namespace CIL {
    class ImageInfo;
    class ImageHandler
    {
      public:
        virtual CIL::ImageInfo read(const char* filename) = 0;
        virtual bool write(const CIL::ImageInfo* cil_img_info,
                           const char* filename) = 0;
        virtual bool isSupportedFile(const char* filename) = 0;
        virtual std::string imageType() = 0;
        virtual void destroyInternalInfo(const CIL::ImageInfo* cil_img_info);
        virtual void* cloneInternalInfo(void* internal_img_info);
        virtual ~ImageHandler() = default;
    };
} // namespace CIL

#endif