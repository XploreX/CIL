#ifndef CIL_COLOR_HPP
#define CIL_COLOR_HPP

#include <CIL/Core/Debug.hpp>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>

namespace CIL {
    class Vector3D;
    class ColorMap
    {
      public:
        using ValueType = uint8_t;

      private:
        std::vector<ValueType> m_components;

      public:
        ColorMap() = default;
        ColorMap(const int c) { m_components.resize(c); }
        ColorMap(const std::initializer_list<ValueType>& components)
            : m_components(components)
        {}
        ColorMap(std::initializer_list<ValueType>&& components) noexcept
            : m_components(std::move(components))
        {}
        ColorMap(const std::vector<double>& components)
        {
            m_components.resize(components.size());
            uint8_t max = std::numeric_limits<uint8_t>::max();
            for (auto i = 0U; i < m_components.size(); i++)
                m_components[i] = ((components[i] > static_cast<uint8_t>(max))
                                       ? max
                                   : (components[i] < 0) ? 0
                                                         : components[i]);
        }
        ColorMap(const CIL::Vector3D& v);
        ColorMap(const ColorMap& other) : m_components(other.m_components) {}
        ColorMap& operator=(const ColorMap& rhs)
        {
            m_components = rhs.m_components;
            return *this;
        }
        ValueType& operator[](int idx) { return m_components[idx]; }
        const ValueType& operator[](int idx) const { return m_components[idx]; }
        std::size_t numComponents() const { return m_components.size(); }
        void showMap()
        {
            for (auto i = 0U; i < m_components.size(); i++)
                std::cerr << m_components[i] << ' ';
        }
        bool operator==(const ColorMap& c1) const
        {
            assert(c1.numComponents() == numComponents());
            for (auto i = 0U; i < numComponents(); i++)
                if ((*this)[i] != c1[i])
                    return false;
            return true;
        }
        friend std::ostream& operator<<(std::ostream& os, CIL::ColorMap c1);
    };

    bool operator==(const Vector3D& v, const ColorMap& c);
    bool operator==(const ColorMap& c, const Vector3D& v);

    namespace Color {
        const ColorMap::ValueType
            max = std::numeric_limits<ColorMap::ValueType>::max();
        const ColorMap RED = {max, 0, 0};
        const ColorMap GREEN = {0, max, 0};
        const ColorMap BLUE = {0, 0, max};
        const ColorMap BLACK = {0, 0, 0};
        const ColorMap WHITE = {max, max, max};
    } // namespace Color
} // namespace CIL
#endif
