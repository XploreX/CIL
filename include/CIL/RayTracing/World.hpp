#ifndef CIL_RAY_TRACING_WORLD_HPP
#define CIL_RAY_TRACING_WORLD_HPP

#include <CIL/RayTracing/Camera.hpp>
#include <CIL/RayTracing/HitInfo.hpp>
#include <CIL/RayTracing/Object.hpp>
#include <CIL/ThreadHandler.hpp>

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

        CIL::ColorMap rayColor(const CIL::Ray& ray)
        {
            double dist_begin = 0;
            double dist_end = std::numeric_limits<double>::infinity();
            CIL::HitInfo hit_info;
            this->hit(ray, dist_begin, dist_end, hit_info);
            // background will always be hit
            assert(hit_info.is_valid());
            return hit_info.color;
        }

        void generate_image(const CIL::Camera& cam, CIL::ImageMatrix& data)
        {
            CIL::ThreadHandler th;

            th.fn = [&](int r, int c) {
                CIL::Ray ray = cam.get_ray(data, data(r, c));
                data(r, c).assign(rayColor(ray));
            };

            th.process_matrix(data.width(), data.height());
        }

      private:
        std::vector<std::shared_ptr<Object>> m_objects;
    };
} // namespace CIL

#endif
