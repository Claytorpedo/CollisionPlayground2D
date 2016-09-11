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
	window_ = SDL_CreateWindow("Bug Maze!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, int(constants::SCREEN_WIDTH), int(constants::SCREEN_HEIGHT), SDL_WINDOW_SHOWN);
	if (!window_) {
		std::cerr << "Error: The window could not be created.\nSDL Error: " << SDL_GetError() << "\n";
		return false;
	}
	renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer_) {
		std::cerr << "Error: The renderer could not be created.\nSDL Error: " << SDL_GetError() << "\n";
		return false;
	}
	return true;
}
void Graphics::renderRect(SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint8 thickness) const {
	SDL_SetRenderDrawColor(renderer_, r, g, b, a);
	SDL_RenderDrawRect(renderer_, &rect);

	if (thickness < 2)
		return;
	for (Uint8 i = 1; i < thickness; ++i) {
		SDL_Rect r = { rect.x+i, rect.y+i, rect.w-i*2, rect.h-i*2 };
		SDL_RenderDrawRect(renderer_, &r);
	}
}

void Graphics::renderLine(SDL_Point& start, SDL_Point& end, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Uint8 thickness) const {
	SDL_SetRenderDrawColor(renderer_, r, g, b, a);
	SDL_RenderDrawLine(renderer_, start.x, start.y, end.x, end.y);

	if (thickness < 2)
		return;
	for (Uint8 i = 1; i < thickness; ++i) {
		SDL_RenderDrawLine(renderer_, start.x+i, start.y, end.x+i, end.y);
		SDL_RenderDrawLine(renderer_, start.x, start.y+i, end.x, end.y+i);
	}
}
void Graphics::renderPoly(std::vector<SDL_Point>& points, Uint8 r, Uint8 g, Uint8 b, Uint8 a) const {
	SDL_SetRenderDrawColor(renderer_, r, g, b, a);
	points.push_back(points[0]); // Duplicate the last point to close the shape.
	SDL_RenderDrawLines(renderer_, points.data(), points.size());
}

void Graphics::clear() {
	SDL_SetRenderDrawColor(renderer_, 0,0,0, 255); // Black.
	SDL_RenderClear(renderer_);
}
void Graphics::present() {
	SDL_RenderPresent(renderer_);
}