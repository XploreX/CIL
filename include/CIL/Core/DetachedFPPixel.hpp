#ifndef CIL_CORE_DETACHED_FP_PIXEL_HPP
#define CIL_CORE_DETACHED_FP_PIXEL_HPP
#include <cassert>
#include <cstdint>
#include <vector>

namespace CIL {
    class Pixel;
    class DetachedFPPixel
    {
      public:
        using ValueType = double;

      private:
        std::vector<ValueType> m_components;

      public:
        DetachedFPPixel(int num_of_componenets)
            : m_components(num_of_componenets, 0)
        {}
        DetachedFPPixel(const Pixel& px);
        DetachedFPPixel(const DetachedFPPixel& other)
            : m_components(other.m_components)
        {}
        ValueType& operator[](int idx) { return m_components[idx]; }
        const ValueType& operator[](int idx) const { return m_components[idx]; }
        int32_t numComponents() const { return m_components.size(); }
        ValueType sum() const;
        void scale(const std::vector<ValueType> multipliers);

    };

    DetachedFPPixel operator*(double scale, const DetachedFPPixel& dpx);
    DetachedFPPixel& operator+=(DetachedFPPixel& lhs,
                                const DetachedFPPixel& rhs);
} // namespace CIL
#endif