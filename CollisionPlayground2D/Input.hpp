#ifndef INCLUDE_INPUT_HPP
#define INCLUDE_INPUT_HPP

#include <unordered_map>
#include <SDL.h>

class Input {
public:
	// Clear old input and poll for new input.
	// Returns false if the window was closed, otherwise returns true.
	bool refresh();

	// Clear keydown and keyup events.
	void clearFrame();
	// Clear all events.
	void clear();

	// Poll for new input.
	// Returns false if the window was closed, otherwise returns true.
	bool poll();

	void keyDownEvent(SDL_Keycode k);
	void keyUpEvent(SDL_Keycode k);

	// Return whether a key is being held down.
	// Note that there is no time delay for this: a key just pressed down is also held.
	bool isKeyHeld(SDL_Keycode k) const;
	// See if a key was pressed down.
	bool wasKeyPressed(SDL_Keycode k) const;
	// See if a key stopped being pressed/held down.
	bool wasKeyReleased(SDL_Keycode k) const;

private:
	std::unordered_map<SDL_Keycode, bool> held_keys_;
	std::unordered_map<SDL_Keycode, bool> pressed_keys_;
	std::unordered_map<SDL_Keycode, bool> released_keys_;
};

#endif // INCLUDE_INPUT_HPP
