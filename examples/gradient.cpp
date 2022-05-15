#include <CIL/ImageInfo.hpp>
#include <CIL/RayTracing/Camera.hpp>
#include <CIL/RayTracing/Ray.hpp>

CIL::ColorMap rayColor(const CIL::Ray& r)
{
    CIL::Vector3D unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    assert(t < 1.00);
    CIL::ColorMap o = (1.0 - t) * CIL::Color::WHITE + t * CIL::Color::BLUE;
    return o;
}

int main(int argc, const char** argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: ./bin <out.jpeg>";
        return 1;
    }
    const auto aspect_ratio = 16.0 / 9.0;

    // Camera
    auto viewport_height = 1.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    CIL::Camera cam(viewport_width, viewport_height, focal_length);

    // Image
    const int image_width = 800;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int num_components = 3;
    const int sample_depth = 8;

    CIL::ImageMatrix data(image_width, image_height, num_components,
                          sample_depth);
    // Implement multiprocessing here
    for (auto i : data)
    {
        CIL::Ray r = cam.get_ray(data, i);
        i.assign(rayColor(r));
    }
    CIL::ImageInfo img(CIL::ColorModel::COLOR_RGB, "JPEG", data);
    img.save(argv[1]);
    return 0;
}
