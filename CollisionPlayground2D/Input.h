#ifndef _INPUT_H
#define _INPUT_H

#include <bitset>
#include <SDL.h>

class Input {
public:
	enum Key {
		INVALID, // For inputs that are not being handled.
		LEFT,
		RIGHT,
		UP,
		DOWN,
		ESC,
		R,
		E,

		TOTAL_KEYS
	};
	Input() : held_keys_(), pressed_keys_(), released_keys_() {}
	~Input() {}

	// Clear old input and poll for new input.
	// Returns false if the window was closed, otherwise returns true.
	bool refresh();

	// Clear bitsets for new frame.
	void clear();

	// Poll for new input.
	// Returns false if the window was closed, otherwise returns true.
	bool poll();

	// Get equivalent Key for a given SDL_Keycode.
	Key mapSDLKToKey(SDL_Keycode k) const;

	void keyDownEvent(Key k);
	void keyUpEvent(Key k);

	// Return whether a key is being held down.
	// Note that there is no time delay for this: a key just pressed down is also held.
	bool isKeyHeld(Key k);
	// See if a key was pressed down.
	bool wasKeyPressed(Key k);
	// See if a key stopped being pressed/held down.
	bool wasKeyReleased(Key k);

private:
	std::bitset<TOTAL_KEYS> held_keys_;
	std::bitset<TOTAL_KEYS> pressed_keys_;
	std::bitset<TOTAL_KEYS> released_keys_;
};


#endif // _INPUT_H