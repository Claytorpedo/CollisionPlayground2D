#include <SDL.h>
#include <array>
#include <iostream>
#include <numeric>
#include <memory>
#include <sstream>
#include <vector>

#include "units.hpp"
#include "constants.hpp"
#include "util.hpp"
#include "Input.hpp"
#include "Graphics.hpp"
#include "generator.hpp"

#include "geom_examples/Example.hpp"
#include "geom_examples/ExampleShapes.hpp"
#include "geom_examples/ExampleRays.hpp"

#include <Geometry2D/Geometry.hpp>

namespace game {
	const geom::Rect LEVEL_REGION = geom::Rect(util::tileToCoord(10), util::tileToCoord(5),
		game::SCREEN_WIDTH - util::tileToCoord(20), game::SCREEN_HEIGHT - util::tileToCoord(10)
	);
	const std::array<std::string, 7> EXAMPLE_NAMES {
		" - Example 1: Rectangles",
		" - Example 2: Polygons",
		" - Example 3: Circles",
		" - Example 4: Mixed shapes",
		" - Example 5: Peircing ray",
		" - Example 6: Closest ray",
		" - Example 7: Reflecting ray",
	};
	const std::string WINDOW_TITLE = "Collision Playground 2D";

	void close() {
		SDL_Quit();
	}

	void closeWithError() {
		std::cout << "Press Enter to close." << std::endl;
		std::cin.ignore();
		close();
	}

	constexpr int FPS_SMOOTHING = 20; // Get an average over several frames.
	std::vector<game::FPS> fps_counter_;
	std::string getFPS(const game::MS elapsedTime) {
		if (fps_counter_.size() >= FPS_SMOOTHING)
			fps_counter_.erase(fps_counter_.begin());
		fps_counter_.emplace_back(util::millisToFPS(elapsedTime));

		game::FPS ave = std::accumulate(fps_counter_.cbegin(), fps_counter_.cend(), 0);
		std::ostringstream stream;
		stream << "FPS: " << ave / fps_counter_.size() << " - ";
		return stream.str();
	}

	int run(int, char*[]) {
		Input input;
		Graphics graphics;
		game::MS currentTime, previousTime, elapsedTime;
		gen::init();

		if (!graphics.init(game::SCREEN_WIDTH, game::SCREEN_HEIGHT)) {
			std::cerr << "Error: Failed to initialize graphics.\n";
			closeWithError();
			return -1;
		}
		std::unique_ptr<Example> example(std::make_unique<ExampleShapes>(ExampleShapes::ExampleType::MIXED, LEVEL_REGION));
		uint32_t exampleNum{ 4 };
		previousTime = SDL_GetTicks();
		// Start the game loop.
		while (true) {
			if (!input.refresh())
				break; // Window was closed.
			if (input.wasKeyPressed(SDLK_ESCAPE))
				break;
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

			currentTime = SDL_GetTicks();
			elapsedTime = currentTime - previousTime;
			previousTime = currentTime;
			example->update(input, elapsedTime);

			graphics.clear(game::BACKGROUND_COLOUR);
			example->draw(graphics);
			graphics.setWindowTitle(getFPS(elapsedTime) + WINDOW_TITLE + EXAMPLE_NAMES[exampleNum]);
			graphics.present();
		}
		close();
		return 0;
	}
}