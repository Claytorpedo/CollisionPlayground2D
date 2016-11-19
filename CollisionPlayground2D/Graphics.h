#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <SDL.h>
#include <vector>

#include "Units.h"

class Graphics {
private:
	SDL_Window* window_;
	SDL_Renderer* renderer_;
public:
	Graphics();
	~Graphics();

	// Initialize the Window, Renderer. Returns true on success, false if there was an error.
	bool init();

	void setRenderColour(Uint8 r, Uint8 g, Uint8 b, Uint8 a=255) const;
	void renderRect(const SDL_Rect& rect, Uint8 thickness=1) const;
	void renderLine(const SDL_Point& start, const SDL_Point& end, Uint8 thickness=1) const;
	void renderRay(const SDL_Point& origin, const units::Coordinate2D& dir) const;
	void renderPoly(std::vector<SDL_Point>& points) const;
	void renderPoint(const SDL_Point& point, Uint8 pointSize=1) const;
	void renderCircle(const SDL_Point& center, Uint8 radius) const;

	void clear();
	void present();
};


#endif // _GRAPHICS_H