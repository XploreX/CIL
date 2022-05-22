#include <CIL/ImageInfo.hpp>
#include <CIL/RayTracing/Background.hpp>
#include <CIL/RayTracing/Camera.hpp>
#include <CIL/RayTracing/HitInfo.hpp>
#include <CIL/RayTracing/Ray.hpp>
#include <CIL/RayTracing/Sphere.hpp>
#include <CIL/RayTracing/World.hpp>

#include <limits>

CIL::World world(8);

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
    const int image_width = 1280;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int num_components = 3;
    const int sample_depth = 8;

    CIL::ImageMatrix data(image_width, image_height, num_components,
                          sample_depth);

    auto background = std::shared_ptr<CIL::Object>(
        new CIL::Background([&](const CIL::Ray& ray) {
            // changing the range of t from 0 to 1
            auto t = ray.direction().getInRange0to1().y();
            assert(t >= 0.0 && t < 1.00);
            CIL::ColorMap o = (1.0 - t) * CIL::Color::WHITE +
                              t * CIL::Color::BLUE;
            return o;
        }));

    auto sphere_one = std::shared_ptr<CIL::Object>(
        new CIL::Sphere(CIL::Point3D(0, 0, -6), 2,
                        [](const CIL::Ray& r, CIL::HitInfo& hit_info) {
                            UNUSED(r);
                            CIL::ColorMap color = static_cast<CIL::ColorMap>(
                                255.0 * hit_info.normal.getInRange0to1());
                            hit_info.color = color;
                        }));

    auto sphere_two = std::shared_ptr<CIL::Object>(
        new CIL::Sphere(CIL::Point3D(-1.7, 0, -5), 2,
                        [](const CIL::Ray& r, CIL::HitInfo& hit_info) {
                            UNUSED(r);
                            CIL::ColorMap color = static_cast<CIL::ColorMap>(
                                CIL::Color::RED -
                                255.0 * hit_info.normal.getInRange0to1());
                            hit_info.color = color;
                        }));

    world.add(background);
    world.add(sphere_one);
    world.add(sphere_two);

    world.generate_image(cam, data);

    CIL::ImageInfo img(CIL::ColorModel::COLOR_RGB, "JPEG", data);
    img.save(argv[1]);
    return 0;
}
