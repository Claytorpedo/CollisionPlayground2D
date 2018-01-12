#pragma once
#ifndef INCLUDE_GRAPHICS_HPP
#define INCLUDE_GRAPHICS_HPP

#include <SDL.h>
#include <vector>
#include <string>

#include "units.hpp"

#include "Geometry2D/Geometry.hpp"

class Graphics {
public:
	Graphics();
	~Graphics();

	// Initialize the Window, Renderer. Returns true on success, false if there was an error.
	bool init(game::Pixel screenWidth, game::Pixel screenHeight);

	void setRenderColour(Uint8 r, Uint8 g, Uint8 b, Uint8 a=255) const;
	void renderRect(const SDL_Rect& rect, Uint8 thickness=1) const;
	void renderLine(const SDL_Point& start, const SDL_Point& end, Uint8 thickness=1) const;
	void renderLines(const std::vector<SDL_Point>& points, Uint8 thickness=1) const;
	void renderRay(const SDL_Point& origin, float dirx, float diry, Uint16 length=1000, Uint8 thickness=1) const;
	void renderPoly(const std::vector<SDL_Point>& points, Uint8 thickness=1) const;
	void renderPoint(const SDL_Point& point, Uint8 pointSize=1) const;
	void renderPoints(const std::vector<SDL_Point>& points, Uint8 pointSize=1) const;
	void renderCircle(const SDL_Point& center, Uint16 radius, Uint8 thickness=1) const;

	void renderRect(const geom::Rect& r, Uint8 thickness=1) const;
	void renderPoly(const geom::Polygon& p, Uint8 thickness=1) const;
	void renderPolyVerts(const geom::Polygon& p, Uint8 pointSize=1) const;
	void renderPolyEdgeNormals(const geom::Polygon& p, Uint16 length=50, Uint8 thickness=1) const;
	void renderCircle(const geom::Circle& c, Uint8 thickness=1) const;

	void setWindowTitle(const std::string& text);

	void clear();
	void present();

private:
	SDL_Window* window_;
	SDL_Renderer* renderer_;
};


#endif // INCLUDE_GRAPHICS_HPP
