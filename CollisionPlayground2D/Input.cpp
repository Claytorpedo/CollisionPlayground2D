#include "Input.h"

#include <bitset>
#include "SDL2/SDL.h"

bool Input::refresh() {
	clear();
	return poll();
}

bool Input::poll() {
	SDL_Event e;
	while (SDL_PollEvent( &e ) ) {
		switch(e.type) {
		case SDL_KEYDOWN:
			keyDownEvent(  mapSDLKToKey(e.key.keysym.sym) );
			break;
		case SDL_KEYUP:
			keyUpEvent(  mapSDLKToKey(e.key.keysym.sym) );
			break;
		case SDL_QUIT: // User closes window.
			return false;
		default:
			break;
		}
	}
	return true;
}

Input::Key Input::mapSDLKToKey(SDL_Keycode k) const {
	switch(k) {
	case SDLK_LEFT:
	case SDLK_a:
		return Input::LEFT;
	case SDLK_RIGHT:
	case SDLK_d:
		return Input::RIGHT;
	case SDLK_UP:
	case SDLK_w:
		return Input::UP;
	case SDLK_DOWN:
	case SDLK_s:
		return Input::DOWN;
	case SDLK_ESCAPE:
		return Input::ESC;
	case SDLK_r:
		return Input::R;
	case SDLK_e:
		return Input::E;
	default:
		break;
	}
	return Input::INVALID;
}

void Input::clear() {
	pressed_keys_.reset();
	released_keys_.reset();
}
void Input::keyDownEvent(Key k) {
	pressed_keys_[k] = true;
	held_keys_[k] = true;
}
void Input::keyUpEvent(Key k) {
	released_keys_[k] = true;
	held_keys_[k] = false;
}

bool Input::isKeyHeld(Key k)      { return held_keys_[k]; }
bool Input::wasKeyPressed(Key k)  { return pressed_keys_[k]; }
bool Input::wasKeyReleased(Key k) { return released_keys_[k]; }