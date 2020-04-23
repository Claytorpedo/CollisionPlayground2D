#include <SDL.h>
#include <array>
#include <iostream>
#include <memory>
#include <numeric>
#include <sstream>
#include <string_view>
#include <vector>

#include "Graphics.hpp"
#include "Input.hpp"

#include "constants.hpp"
#include "game.hpp"
#include "generator.hpp"
#include "units.hpp"
#include "util.hpp"

#include "geom_examples/Example.hpp"
#include "geom_examples/ExampleRays.hpp"
#include "geom_examples/ExampleShapes.hpp"

#include <Geometry2D/Geometry.hpp>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

namespace game {
namespace {
const ctp::Rect LEVEL_REGION = ctp::Rect{160, 80, SCREEN_WIDTH - 320, SCREEN_HEIGHT - 160};
constexpr std::array<std::string_view, 7> EXAMPLE_NAMES{
	" - Example 1: Rectangles",
	" - Example 2: Polygons",
	" - Example 3: Circles",
	" - Example 4: Mixed shapes",
	" - Example 5: Peircing ray",
	" - Example 6: Closest ray",
	" - Example 7: Reflecting ray",
};
constexpr std::string_view WINDOW_TITLE = "Collision Playground 2D";

Input input;
Graphics graphics;
MS previousTime = 0;
MS elapsedTime = 0;
std::unique_ptr<Example> example;
std::size_t exampleNum = 0;

void close() {
	SDL_Quit();
}

void closeWithError() {
	std::cout << "Press Enter to close." << std::endl;
	std::cin.ignore();
	close();
}

constexpr int FPS_SMOOTHING = 20; // Get an average over several frames.
std::vector<FPS> fps_counter_;
std::string getFPS() {
	if (fps_counter_.size() >= FPS_SMOOTHING)
		fps_counter_.erase(fps_counter_.begin());
	fps_counter_.emplace_back(util::millisToFPS(elapsedTime));

	FPS ave = std::accumulate(fps_counter_.cbegin(), fps_counter_.cend(), 0);
	std::ostringstream stream;
	stream << "FPS: " << ave / fps_counter_.size() << " - ";
	return stream.str();
}

#ifdef __EMSCRIPTEN__
void
#else
bool
#endif
update() {

	if (!input.refresh() || input.wasKeyPressed(SDLK_ESCAPE)) {
#ifdef __EMSCRIPTEN__
		emscripten_cancel_main_loop();
		return;
#else
		return false;
#endif
	}

	if (input.wasKeyPressed(SDLK_r)) {
		example->reset();
	} else if (input.wasKeyPressed(SDLK_1)) {
		example = std::make_unique<ExampleShapes>(ExampleShapes::ExampleType::RECT, LEVEL_REGION);
		exampleNum = 0;
	} else if (input.wasKeyPressed(SDLK_2)) {
		example = std::make_unique<ExampleShapes>(ExampleShapes::ExampleType::POLY, LEVEL_REGION);
		exampleNum = 1;
	} else if (input.wasKeyPressed(SDLK_3)) {
		example = std::make_unique<ExampleShapes>(ExampleShapes::ExampleType::CIRCLE, LEVEL_REGION);
		exampleNum = 2;
	} else if (input.wasKeyPressed(SDLK_4)) {
		example = std::make_unique<ExampleShapes>(ExampleShapes::ExampleType::MIXED, LEVEL_REGION);
		exampleNum = 3;
	} else if (input.wasKeyPressed(SDLK_5)) {
		example = std::make_unique<ExampleRays>(ExampleRays::ExampleType::PEIRCING, LEVEL_REGION);
		exampleNum = 4;
	} else if (input.wasKeyPressed(SDLK_6)) {
		example = std::make_unique<ExampleRays>(ExampleRays::ExampleType::CLOSEST, LEVEL_REGION);
		exampleNum = 5;
	} else if (input.wasKeyPressed(SDLK_7)) {
		example = std::make_unique<ExampleRays>(ExampleRays::ExampleType::REFLECTING, LEVEL_REGION);
		exampleNum = 6;
	}

	MS currentTime = SDL_GetTicks();
	elapsedTime = currentTime - previousTime;
	previousTime = currentTime;
	example->update(input, elapsedTime);

	graphics.clear(BACKGROUND_COLOUR);
	example->draw(graphics);
	std::string windowTitle = getFPS();
	windowTitle.reserve(windowTitle.size() + WINDOW_TITLE.size() + EXAMPLE_NAMES[exampleNum].size());
	windowTitle += WINDOW_TITLE;
	windowTitle += EXAMPLE_NAMES[exampleNum];
	graphics.setWindowTitle(windowTitle);
	graphics.present();

#ifndef __EMSCRIPTEN__
	return true;
#endif
}
} // namespace

int run(int, char* []) {
	gen::init();

	if (!graphics.init(SCREEN_WIDTH, SCREEN_HEIGHT)) {
		std::cerr << "Error: Failed to initialize graphics.\n";
		closeWithError();
		return -1;
	}

	example = std::make_unique<ExampleShapes>(ExampleShapes::ExampleType::MIXED, LEVEL_REGION);
	exampleNum = 4;
	previousTime = SDL_GetTicks();
	// Start the game loop.
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(update, -1, 1);
#else
	while (update());
#endif
	close();
	return 0;
}
} // namespace game
