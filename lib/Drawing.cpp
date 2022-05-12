#include <CIL/Drawing.hpp>
#include <CIL/ImageInfo.hpp>

namespace CIL {
    void drawPoint(Pixel px, const ColorMap color) { px.assign(color); }
    ColorMap operator*(const ColorMap& c1, double scale)
    {
        // Do all calculations in double to avoid calculation errors by int
        std::vector<double> color(c1.numComponents());

        for (auto i = 0U; i < c1.numComponents(); ++i)
            color[i] = scale * c1[i];

        return color;
    }
    ColorMap operator*(double scale, const ColorMap& c1) { return c1 * scale; }
    ColorMap operator+(const ColorMap& c1, const ColorMap& c2)
    {
        assert(c1.numComponents() == c2.numComponents());
        std::vector<double> c(c1.numComponents());

        for (auto i = 0U; i < c1.numComponents(); ++i)
            c[i] = c1[i] + c2[i];

        return c;
    }
    ColorMap operator+(const ColorMap& c1, double val)
    {
        std::vector<double> c(c1.numComponents());

        for (auto i = 0U; i < c1.numComponents(); ++i)
            c[i] = c1[i] + val;

        return c;
    }
    ColorMap operator+(double val, const ColorMap& c1) { return c1 * val; }
    std::ostream& operator<<(std::ostream& os, CIL::ColorMap c1)
    {
        for (auto i : c1.m_components)
            os << static_cast<int>(i) << ' ';
        os << "\n";
        return os;
    }

} // namespace CIL
