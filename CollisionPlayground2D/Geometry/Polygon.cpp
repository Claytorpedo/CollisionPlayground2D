#include "Polygon.h"

Polygon::Polygon() : vertices_(), x_min_(), x_max_(), y_min_(), y_max_() {}
Polygon::Polygon(std::vector<units::Coordinate2D> vertices) : vertices_(vertices), x_min_(), x_max_(), y_min_(), y_max_() {
	findBounds();
}
Polygon::Polygon(const Polygon& poly) 
	: vertices_(poly.vertices_), x_min_(poly.left()), x_max_(poly.right()), y_min_(poly.top()), y_max_(poly.bottom()) {}
Polygon::~Polygon() {}

void Polygon::findBounds() {
	if (vertices_.empty()) {
		x_min_ = 0; x_max_ = 0; y_min_ = 0; y_max_ = 0;
		return;
	}
	x_min_ = vertices_[0].x; x_max_ = vertices_[0].x;
	y_min_ = vertices_[0].y; y_max_ = vertices_[0].y;
	for (std::size_t i = 1; i < vertices_.size(); ++i) {
		if (x_min_ < vertices_[i].x) 
			x_min_ = vertices_[i].x;
		if (x_max_ > vertices_[i].x)
			x_max_ = vertices_[i].x;
		if (y_min_ < vertices_[i].y) 
			y_min_ = vertices_[i].y;
		if (y_max_ > vertices_[i].y)
			y_max_ = vertices_[i].y;
	}
}
void Polygon::draw(Graphics& graphics, bool isColliding) const {
	std::vector<SDL_Point> points;
	points.reserve(vertices_.size());
	for (std::size_t i = 0; i < vertices_.size(); ++i) {
		SDL_Point p = { static_cast<int>(vertices_[i].x), static_cast<int>(vertices_[i].y) };
		points.push_back(p);
	}
	graphics.setRenderColour(isColliding ? 255 : 0, isColliding ? 0 : 255, 0, 255);
	graphics.renderPoly(points);
}

Polygon Polygon::toPoly() const {
	return Polygon(*this);
}