#ifndef INCLUDE_GRAPHICS_HPP
#define INCLUDE_GRAPHICS_HPP

#include <SDL.h>
#include <vector>
#include <string>

#include "units.hpp"
#include "Colour.hpp"

#include <Geometry2D/Geometry.hpp>

class Graphics {
public:
	static const std::string DEFAULT_WINDOW_TITLE;
	Graphics();
	~Graphics();

	// Initialize the Window, Renderer. Returns true on success, false if there was an error.
	bool init(game::Pixel screenWidth, game::Pixel screenHeight);
	// Render SDL structs.
	void setRenderColour(Uint8 r, Uint8 g, Uint8 b, Uint8 a=255) const;
	void setRenderColour(const Colour& c) const;
	void renderRect(const SDL_Rect& rect, Uint8 thickness=1) const;
	void renderLine(const SDL_Point& start, const SDL_Point& end) const;
	void renderLines(const std::vector<SDL_Point>& points) const;
	void renderRay(const SDL_Point& origin, float dirx, float diry, Uint16 length=1000, Uint8 thickness=1) const;
	void renderPoly(const std::vector<SDL_Point>& points) const;
	void renderPoint(const SDL_Point& point, Uint8 pointSize=1) const;
	void renderPoints(const std::vector<SDL_Point>& points, Uint8 pointSize=1) const;
	void renderCircle(const SDL_Point& center, Uint16 radius, Uint8 thickness=1) const;
	// Render Geometry shapes.
	void renderRect(const ctp::Rect& r, const ctp::Coord2& pos, Uint8 thickness=1) const;
	void renderPoly(const ctp::Polygon& p, const ctp::Coord2& pos) const;
	void renderPolyVerts(const ctp::Polygon& p, const ctp::Coord2& pos, Uint8 pointSize=1) const;
	void renderPolyEdgeNormals(const ctp::Polygon& p, const ctp::Coord2& pos, Uint16 length=50) const;
	void renderCircle(const ctp::Circle& c, const ctp::Coord2& pos, Uint8 thickness=1) const;
	void renderShape(ctp::ConstShapeRef s, const ctp::Coord2& pos, Uint8 thickness = 1) const;

	void setWindowTitle(const std::string& text);

	void clear(const Colour& c);
	void clear();
	void present();

private:
	SDL_Window* window_;
	SDL_Renderer* renderer_;
};

#endif // INCLUDE_GRAPHICS_HPP
