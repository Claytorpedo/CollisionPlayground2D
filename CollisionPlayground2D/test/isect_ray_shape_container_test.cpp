#include "catch.hpp"
#include "definitions.hpp"

#include "../Geometry/isect_ray_shape_container.hpp"
#include "../Geometry/units.hpp"
#include "../Geometry/Ray.hpp"
#include "../Geometry/ShapeContainer.hpp"
#include "../Geometry/Rectangle.hpp"
#include "../Geometry/Polygon.hpp"
#include "../Geometry/Circle.hpp"

using geom::Ray;
using geom::ShapeContainer;
using geom::Rect;
using geom::Polygon;
using geom::Circle;
using geom::Coord2;

TEST_CASE("Ray and shape intersections.", "[isect][ray][shape]") {
	SECTION("The shape is behind the ray.") {
		Ray r(Coord2(0, 0), Coord2(1, 0));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Rect(-1.1f, 0, 1, 1))));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Rect(0, 0, 1, 1)), Coord2(-1.1f, 1)));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(-2.1f, 0)));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Circle(1)), Coord2(-1.1f, 0)));
		r = Ray(Coord2(1, 1), Coord2(1, 1).normalize());
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Rect(0, 0, 0.9f, 0.9f))));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Rect(0, 0, 0.9f, 0.9f)), Coord2(0.1f, -0.1f)));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(-0.6f, -0.6f)));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Circle(1)), Coord2(-0.1f, 0)));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Circle(1)), Coord2(0, -0.1f)));
	}
	SECTION("The shape is beside the ray.") {
		Ray r(Coord2(0, 0), Coord2(1, 0));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Rect(0, 1.1f, 1, 1))));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Polygon::translate(Polygon(shapes::octagon), Coord2(10, -2.1f)))));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Circle(10, -1.1f, 1))));
	}
	SECTION("The ray's origin is inside or touches the shape.") {
		Ray r(Coord2(0, 0), Coord2(1, 0));
		CHECK(geom::intersects(r, ShapeContainer(Rect(-1, 0, 1, 1))));
		CHECK(geom::intersects(r, ShapeContainer(Rect(0, 0, 1, 1)), Coord2(-0.5f, -0.5f)));
		CHECK(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(-2, 0)));
		CHECK(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon))));
		CHECK(geom::intersects(r, ShapeContainer(Circle(1)), Coord2(-1, 0)));
		CHECK(geom::intersects(r, ShapeContainer(Circle(1))));
		r = Ray(Coord2(1, 1), Coord2(1, 1).normalize());
		CHECK(geom::intersects(r, ShapeContainer(Rect(0, 0, 1, 1))));
		CHECK(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon))));
		CHECK(geom::intersects(r, ShapeContainer(Circle(1)), Coord2(1, 1)));
		CHECK(geom::intersects(r, ShapeContainer(Circle(static_cast<geom::gFloat>(std::sqrt(2)) + geom::constants::EPSILON))));
	}
	SECTION("The shape is in front of the ray.") {
		Ray r(Coord2(0, 0), Coord2(1, 0));
		CHECK(geom::intersects(r, ShapeContainer(Rect(0, 0, 1, 1)), Coord2(10, 0)));
		CHECK(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(10, -2)));
		CHECK(geom::intersects(r, ShapeContainer(Circle(0, 0, 1)), Coord2(10, -0.9f)));
	}
}
TEST_CASE("Ray and shape intersections, with output distance to intersection point.", "[isect][ray][shape]") {
	geom::gFloat out_t;
	SECTION("The shape is behind the ray.") {
		Ray r(Coord2(0, 0), Coord2(1, 0));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Rect(0, 0, 1, 1)), Coord2(-1.1f, 0), out_t));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(-0.1f, 2), out_t));
		r = Ray(Coord2(1, 1), Coord2(1, 1).normalize());
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Rect(0, 0, 0.9f, 0.9f)), Coord2(0, 0), out_t));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(-0.1f, -2), out_t));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Circle(1)), Coord2(0, -0.1f), out_t));
	}
	SECTION("The shape is beside the ray.") {
		Ray r(Coord2(0, 0), Coord2(1, 0));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Rect(0, 0, 1, 1)), Coord2(0, 1.1f), out_t));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Rect(0, 0, 1, 1)), Coord2(10, 0.1f), out_t));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(10, -2.1f), out_t));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Circle(1)), Coord2(-0.71f, 0.71f), out_t));
		r = Ray(Coord2(1, 1), Coord2(1, 1).normalize());
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Rect(0, 0, 1, 1)), Coord2(1, 2.1f), out_t));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(5.1f, 2), out_t));
	}
	SECTION("The ray's origin is inside or touches the shape.") {
		Ray r(Coord2(0, 0), Coord2(1, 0));
		CHECK(geom::intersects(r, ShapeContainer(Rect(0, 0, 1, 1)), Coord2(-1, 0), out_t));
		CHECK(out_t == ApproxEps(0));
		CHECK(geom::intersects(r, ShapeContainer(Rect(0, 0, 1, 1)), Coord2(0, 0), out_t));
		CHECK(out_t == ApproxEps(0));
		CHECK(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(0, 2), out_t));
		CHECK(out_t == ApproxEps(0));
		CHECK(geom::intersects(r, ShapeContainer(Circle(1)), Coord2(-0.7f, 0.71f), out_t));
		CHECK(out_t == ApproxEps(0));
		r = Ray(Coord2(1, 1), Coord2(1, 1).normalize());
		CHECK(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(-0.5f, -0.5f), out_t));
		CHECK(out_t == ApproxEps(0));
		CHECK(geom::intersects(r, ShapeContainer(Circle(static_cast<geom::gFloat>(std::sqrt(2)) + geom::constants::EPSILON)), Coord2(0, 0), out_t));
		CHECK(out_t == ApproxEps(0));
	}
	SECTION("The shape is in front of the ray.") {
		Ray r(Coord2(0, 0), Coord2(1, 0));
		CHECK(geom::intersects(r, ShapeContainer(Rect(0, 0, 1, 1)), Coord2(0.1f, 0), out_t));
		CHECK(out_t == ApproxEps(0.1f));
		CHECK(geom::intersects(r, ShapeContainer(Rect(0, 0, 1, 1)), Coord2(10, 0), out_t));
		CHECK(out_t == ApproxEps(10));
		CHECK(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(10, -2), out_t));
		CHECK(out_t == ApproxEps(10));
		CHECK(geom::intersects(r, ShapeContainer(Circle(1)), Coord2(10, -0.9f), out_t));
		CHECK(out_t == ApproxEps(10 - std::sqrt(19)*0.1f));
		r = Ray(Coord2(1, 1), Coord2(1, 1).normalize());
		CHECK(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(10, 12), out_t));
		CHECK(out_t == ApproxEps(std::sqrt(162)));
		CHECK(geom::intersects(r, ShapeContainer(Circle(1)), Coord2(8.6f, 10), out_t));
		CHECK(out_t == ApproxEps(std::sqrt(8.2f*8.2f*2.0f)));
	}
}
TEST_CASE("Ray and shape intersections, with output distance and normal to intersection point.", "[isect][ray][shape]") {
	geom::gFloat out_t;
	Coord2 out_norm;
	SECTION("The shape is behind the ray.") {
		Ray r(Coord2(0, 0), Coord2(1, 0));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Rect(0, 0, 1, 1)), Coord2(-1.1f, 0), out_t, out_norm));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(-0.1f, 2), out_t, out_norm));
		r = Ray(Coord2(1, 1), Coord2(1, 1).normalize());
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Rect(0, 0, 0.9f, 0.9f)), Coord2(0, 0), out_t, out_norm));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(-0.1f, -2), out_t, out_norm));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Circle(1)), Coord2(0, -0.1f), out_t, out_norm));
	}
	SECTION("The shape is beside the ray.") {
		Ray r(Coord2(0, 0), Coord2(1, 0));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Rect(0, 0, 1, 1)), Coord2(0, 1.1f), out_t, out_norm));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(10, 2.1f), out_t, out_norm));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Circle(1)), Coord2(-0.71f, 0.71f), out_t, out_norm));
		r = Ray(Coord2(1, 1), Coord2(1, 1).normalize());
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Rect(0, 0, 1, 1)), Coord2(1, 2.1f), out_t, out_norm));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(5.1f, 2), out_t, out_norm));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Circle(1)), Coord2(10.21f, 8.79f), out_t, out_norm));
	}
	SECTION("The ray's origin is inside or touches the shape.") {
		Ray r(Coord2(0, 0), Coord2(1, 0));
		CHECK(geom::intersects(r, ShapeContainer(Rect(0, 0, 1, 1)), Coord2(0, 0), out_t, out_norm));
		CHECK(out_t == ApproxEps(0));
		CHECK(out_norm.x == ApproxEps(0));
		CHECK(out_norm.y == ApproxEps(0));
		CHECK(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(1.5f, 1), out_t, out_norm));
		CHECK(out_t == ApproxEps(0));
		CHECK(out_norm.x == ApproxEps(0));
		CHECK(out_norm.y == ApproxEps(0));
		CHECK(geom::intersects(r, ShapeContainer(Circle(1)), Coord2(0, 0), out_t, out_norm));
		CHECK(out_t == ApproxEps(0));
		CHECK(out_norm.x == ApproxEps(0));
		CHECK(out_norm.y == ApproxEps(0));
		r = Ray(Coord2(1, 1), Coord2(1, 1).normalize());
		CHECK(geom::intersects(r, ShapeContainer(Rect(0, 0, 1, 1)), Coord2(1, 1), out_t, out_norm));
		CHECK(out_t == ApproxEps(0));
		CHECK(out_norm.x == ApproxEps(0));
		CHECK(out_norm.y == ApproxEps(0));
		CHECK(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(-0.5f, -0.5f), out_t, out_norm));
		CHECK(out_t == ApproxEps(0));
		CHECK(out_norm.x == ApproxEps(0));
		CHECK(out_norm.y == ApproxEps(0));
	}
	SECTION("The shape is in front of the ray.") {
		Ray r(Coord2(0, 0), Coord2(1, 0));
		CHECK(geom::intersects(r,ShapeContainer(Rect(0, 0, 1, 1)), Coord2(0.1f, 0), out_t, out_norm));
		CHECK(out_t == ApproxEps(0.1f));
		CHECK(out_norm.x == ApproxEps(-1));
		CHECK(out_norm.y == ApproxEps(0));
		CHECK(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(10, 2), out_t, out_norm));
		Coord2 expected_norm = Coord2(-1, -3).normalize();
		CHECK(out_t == ApproxEps(10));
		CHECK(out_norm.x == ApproxEps(expected_norm.x));
		CHECK(out_norm.y == ApproxEps(expected_norm.y));
		CHECK(geom::intersects(r, ShapeContainer(Circle(1)), Coord2(10, 0.9f), out_t, out_norm));
		CHECK(out_t == ApproxEps(10 - std::sqrt(19)*0.1f));
		expected_norm = (r.origin + r.dir*(10 - static_cast<geom::gFloat>(std::sqrt(19))*0.1f) - Coord2(10, 0.9f)).normalize();
		CHECK(out_norm.x == ApproxEps(expected_norm.x));
		CHECK(out_norm.y == ApproxEps(expected_norm.y));
		r = Ray(Coord2(1, 1), Coord2(1, 1).normalize());
		CHECK(geom::intersects(r, ShapeContainer(Rect(0, 0, 1, 1)), Coord2(1.9f, 2), out_t, out_norm));
		CHECK(out_t == ApproxEps(std::sqrt(2)));
		CHECK(out_norm.x == ApproxEps(0));
		CHECK(out_norm.y == ApproxEps(-1));
		CHECK(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(10, 11.96f), out_t, out_norm));
		expected_norm = Coord2(-1, -3).normalize();
		CHECK(out_t == ApproxEps(std::sqrt(8.97f*8.97f*2.0f)));
		CHECK(out_norm.x == ApproxEps(expected_norm.x));
		CHECK(out_norm.y == ApproxEps(expected_norm.y));
	}
}
TEST_CASE("Ray and shape intersections, with output distance to both intersection points.", "[isect][ray][shape]") {
	geom::gFloat out_enter, out_exit;
	SECTION("The shape is behind the ray.") {
		Ray r(Coord2(0, 0), Coord2(1, 0));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Rect(0, 0, 1, 1)), Coord2(-1.1f, 0), out_enter, out_exit));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(-0.1f, 2), out_enter, out_exit));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Circle(1)), Coord2(-1.1f, 0), out_enter, out_exit));
		r = Ray(Coord2(1, 1), Coord2(1, 1).normalize());
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Rect(0, 0, 0.9f, 0.9f)), Coord2(0, 0), out_enter, out_exit));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(-0.1f, -2), out_enter, out_exit));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Circle(1)), Coord2(0, -0.1f), out_enter, out_exit));
	}
	SECTION("The shape is beside the ray.") {
		Ray r(Coord2(0, 0), Coord2(1, 0));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Rect(0, 0, 1, 1)), Coord2(0, 1.1f), out_enter, out_exit));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(10, 2.1f), out_enter, out_exit));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Circle(1)), Coord2(-0.71f, 0.71f), out_enter, out_exit));
		r = Ray(Coord2(1, 1), Coord2(1, 1).normalize());
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Rect(0, 0, 1, 1)), Coord2(1, 2.1f), out_enter, out_exit));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(5.1f, 2), out_enter, out_exit));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Circle(1)), Coord2(9.79f, 11.21f), out_enter, out_exit));
	}
	SECTION("The ray's origin is inside or touches the shape.") {
		Ray r(Coord2(0, 0), Coord2(1, 0));
		CHECK(geom::intersects(r, ShapeContainer(Rect(0, 0, 1, 1)), Coord2(-1, 0), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_exit == ApproxEps(0));
		CHECK(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(1.5f, 1), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_exit == ApproxEps(2.5f + 2 / 3.0f));
		r = Ray(Coord2(1, 1), Coord2(1, 1).normalize());
		CHECK(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(-0.5f, -0.5f), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_exit == ApproxEps(0));
		CHECK(geom::intersects(r, ShapeContainer(Circle(static_cast<geom::gFloat>(std::sqrt(2)) + geom::constants::EPSILON)), Coord2(0, 0), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_exit == ApproxEps(geom::constants::EPSILON));
	}
	SECTION("The shape is in front of the ray.") {
		Ray r(Coord2(0, 0), Coord2(1, 0));
		CHECK(geom::intersects(r, ShapeContainer(Rect(0, 0, 1, 1)), Coord2(1, 0), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(1));
		CHECK(out_exit == ApproxEps(2));
		CHECK(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(10, -2), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(10));
		CHECK(out_exit == ApproxEps(10));
		CHECK(geom::intersects(r, ShapeContainer(Circle(1)), Coord2(10, -0.9f), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(10 - std::sqrt(19)*0.1f));
		CHECK(out_exit == ApproxEps(10 + std::sqrt(19)*0.1f));
		r = Ray(Coord2(1, 1), Coord2(1, 1).normalize());
		CHECK(geom::intersects(r, ShapeContainer(Rect(0, 0, 1, 1)), Coord2(1, 1), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_exit == ApproxEps(std::sqrt(2)));
		CHECK(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(10, 12), out_enter, out_exit));
		CHECK(out_enter == ApproxEps(std::sqrt(162)));
		CHECK(out_exit == ApproxEps(std::sqrt(121 * 2.0f)));
	}
}
TEST_CASE("Ray and shape intersections, with output distance and normals to both intersection points.", "[isect][ray][shape]") {
	geom::gFloat out_enter, out_exit;
	Coord2 out_norm_enter, out_norm_exit;
	SECTION("The shape is behind the ray.") {
		Ray r(Coord2(0, 0), Coord2(1, 0));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Rect(0, 0, 1, 1)), Coord2(-1.1f, 0), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(-0.1f, 2), out_enter, out_norm_enter, out_exit, out_norm_exit));
		r = Ray(Coord2(1, 1), Coord2(1, 1).normalize());
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Rect(0, 0, 0.9f, 0.9f)), Coord2(0, 0), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(-0.1f, -2), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Circle(1)), Coord2(-0.1f, 0), out_enter, out_norm_enter, out_exit, out_norm_exit));
	}
	SECTION("The shape is beside the ray.") {
		Ray r(Coord2(0, 0), Coord2(1, 0));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Rect(0, 0, 1, 1)), Coord2(0, 1.1f), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(10, 2.1f), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Circle(1)), Coord2(-0.71f, 0.71f), out_enter, out_norm_enter, out_exit, out_norm_exit));
		r = Ray(Coord2(1, 1), Coord2(1, 1).normalize());
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Rect(0, 0, 1, 1)), Coord2(1, 2.1f), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK_FALSE(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(5.1f, 2), out_enter, out_norm_enter, out_exit, out_norm_exit));
	}
	SECTION("The ray's origin is inside or touches the shape.") {
		Coord2 expected_norm_enter, expected_norm_exit;
		Ray r(Coord2(0, 0), Coord2(1, 0));
		CHECK(geom::intersects(r, ShapeContainer(Rect(0, 0, 1, 1)), Coord2(0, 0), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_norm_enter.x == ApproxEps(0));
		CHECK(out_norm_enter.y == ApproxEps(0));
		CHECK(out_exit == ApproxEps(1));
		CHECK(out_norm_exit.x == ApproxEps(1));
		CHECK(out_norm_exit.y == ApproxEps(0));
		CHECK(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(1.5f, 1), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_exit == ApproxEps(2.5f + 2 / 3.0f));
		expected_norm_enter = Coord2(0, 0);
		expected_norm_exit = Coord2(3, -1).normalize();
		CHECK(out_norm_enter.x == ApproxEps(expected_norm_enter.x));
		CHECK(out_norm_enter.y == ApproxEps(expected_norm_enter.y));
		CHECK(out_norm_exit.x == ApproxEps(expected_norm_exit.x));
		CHECK(out_norm_exit.y == ApproxEps(expected_norm_exit.y));
		CHECK(geom::intersects(r, ShapeContainer(Circle(1)), Coord2(0, 0), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_norm_enter.x == ApproxEps(0));
		CHECK(out_norm_enter.y == ApproxEps(0));
		CHECK(out_exit == ApproxEps(1));
		CHECK(out_norm_exit.x == ApproxEps(1));
		CHECK(out_norm_exit.y == ApproxEps(0));
		r = Ray(Coord2(1, 1), Coord2(1, 1).normalize());
		CHECK(geom::intersects(r, ShapeContainer(Rect(0, 0, 1, 1)), Coord2(1, 1), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK(out_enter == ApproxEps(0));
		CHECK(out_norm_enter.x == ApproxEps(0));
		CHECK(out_norm_enter.y == ApproxEps(0));
		CHECK(out_exit == ApproxEps(std::sqrt(2)));
		CHECK(out_norm_exit.x == ApproxEps(1));
		CHECK(out_norm_exit.y == ApproxEps(0));
	}
	SECTION("The shape is in front of the ray.") {
		Coord2 expected_norm_enter, expected_norm_exit;
		Ray r(Coord2(0, 0), Coord2(1, 0));
		CHECK(geom::intersects(r, ShapeContainer(Rect(0, 0, 1, 1)), Coord2(10, -0.9f), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK(out_enter == ApproxEps(10));
		CHECK(out_norm_enter.x == ApproxEps(-1));
		CHECK(out_norm_enter.y == ApproxEps(0));
		CHECK(out_exit == ApproxEps(11));
		CHECK(out_norm_exit.x == ApproxEps(1));
		CHECK(out_norm_exit.y == ApproxEps(0));
		r = Ray(Coord2(1, 1), Coord2(1, 1).normalize());
		CHECK(geom::intersects(r, ShapeContainer(Polygon(shapes::octagon)), Coord2(10, 12.1f), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK(out_enter == ApproxEps(std::sqrt(9.15f*9.15f*2.0f)));
		CHECK(out_exit == ApproxEps(std::sqrt(10.95f*10.95f*2.0f)));
		expected_norm_enter = Coord2(1, -3).normalize();
		expected_norm_exit = Coord2(3, -1).normalize();
		CHECK(out_norm_enter.x == ApproxEps(expected_norm_enter.x));
		CHECK(out_norm_enter.y == ApproxEps(expected_norm_enter.y));
		CHECK(out_norm_exit.x == ApproxEps(expected_norm_exit.x));
		CHECK(out_norm_exit.y == ApproxEps(expected_norm_exit.y));
		CHECK(geom::intersects(r, ShapeContainer(Circle(1)), Coord2(2, 2), out_enter, out_norm_enter, out_exit, out_norm_exit));
		CHECK(out_enter == ApproxEps(std::sqrt(2) - 1));
		expected_norm_enter = (r.origin + r.dir*(static_cast<geom::gFloat>(std::sqrt(2)) - 1) - Coord2(2, 2)).normalize();
		expected_norm_exit = (r.origin + r.dir*(static_cast<geom::gFloat>(std::sqrt(2)) + 1) - Coord2(2, 2)).normalize();
		CHECK(out_norm_enter.x == ApproxEps(expected_norm_enter.x));
		CHECK(out_norm_enter.y == ApproxEps(expected_norm_enter.y));
		CHECK(out_exit == ApproxEps(std::sqrt(2) + 1));
		CHECK(out_norm_exit.x == ApproxEps(expected_norm_exit.x));
		CHECK(out_norm_exit.y == ApproxEps(expected_norm_exit.y));
	}
}
