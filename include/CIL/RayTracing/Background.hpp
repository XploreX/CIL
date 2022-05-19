#ifndef CIL_RAY_TRACING_BACKGROUND_HPP
#define CIL_RAY_TRACING_BACKGROUND_HPP

#include <CIL/Color.hpp>
#include <CIL/RayTracing/HitInfo.hpp>
#include <CIL/RayTracing/Object.hpp>
#include <CIL/RayTracing/Ray.hpp>

#include <functional>
#include <limits>

namespace CIL {
    class Background : public Object
    {
      public:
        Background(std::function<ColorMap(const Ray& ray)> compute_color)
            : m_compute_color(compute_color)
        {}

        bool hit(const Ray& ray, double dist_begin, double dist_end,
                 HitInfo& hit_info)
        {
            auto distance = std::numeric_limits<double>::max();
            if (distance < dist_begin || distance >= dist_end)
                return false;
            hit_info.distance = distance;
            hit_info.hit_point = Point3D(distance, distance, distance);
            hit_info.color = m_compute_color(ray);
            return true;
        }

      private:
        std::function<ColorMap(const Ray& ray)> m_compute_color;
    };
} // namespace CIL

#endif