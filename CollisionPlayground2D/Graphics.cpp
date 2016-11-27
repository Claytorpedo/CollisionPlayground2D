#include <SDL.h>
#include <string>
#include <map>
#include <iostream>

#include "Geometry/Rectangle.h"
#include "Constants.h"
#include "Graphics.h"

Graphics::Graphics() : window_(nullptr), renderer_(nullptr) {};
Graphics::~Graphics() {
	// Free renderer and window.
	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);
}

bool Graphics::init() {
	if ( !SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1") ) { // Set to linear filtering.
		std::cout << "Warning: Linear filtering could not be enabled.\n";
	}
	window_ = SDL_CreateWindow("Collision Playground 2D", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, int(constants::SCREEN_WIDTH), int(constants::SCREEN_HEIGHT), SDL_WINDOW_SHOWN);
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
	SDL_RenderDrawRect(renderer_, &rect);

	if (thickness < 2)
		return;
	for (Uint8 i = 1; i < thickness; ++i) {
		SDL_Rect r = { rect.x+i, rect.y+i, rect.w-i*2, rect.h-i*2 };
		SDL_RenderDrawRect(renderer_, &r);
	}
}

void Graphics::renderLine(const SDL_Point& start, const SDL_Point& end, Uint8 thickness) const {
	SDL_RenderDrawLine(renderer_, start.x, start.y, end.x, end.y);

	if (thickness < 2)
		return;
	for (Uint8 i = 1; i < thickness; ++i) {
		SDL_RenderDrawLine(renderer_, start.x+i, start.y, end.x+i, end.y);
		SDL_RenderDrawLine(renderer_, start.x, start.y+i, end.x, end.y+i);
	}
}
void Graphics::renderRay(const SDL_Point& origin, const units::Coordinate2D& dir) const {
	std::vector<SDL_Point> points;
	points.reserve(500);
	// Just draw an arbitrary length.
	for (int i = 0; i < 1500; i+=3) {
		SDL_Point p = {static_cast<int>(origin.x + dir.x*i), static_cast<int>(origin.y + dir.y*i)};
		points.push_back(p);
	}
	SDL_RenderDrawPoints(renderer_, points.data(), points.size());
}
void Graphics::renderPoly(std::vector<SDL_Point>& points) const {
	points.push_back(points[0]); // Duplicate the first vertex to close the shape.
	SDL_RenderDrawLines(renderer_, points.data(), points.size());
}
void Graphics::renderPoint(const SDL_Point& point, Uint8 pointSize) const {
	SDL_RenderDrawPoint(renderer_, point.x, point.y);

	if (pointSize < 2)
		return;
	int s = static_cast<int>(pointSize);
	int s2 = s*s;
	for (int i = -s; i < s; ++i) {
		for (int j = -s; j < s; ++j) {
			if (i == 0 && j == 0)
				continue; // Already drew the center;
			if (i*i + j*j > s2)
				continue; // Not inside the circle.
			SDL_RenderDrawPoint(renderer_, point.x+i, point.y+j);
		}
	}
}
void Graphics::renderCircle(const SDL_Point& center, Uint8 radius) const {
	int r = static_cast<int>(radius);
	int r2 = r*r;
	for (int i = -r; i < r; ++i) {
		for (int j = -r; j < r; ++j) {
			if (i*i + j*j > r2)
				continue; // Not inside the circle.
			SDL_RenderDrawPoint(renderer_, center.x+i, center.y+j);
		}
	}
}

void Graphics::clear() {
	SDL_SetRenderDrawColor(renderer_, 0,0,0, 255); // Black.
	SDL_RenderClear(renderer_);
}
void Graphics::present() {
	SDL_RenderPresent(renderer_);
}