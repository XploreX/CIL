#ifndef CIL_CAMERA_HPP
#define CIL_CAMERA_HPP

#include <CIL/ImageMatrix.hpp>
#include <CIL/RayTracing/Ray.hpp>

namespace CIL {
    class Camera
    {
      public:
        Camera(const double viewport_width, const double viewport_height,
               const double focal_length = 1.0,
               const Point3D origin = Point3D(0, 0, 0))
            : m_origin(origin), m_horizontal(Vector3D(viewport_width, 0, 0)),
              m_vertical(Vector3D(0.0, viewport_height, 0.0))
        {
            m_lower_left_corner = m_origin - m_horizontal / 2 - m_vertical / 2 -
                                  Vector3D(0, 0, focal_length);
        }

        Ray get_ray(double normalised_x, double normalised_y) const
        {
            return Ray(m_origin, m_lower_left_corner + normalised_x * m_horizontal +
                                     normalised_y * m_vertical - m_origin);
        }

        Ray get_ray(const ImageMatrix& img_data, const Pixel p) const
        {
            double normalised_x = static_cast<double>(p.col()) / (img_data.width() - 1);
            double normalised_y = 1 - static_cast<double>(p.row()) / (img_data.height() - 1);
            return get_ray(normalised_x, normalised_y);
        }

      private:
        Point3D m_origin;
        Point3D m_lower_left_corner;
        Vector3D m_horizontal;
        Vector3D m_vertical;
    };
} // namespace CIL
#endif
