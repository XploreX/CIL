#include <CIL/Drawing.hpp>
#include <CIL/RayTracing/Vector3D.hpp>
#include <catch2/catch.hpp>

TEST_CASE("drawing-1", "[drawing][colormap]")
{
    CHECK(CIL::Color::WHITE * 0 == CIL::Color::BLACK);
    // overflow should set everything as max
    CHECK(CIL::Color::WHITE * 2 == CIL::Color::WHITE);
    CHECK(CIL::Color::WHITE * 0.5 == CIL::ColorMap({127, 127, 127}));
    CHECK(CIL::Color::WHITE + CIL::Color::WHITE == CIL::Color::WHITE);
}
