#include <CIL/ImageInfo.hpp>
#include <CIL/Transformations.hpp>
#include <iostream>
int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: ./bin <in.png>";
        return 1;
    }
    CIL::ImageInfo image1 = CIL::readImage(argv[1]);
    CIL::ImageInfo image2 = CIL::readImage(argv[1]);
    CIL::rotate(image1, 45);
    CIL::rotate(image2, 45);
    if (CIL::equal(image1, image2)) {
      std::cout<<"Images are equal\n";
    } else {
      std::cout<<"Images are not equal\n";
    }
    return 0;
}
