#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <SDL.h>
#include <vector>

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
	void renderRect(SDL_Rect& rect, Uint8 thickness=1) const;
	void renderLine(SDL_Point& start, SDL_Point& end, Uint8 thickness=1) const;
	void renderPoly(std::vector<SDL_Point>& points) const;
	void renderPoint(SDL_Point& point, Uint8 pointSize=1) const;

	void clear();
	void present();
};


#endif // _GRAPHICS_H