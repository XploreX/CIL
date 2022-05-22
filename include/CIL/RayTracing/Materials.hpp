#ifndef CIL_MATERIALS_HPP
#define CIL_MATERIALS_HPP
#include <CIL/RayTracing/HitInfo.hpp>
#include <CIL/RayTracing/Ray.hpp>
#include <CIL/ThreadHandler.hpp>

namespace CIL {
    class Material
    {
      public:
        virtual bool scatter(const Ray& ray, const HitInfo& hit_info,
                             Vector3D& attenuation, Ray& scattered) const = 0;
        virtual ~Material() = default;
    };
    class Lambertian : public Material
    {
      public:
        Lambertian(const Vector3D& a) : albedo(a) {}

        virtual bool scatter(const Ray& ray, const HitInfo& hit_info,
                             Vector3D& attenuation,
                             Ray& scattered) const override
        {
            UNUSED(ray);
            auto scatter_direction = hit_info.normal +
                                     unit_vector(Vector3D::random());

            // Catch degenerate scatter direction
            if (scatter_direction.near_zero())
                scatter_direction = hit_info.normal;

            scattered = Ray(hit_info.hit_point, scatter_direction);
            attenuation = albedo;
            return true;
        }

      public:
        Vector3D albedo;
    };
} // namespace CIL

#endif
