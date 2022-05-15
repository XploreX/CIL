#ifndef CIL_RAY_TRACING_WORLD_HPP
#define CIL_RAY_TRACING_WORLD_HPP

#include <CIL/RayTracing/HitInfo.hpp>
#include <CIL/RayTracing/Object.hpp>

#include <limits>
#include <memory>
#include <vector>

namespace CIL {
    class World
    {
      public:
        World() = default;

        void add(std::shared_ptr<Object> object)
        {
            m_objects.push_back(object);
        }

        void clear() { m_objects.clear(); }

        bool hit(const Ray& ray, double dist_begin, double dist_end,
                 HitInfo& hit_info)
        {
            bool hit_anything = false;
            for (auto obj : m_objects)
            {
                bool hit_success = obj->hit(ray, dist_begin, dist_end,
                                            hit_info);
                if (hit_success)
                {
                    hit_anything = true;
                    dist_end = hit_info.distance;
                }
            }
            return hit_anything;
        }

      private:
        std::vector<std::shared_ptr<Object>> m_objects;
    };
} // namespace CIL

#endif