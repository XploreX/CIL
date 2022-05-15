#ifndef CIL_OBJECT_HPP
#define CIL_OBJECT_HPP

namespace CIL {
    // forward declarations
    class Ray;
    class ColorMap;
    class HitInfo;

    class Object
    {
      public:
        virtual bool hit(const Ray& ray, double dist_begin, double dist_end,
                         HitInfo& hit_info) = 0;
        virtual ~Object() = default;
    };
} // namespace CIL

#endif