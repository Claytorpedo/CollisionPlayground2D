#pragma once
#ifndef INCLUDE_GEOM_POLYGON_HPP
#define INCLUDE_GEOM_POLYGON_HPP

#include "Shape.hpp"

#include <vector>
#include <random>

// Convex polygon with counterclockwise winding.
namespace geom {
	class Rect;
	struct Projection;

	class Polygon : public Shape {
	public:
		// Used when determining if two vectors are perpendicular.
		// We don't normalize the vectors, so scale is a factor.
		static const gFloat EPSILON_DEGREE_TOLERANCE;

		Polygon();
		// Vertices should be in counterclockwise order.
		// The final vertex connects with the first vertex.
		Polygon(std::vector<Coord2> vertices);
		Polygon(std::vector<Coord2> vertices, std::vector<Coord2> edgeNormals);
		virtual ~Polygon() = default;

		// Randomly generate a polygon.
		// region is a bounding box defining the region to place the polygon's center in (part of the polygon can be outside this region).
		// minRad and maxRad control how large the generated polygon will be.
		// minVerts and maxVerts control how many vertices the generated polygon can have.
		static Polygon generate(std::mt19937& rando, const Rect& region, const gFloat minRad = 0.1f, const gFloat maxRad = 100.0f,
			const std::size_t minVerts = 3, const std::size_t maxVerts = 20);

		// Rather than recompute the bounds every call, the bounds
		// are computed when the polygon is constructed or changed,
		// and then cached to be returned here.
		virtual gFloat left()   const { return x_min_; }
		virtual gFloat right()  const { return x_max_; }
		virtual gFloat top()    const { return y_min_; }
		virtual gFloat bottom() const { return y_max_; }

		virtual Projection getProjection(const Coord2& axis) const;

		// Returns a copy of itself.
		virtual Polygon toPoly() const;

		// Extend a polygon by projecting it along a direction by dist.
		virtual Polygon extend(const Coord2& dir, const gFloat dist) const;

		// Find the closest vertex to the given point.
		// Note: this doesn't disqualify the edge case were the closest vertex could be on the "far" side of the polygon.
		virtual Coord2 getClosestTo(const Coord2& point) const;

		// Get normalized counter-clockwise edge normal for the polygon at a given index.
		// Edges are indexed by vertex order, e.g. edge 0 is made from vertex 0 and 1.
		const Coord2& getEdgeNorm(std::size_t index) const;
		// Compute all normals for the polygon.
		void computeNormals() const;

		// Increase the size of a polygon by scaleAmount in all directions (by vertex normals).
		void expand(const gFloat expandAmount);
		// Increase the size of a polygon by scaleAmount in all directions (by vertex normals).
		static Polygon expand(const Polygon& p, const gFloat expandAmount);

		// Find the first and last vertices in the range to extend from in the direction of the given direction vector.
		// Also finds whether the first and last vertices need to be duplicated when extending.
		// Note that first and last follow the winding of the polygon, and thus can overlap between the end and
		// start of the Polygon's vertex list (resulting in first > last).
		// Returns false if the range could not be found, indicating an invalid polygon.
		bool findExtendRange(const Coord2& dir, std::size_t& out_first, std::size_t& out_last,
			bool& out_should_dupe_first, bool& out_should_dupe_last) const;
		// Extend a polygon by projecting it along a direction by delta (dir*dist), clipping the result to only include
		// the portion of the polygon that was extended.
		Polygon clipExtend(const Coord2& dir, const gFloat dist) const;
		// If we've already found the range, we can clip extend with the found values.
		Polygon clipExtend(const Coord2& dir, const gFloat dist,
			const std::size_t rangeFirst, const std::size_t rangeLast) const;
		// If we've already found the range, we can extend with the found values.
		Polygon extend(const Coord2& dir, const gFloat dist,
			const std::size_t rangeFirst, const std::size_t rangeLast, const bool shouldDupeFirst, const bool shouldDupeLast) const;

		// Move the polygon by given x and y.
		void translate(const gFloat x, const gFloat y);
		// Move the polygon by delta.
		void translate(const Coord2& delta);
		// Create a polygon by translating an existing polygon by a delta vector.
		static Polygon translate(const Polygon& p, const Coord2& delta);

		// For accessing the values of the vertices of the polygon. Note no safety checks.
		inline const Coord2& operator[](std::size_t index) const { return vertices_[index]; }
		// Get the number of vertices in the polygon.
		inline std::size_t size() const { return vertices_.size(); }
		// Check if the polygon has any vertices.
		inline bool isEmpty() const { return vertices_.empty(); }

	private:
		std::vector<Coord2> vertices_;
		mutable std::vector<Coord2> edge_normals_;
		gFloat x_min_, x_max_, y_min_, y_max_;

		// Find the bounding box for the polygon and cache the values.
		void _find_bounds();
	};
}
#endif // INCLUDE_GEOM_POLYGON_HPP
