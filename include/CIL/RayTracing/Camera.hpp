#ifndef CIL_CAMERA_HPP
#define CIL_CAMERA_HPP

#include <CIL/ImageMatrix.hpp>
#include <CIL/RayTracing/Ray.hpp>

namespace CIL {
    class Camera
    {
      public:
        Camera(double viewport_width, double viewport_height,
               double focal_length = 1.0,
               Point3D lookfrom = Point3D(0, 0, 0),
               Point3D lookat = Point3D(0, 0, -1),
               Vector3D vertical_direction = Point3D(0, 1, 0)
               )
            : m_horizontal(Vector3D(viewport_width, 0, 0)),
              m_vertical(Vector3D(0.0, viewport_height, 0.0))
        {
            (void)(focal_length);
            auto z_axis = unit_vector(lookfrom - lookat);
            auto y_axis = unit_vector(vertical_direction);
            auto x_axis = cross(z_axis, y_axis);
            m_origin = lookfrom;
            m_horizontal = viewport_width * x_axis;
            m_vertical = viewport_height * y_axis;
            m_lower_left_corner = m_origin - m_horizontal / 2 - m_vertical / 2 -
                                  z_axis;
        }

        Ray get_ray(double normalised_x, double normalised_y) const
        {
            return Ray(m_origin, m_lower_left_corner +
                                     normalised_x * m_horizontal +
                                     normalised_y * m_vertical - m_origin);
        }

        Ray get_ray(const ImageMatrix& img_data, const Pixel p,
                    double x_rand = 0, double y_rand = 0) const
        {
            double normalised_x = static_cast<double>(p.col() + x_rand) /
                                  (img_data.width() - 1 + x_rand);
            double normalised_y = 1 - static_cast<double>(p.row() + y_rand) /
                                          (img_data.height() - 1 + y_rand);
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
