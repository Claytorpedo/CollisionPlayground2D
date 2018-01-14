#include "Graphics.hpp"

#include <SDL.h>
#include <string>
#include <iostream>

#include "util.hpp"

Graphics::Graphics() : window_(nullptr), renderer_(nullptr) {}
Graphics::~Graphics() {
	// Free renderer and window.
	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);
}

bool Graphics::init(game::Pixel screenWidth, game::Pixel screenHeight) {
	if ( !SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1") ) { // Set to linear filtering.
		std::cout << "Warning: Linear filtering could not be enabled.\n";
	}
	window_ = SDL_CreateWindow("Collision Playground 2D", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
	if (!window_) {
		std::cerr << "Error: The window could not be created.\nSDL Error: " << SDL_GetError() << "\n";
		return false;
	}
	renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer_) {
		std::cerr << "Error: The renderer could not be created.\nSDL Error: " << SDL_GetError() << "\n";
		return false;
	}
	if (SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND) != 0) {
		std::cerr << "Warning: SDL blending could not be enabled.\n";
	}
	return true;
}

void Graphics::setRenderColour(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const {
	SDL_SetRenderDrawColor(renderer_, r, g, b, a);
}

void Graphics::renderRect(const SDL_Rect& rect, Uint8 thickness) const {
	std::vector<SDL_Rect> rects;
	rects.reserve(thickness + 1);
	rects.push_back(rect);
	for (Uint8 i = 1; i < thickness; ++i)
		rects.push_back(SDL_Rect{rect.x+i, rect.y+i, rect.w-i*2, rect.h-i*2 });
	SDL_RenderDrawRects(renderer_, rects.data(), rects.size());
}

void Graphics::renderLine(const SDL_Point& start, const SDL_Point& end, Uint8 thickness) const {
	SDL_RenderDrawLine(renderer_, start.x, start.y, end.x, end.y);
	for (Uint8 i = 1; i < thickness; ++i) {
		SDL_RenderDrawLine(renderer_, start.x+i, start.y, end.x+i, end.y);
		SDL_RenderDrawLine(renderer_, start.x, start.y+i, end.x, end.y+i);
	}
}
void Graphics::renderLines(const std::vector<SDL_Point>& points, Uint8 thickness) const {
	SDL_RenderDrawLines(renderer_, points.data(), points.size());
	if (thickness < 2)
		return;
	std::vector<SDL_Point> points_out = points, points_in = points;
	const std::size_t size = points.size();
	for (Uint8 i = 1; i < thickness; ++i) {
		for (std::size_t k = 0; k < size; ++k) {
			++points_out[i].x;
			++points_in[i].y;
		}
		SDL_RenderDrawLines(renderer_, points_out.data(), size);
		SDL_RenderDrawLines(renderer_, points_in.data(), size);
	}
}
void Graphics::renderRay(const SDL_Point& origin, float dirx, float diry, Uint16 length, Uint8 thickness) const {
	std::vector<SDL_Point> points;
	points.reserve(length/(2*thickness));
	for (int i = 0; i < length; i+=2*thickness)
		points.push_back(SDL_Point{ static_cast<int>(origin.x + dirx * i), static_cast<int>(origin.y + diry * i) });
	renderPoints(points, thickness);
}
void Graphics::renderPoly(const std::vector<SDL_Point>& points, Uint8 thickness) const {
	std::vector<SDL_Point> drawPoints = points;
	drawPoints.push_back(points[0]); // Duplicate the first vertex to close the shape.
	renderLines(drawPoints, thickness);
}
void Graphics::renderPoint(const SDL_Point& point, Uint8 pointSize) const {
	std::vector<SDL_Point> points;
	const int r = static_cast<int>(pointSize) - 1;
	const int r2 = r * r;
	for (int i = -r; i <= r; ++i) {
		for (int j = -r; j <= r; ++j) {
			if (i*i + j * j <= r2) // Check if inside the circle.
				points.push_back(SDL_Point{ point.x + i, point.y + j });
		}
	}
	SDL_RenderDrawPoints(renderer_, points.data(), points.size());
}
void Graphics::renderPoints(const std::vector<SDL_Point>& points, Uint8 pointSize) const {
	std::vector<SDL_Point> drawPoints;
	const int r = static_cast<int>(pointSize) - 1;
	const int r2 = r * r;
	for (std::size_t h = 0; h < points.size(); ++h) {
		for (int i = -r; i <= r; ++i) {
			for (int j = -r; j <= r; ++j) {
				if (i*i + j*j <= r2) // Check if inside the circle.
					drawPoints.push_back(SDL_Point{ points[h].x + i, points[h].y + j });
			}
		}
	}
	SDL_RenderDrawPoints(renderer_, points.data(), points.size());
}
void Graphics::renderCircle(const SDL_Point& center, Uint16 radius, Uint8 thickness) const {
	std::vector<SDL_Point> points;
	int r = static_cast<int>(radius);
	int r2 = r*r;
	int minRad = r - static_cast<int>(thickness);
	int minRad2 = minRad < 0 ? 0 : minRad * minRad;
	for (int i = -r; i <= r; ++i) {
		for (int j = -r; j <= r; ++j) {
			const int hypotenuse2(i*i + j*j);
			if (hypotenuse2 <= r2 && hypotenuse2 >= minRad2)
				points.push_back(SDL_Point{ center.x + i, center.y + j });
		}
	}
	SDL_RenderDrawPoints(renderer_, points.data(), points.size());
}

void Graphics::renderRect(const geom::Rect& r, const geom::Coord2& pos, Uint8 thickness) const {
	SDL_Rect rect = { static_cast<int>(r.x+pos.x), static_cast<int>(r.y+pos.y), static_cast<int>(r.w), static_cast<int>(r.h) };
	renderRect(rect, thickness);
}

void Graphics::renderPoly(const geom::Polygon& p, const geom::Coord2& pos, Uint8 thickness) const {
	std::vector<SDL_Point> points;
	points.reserve(p.size() + 1);
	for (std::size_t i = 0; i < p.size(); ++i)
		points.push_back(game::util::coord2DToSDLPoint(p[i] + pos));
	points.push_back(game::util::coord2DToSDLPoint(p[0] + pos)); // Close the polygon.
	renderLines(points, thickness);
}
void Graphics::renderPolyVerts(const geom::Polygon& p, const geom::Coord2& pos, Uint8 pointSize) const {
	const size_t size = p.size();
	for (std::size_t i = 0; i < size; ++i)
		renderPoint(game:: util::coord2DToSDLPoint(p[i]+pos), pointSize);
}
void Graphics::renderPolyEdgeNormals(const geom::Polygon& p, const geom::Coord2& pos, Uint16 length, Uint8 thickness) const {
	const size_t size = p.size();
	const geom::Coord2 twoPos = pos * 2.0f;
	for (std::size_t i = 0, k = size - 1; i < size; k = i++) {
		const geom::Coord2 start((p[k] + p[i] + twoPos) * 0.5f);
		const geom::Coord2 end(start + p.getEdgeNorm(k) * length);
		renderLine(game::util::coord2DToSDLPoint(start), game::util::coord2DToSDLPoint(end), thickness);
	}
}
void Graphics::renderCircle(const geom::Circle& c, const geom::Coord2& pos, Uint8 thickness) const {
	renderCircle(game::util::coord2DToSDLPoint(c.center+pos), static_cast<Uint16>(c.radius), thickness);
}
void Graphics::renderShape(const geom::ShapeContainer& s, const geom::Coord2& pos, Uint8 thickness) const {
	switch (s.type()) {
	case geom::ShapeType::RECTANGLE:
		renderRect(s.rect(), pos, thickness);
		break;
	case geom::ShapeType::POLYGON:
		renderPoly(s.poly(), pos, thickness);
		break;
	case geom::ShapeType::CIRCLE:
		renderCircle(s.circle(), pos, thickness);
		break;
	default:
		std::cerr << "Error: Unhandled shape type to renderShape.\n";
	}
}

void Graphics::setWindowTitle(const std::string& text) {
	SDL_SetWindowTitle(window_, text.data());
}

void Graphics::clear() {
	SDL_SetRenderDrawColor(renderer_, 0,0,0, 255); // Black.
	SDL_RenderClear(renderer_);
}
void Graphics::present() {
	SDL_RenderPresent(renderer_);
}
