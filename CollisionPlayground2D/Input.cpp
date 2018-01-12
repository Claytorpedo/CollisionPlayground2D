#include "Input.hpp"

#include <bitset>

bool Input::refresh() {
	clearFrame();
	return poll();
}

bool Input::poll() {
	SDL_Event e;
	while (SDL_PollEvent( &e ) ) {
		switch(e.type) {
		case SDL_KEYDOWN:
			keyDownEvent(e.key.keysym.sym);
			break;
		case SDL_KEYUP:
			keyUpEvent(e.key.keysym.sym);
			break;
		case SDL_QUIT: // User closes window.
			return false;
		default:
			break;
		}
	}
	return true;
}

void Input::clearFrame() {
	pressed_keys_.clear();
	released_keys_.clear();
}
void Input::clear() {
	pressed_keys_.clear();
	released_keys_.clear();
	held_keys_.clear();
}
void Input::keyDownEvent(SDL_Keycode k) {
	pressed_keys_[k] = true;
	held_keys_[k] = true;
}
void Input::keyUpEvent(SDL_Keycode k) {
	released_keys_[k] = true;
	held_keys_[k] = false;
}

bool Input::isKeyHeld(SDL_Keycode k)      { return held_keys_[k]; }
bool Input::wasKeyPressed(SDL_Keycode k)  { return pressed_keys_[k]; }
bool Input::wasKeyReleased(SDL_Keycode k) { return released_keys_[k]; }