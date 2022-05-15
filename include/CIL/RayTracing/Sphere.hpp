#ifndef CIL_SPHERE_HPP
#define CIL_SPHERE_HPP

#include <CIL/Drawing.hpp>
#include <CIL/RayTracing/HitInfo.hpp>
#include <CIL/RayTracing/Object.hpp>
#include <CIL/RayTracing/Ray.hpp>
#include <CIL/RayTracing/Vector3D.hpp>

namespace CIL {
    class Sphere : public Object
    {
      public:
        Sphere(const Point3D& center, double radius, const ColorMap& color)
            : m_center(center), m_radius(radius), m_color(color)
        {}

        bool hit(const Ray& ray, double dist_begin, double dist_end,
                 HitInfo& hit_info) override
        {
            auto center_to_ray_origin = ray.origin() - m_center;
            auto a = ray.direction().magnitude_squared();
            auto b = 2 * dot(ray.direction(), center_to_ray_origin);
            auto c = center_to_ray_origin.magnitude_squared() -
                     m_radius * m_radius;
            auto D = b * b - 4 * a * c;
            if (D < 0)
                return false;
            auto sqrt_d = std::sqrt(D);
            auto t = (-b - sqrt_d) / (2 * a);
            auto distance = ray.length(t);
            if (distance < dist_begin || distance >= dist_end)
            {
                t = (-b + sqrt_d) / (2 * a);
                distance = ray.length(t);
                if (distance < dist_begin || distance >= dist_end)
                    return false;
            }

            hit_info.hit_point = ray.at(t);
            hit_info.distance = distance;
            hit_info.color = m_color;
            return true;
        }

      private:
        Point3D m_center;
        double m_radius;
        ColorMap m_color;
    };
} // namespace CIL

#endif