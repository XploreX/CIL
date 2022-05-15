#include <CIL/ImageInfo.hpp>
#include <CIL/RayTracing/Background.hpp>
#include <CIL/RayTracing/Camera.hpp>
#include <CIL/RayTracing/HitInfo.hpp>
#include <CIL/RayTracing/Ray.hpp>
#include <CIL/RayTracing/Sphere.hpp>
#include <CIL/RayTracing/World.hpp>

#include <limits>

CIL::World world;

CIL::ColorMap rayColor(const CIL::Ray& ray)
{
    double dist_begin = 0;
    double dist_end = std::numeric_limits<double>::infinity();
    CIL::HitInfo hit_info;
    world.hit(ray, dist_begin, dist_end, hit_info);
    assert(hit_info.is_valid());
    return hit_info.color;
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
    const int image_width = 1280;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int num_components = 3;
    const int sample_depth = 8;

    CIL::ImageMatrix data(image_width, image_height, num_components,
                          sample_depth);

    auto background = std::shared_ptr<CIL::Object>(
        new CIL::Background([&](const CIL::Ray& ray) {
            CIL::Vector3D unit_direction = unit_vector(ray.direction());
            auto t = 0.5 * (unit_direction.y() + 1.0);
            assert(t < 1.00);
            CIL::ColorMap o = (1.0 - t) * CIL::Color::WHITE +
                              t * CIL::Color::BLUE;
            return o;
        }));

    world.add(background);
    auto black_sphere = std::shared_ptr<CIL::Object>(
        new CIL::Sphere(CIL::Point3D(0, 0, -6), 2, CIL::Color::BLACK));
    auto red_sphere = std::shared_ptr<CIL::Object>(
        new CIL::Sphere(CIL::Point3D(-1.7, 0, -5), 2, CIL::Color::RED));

    world.add(red_sphere);
    world.add(black_sphere);
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
