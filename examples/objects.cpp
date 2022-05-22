#include <CIL/ImageInfo.hpp>
#include <CIL/RayTracing/Background.hpp>
#include <CIL/RayTracing/Camera.hpp>
#include <CIL/RayTracing/HitInfo.hpp>
#include <CIL/RayTracing/Materials.hpp>
#include <CIL/RayTracing/Ray.hpp>
#include <CIL/RayTracing/Sphere.hpp>
#include <CIL/RayTracing/World.hpp>
#include <CIL/Transformations.hpp>

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
    auto viewport_height = 2.0;
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

    auto material1 = std::shared_ptr<CIL::Lambertian>(
        new CIL::Lambertian(CIL::Vector3D(0.4, 0.2, 0.5)));

    auto ground = std::shared_ptr<CIL::Object>(
        new CIL::Sphere(CIL::Point3D(0, -100.5, -1), 100, material1,
                        [](const CIL::Ray& r, CIL::HitInfo& hit_info) {
                            UNUSED(r);
                            UNUSED(hit_info);
                            CIL::ColorMap color = CIL::Color::GREEN;
                            return color;
                        }));

    auto sphere_one = std::shared_ptr<CIL::Object>(
        new CIL::Sphere(CIL::Point3D(0, 0, -1), 0.5, material1,
                        [](const CIL::Ray& r, CIL::HitInfo& hit_info) {
                            UNUSED(r);
                            CIL::ColorMap color = static_cast<CIL::ColorMap>(
                                255.0 * hit_info.normal.getInRange0to1());
                            return color;
                        }));

    auto sphere_two = std::shared_ptr<CIL::Object>(
        new CIL::Sphere(CIL::Point3D(-1.7, 0, -5), 2, nullptr,
                        [](const CIL::Ray& r, CIL::HitInfo& hit_info) {
                            UNUSED(r);
                            CIL::ColorMap color = static_cast<CIL::ColorMap>(
                                255.0 * hit_info.normal.getInRange0to1());
                            return color;
                        }));

    auto sphere_three = std::shared_ptr<CIL::Object>(
        new CIL::Sphere(CIL::Point3D(3.5, -1, -5), 1, nullptr,
                        [](const CIL::Ray& r, CIL::HitInfo& hit_info) {
                            UNUSED(r);
                            UNUSED(hit_info);
                            return CIL::Color::GREEN;
                        }));

    world.add(background);
    world.add(sphere_one);
    // world.add(sphere_two);
    //  world.add(sphere_three);
    world.add(ground);

    world.generate_image(cam, data);

    CIL::ImageInfo img(CIL::ColorModel::COLOR_RGB, "JPEG", data);
    CIL::changeGamma(img, 2);
    img.save(argv[1]);
    return 0;
}
