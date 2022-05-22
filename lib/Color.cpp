#include <CIL/Color.hpp>
#include <CIL/RayTracing/Vector3D.hpp>

namespace CIL {
    ColorMap::ColorMap(const CIL::Vector3D& v) { *this = ColorMap(v.coord); }
    bool operator==(const Vector3D& v, const ColorMap& c)
    {
        return static_cast<CIL::ColorMap>(v) == c;
    }
    bool operator==(const ColorMap& c, const Vector3D& v) { return v == c; }
    std::ostream& operator<<(std::ostream& os, CIL::ColorMap c1)
    {
        for (auto i : c1.m_components)
            os << static_cast<int>(i) << ' ';
        os << "\n";
        return os;
    }
} // namespace CIL
