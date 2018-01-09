#include "isect_ray_shape_container.hpp"

#include "isect_ray_rect.hpp"
#include "isect_ray_poly.hpp"
#include "isect_ray_circle.hpp"

#include "units.hpp"
#include "debug_logger.hpp"
#include "Ray.hpp"
#include "ShapeContainer.hpp"

namespace geom {
	bool intersects(const Ray& r, const ShapeContainer& s, const Coord2& pos) {
		switch (s.type()) {
		case ShapeType::RECTANGLE: return intersects(r, s.rect(), pos);
		case ShapeType::POLYGON:   return intersects(r, s.poly(), pos);
		case ShapeType::CIRCLE:    return intersects(r, s.circle(), pos);
		default:
			DBG_WARN("Unhandled shape type for ray-shape intersection. Converting to polygon.");
			return intersects(r, s.shape().toPoly(), pos);
		}
	}
	bool intersects(const Ray& r, const ShapeContainer& s, const Coord2& pos, gFloat& out_t) {
		switch (s.type()) {
		case ShapeType::RECTANGLE: return intersects(r, s.rect(), pos, out_t);
		case ShapeType::POLYGON:   return intersects(r, s.poly(), pos, out_t);
		case ShapeType::CIRCLE:    return intersects(r, s.circle(), pos, out_t);
		default:
			DBG_WARN("Unhandled shape type for ray-shape intersection. Converting to polygon.");
			return intersects(r, s.shape().toPoly(), pos, out_t);
		}
	}
	bool intersects(const Ray& r, const ShapeContainer& s, const Coord2& pos, gFloat& out_t, Coord2& out_norm) {
		switch (s.type()) {
		case ShapeType::RECTANGLE: return intersects(r, s.rect(), pos, out_t, out_norm);
		case ShapeType::POLYGON:   return intersects(r, s.poly(), pos, out_t, out_norm);
		case ShapeType::CIRCLE:    return intersects(r, s.circle(), pos, out_t, out_norm);
		default:
			DBG_WARN("Unhandled shape type for ray-shape intersection. Converting to polygon.");
			return intersects(r, s.shape().toPoly(), pos, out_t, out_norm);
		}
	}
	bool intersects(const Ray& r, const ShapeContainer& s, const Coord2& pos, gFloat& out_enter, gFloat& out_exit) {
		switch (s.type()) {
		case ShapeType::RECTANGLE: return intersects(r, s.rect(), pos, out_enter, out_exit);
		case ShapeType::POLYGON:   return intersects(r, s.poly(), pos, out_enter, out_exit);
		case ShapeType::CIRCLE:    return intersects(r, s.circle(), pos, out_enter, out_exit);
		default:
			DBG_WARN("Unhandled shape type for ray-shape intersection. Converting to polygon.");
			return intersects(r, s.shape().toPoly(), pos, out_enter, out_exit);
		}
	}
	bool intersects(const Ray& r, const ShapeContainer& s, const Coord2& pos, gFloat& out_enter, Coord2& out_norm_enter, gFloat& out_exit, Coord2& out_norm_exit) {
		switch (s.type()) {
		case ShapeType::RECTANGLE: return intersects(r, s.rect(), pos, out_enter, out_norm_enter, out_exit, out_norm_exit);
		case ShapeType::POLYGON:   return intersects(r, s.poly(), pos, out_enter, out_norm_enter, out_exit, out_norm_exit);
		case ShapeType::CIRCLE:    return intersects(r, s.circle(), pos, out_enter, out_norm_enter, out_exit, out_norm_exit);
		default:
			DBG_WARN("Unhandled shape type for ray-shape intersection. Converting to polygon.");
			return intersects(r, s.shape().toPoly(), pos, out_enter, out_norm_enter, out_exit, out_norm_exit);
		}
	}
}