#ifndef CIL_RAY_TRACING_WORLD_HPP
#define CIL_RAY_TRACING_WORLD_HPP

#include <CIL/RayTracing/Camera.hpp>
#include <CIL/RayTracing/HitInfo.hpp>
#include <CIL/RayTracing/Object.hpp>
#include <CIL/ThreadHandler.hpp>

#include <limits>
#include <memory>
#include <vector>
#include <random>

namespace CIL {
    class World
    {
      public:
        World(std::size_t samples = 1) : m_samples(samples){};

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
                Vector3D pixel_color;
                for (std::size_t i = 0; i < m_samples; ++i)
                {
                    CIL::Ray ray = cam.get_ray(data, data(r, c),
                                               random_in_0_to_1(),
                                               random_in_0_to_1());
                    pixel_color += rayColor(ray);
                }
                pixel_color /= m_samples;
                data(r, c).assign(ColorMap(pixel_color));
            };

            th.process_matrix(data.width(), data.height());
        }

      private:
        std::vector<std::shared_ptr<Object>> m_objects;
        std::size_t m_samples = 1;
        double random_in_0_to_1()
        {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            static std::uniform_real_distribution<double> dist(0.0, 1.0);
            return dist(gen);
        }
    };
} // namespace CIL

#endif
