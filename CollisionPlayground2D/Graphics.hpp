#pragma once
#ifndef INCLUDE_GRAPHICS_HPP
#define INCLUDE_GRAPHICS_HPP

#include <SDL.h>
#include <vector>
#include <string>

#include "units.hpp"
#include "Geometry/units.hpp"

class Graphics {
public:
	Graphics();
	~Graphics();

	// Initialize the Window, Renderer. Returns true on success, false if there was an error.
	bool init(game::Pixel screenWidth, game::Pixel screenHeight);

	void setRenderColour(Uint8 r, Uint8 g, Uint8 b, Uint8 a=255) const;
	void renderRect(const SDL_Rect& rect, Uint8 thickness=1) const;
	void renderLine(const SDL_Point& start, const SDL_Point& end, Uint8 thickness=1) const;
	void renderRay(const SDL_Point& origin, const geom::Coord2& dir) const;
	void renderPoly(std::vector<SDL_Point>& points) const;
	void renderPoint(const SDL_Point& point, Uint8 pointSize=1) const;
	void renderCircle(const SDL_Point& center, Uint8 radius) const;

	void setWindowTitle(const std::string& text);

	void clear();
	void present();

private:
	SDL_Window* window_;
	SDL_Renderer* renderer_;
};


#endif // INCLUDE_GRAPHICS_HPP
