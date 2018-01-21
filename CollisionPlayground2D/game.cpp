#include <SDL.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>

#include "units.hpp"
#include "constants.hpp"
#include "util.hpp"
#include "Input.hpp"
#include "Graphics.hpp"
#include "generator.hpp"

#include "geom_examples/Example.hpp"
#include "geom_examples/ExampleShapes.hpp"
#include "geom_examples/ExampleRays.hpp"

#include "Geometry2D/Geometry.hpp"

namespace game {
	const geom::Rect LEVEL_REGION = geom::Rect(util::tileToCoord(10), util::tileToCoord(5),
		game::SCREEN_WIDTH - util::tileToCoord(20), game::SCREEN_HEIGHT - util::tileToCoord(10)
	);
	const std::string EXAMPLE_HEADER = " - Example ";
	const std::vector<std::string> EXAMPLE_NAMES {
		EXAMPLE_HEADER + "0: unset",
		EXAMPLE_HEADER + "1: Rectangles",
		EXAMPLE_HEADER + "2: Polygons",
		EXAMPLE_HEADER + "3: Circles",
		EXAMPLE_HEADER + "4: Mixed shapes",
		EXAMPLE_HEADER + "5: Peircing ray",
		EXAMPLE_HEADER + "6: Closest ray",
		EXAMPLE_HEADER + "7: Reflecting ray",
	};
	static std::string window_title_ = "Collision Playground 2D";
	static std::string fps_display_ = "";
	static std::string example_title_ = "";

	void close() {
		SDL_Quit();
	}
	void closeWithError() {
		std::cout << "Press Enter to close." << std::endl;
		std::cin.ignore();
		close();
	}

	const int fpsSmoothing = 20; // Get an average over several frames.
	std::vector<game::FPS> fpsCounter;
	void setFPSDisplay(Graphics& graphics, const game::MS elapsedTime) {
		if (elapsedTime <= 0)
			return;
		if (fpsCounter.size() >= fpsSmoothing)
			fpsCounter.erase(fpsCounter.begin());
		const game::FPS fps = util::millisToFPS(elapsedTime);
		fpsCounter.push_back(fps);
		game::FPS ave(0);
		for (std::size_t i = 0; i < fpsCounter.size(); ++i) ave += fpsCounter[i];
		std::ostringstream stream;
		stream << "FPS: " << ave / fpsCounter.size() << " - ";
		fps_display_ = stream.str();
	}

	int run(int argc, char* args[]) {
		Input input;
		Graphics graphics;
		game::MS currentTime, previousTime, elapsedTime;
		gen::init();

		if (!graphics.init(game::SCREEN_WIDTH, game::SCREEN_HEIGHT)) {
			std::cerr << "Error: Failed to initialize graphics.\n";
			closeWithError();
			return -1;
		}
		std::unique_ptr<Example> example(std::make_unique<ExampleShapes>(ExampleShapes::ExampleType::POLY, LEVEL_REGION));
		example_title_ = EXAMPLE_NAMES[2];
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
				example_title_ = EXAMPLE_NAMES[1];
			} else if (input.wasKeyPressed(SDLK_2)) {
				example = std::make_unique<ExampleShapes>(ExampleShapes::ExampleType::POLY, LEVEL_REGION);
				example_title_ = EXAMPLE_NAMES[2];
			} else if (input.wasKeyPressed(SDLK_3)) {
				example = std::make_unique<ExampleShapes>(ExampleShapes::ExampleType::CIRCLE, LEVEL_REGION);
				example_title_ = EXAMPLE_NAMES[3];
			} else if (input.wasKeyPressed(SDLK_4)) {
				example = std::make_unique<ExampleShapes>(ExampleShapes::ExampleType::MIXED, LEVEL_REGION);
				example_title_ = EXAMPLE_NAMES[4];
			} else if (input.wasKeyPressed(SDLK_5)) {
				example = std::make_unique<ExampleRays>(ExampleRays::ExampleType::PEIRCING, LEVEL_REGION);
				example_title_ = EXAMPLE_NAMES[5];
			} else if (input.wasKeyPressed(SDLK_6)) {
				example = std::make_unique<ExampleRays>(ExampleRays::ExampleType::CLOSEST, LEVEL_REGION);
				example_title_ = EXAMPLE_NAMES[6];
			} else if (input.wasKeyPressed(SDLK_7)) {
				example = std::make_unique<ExampleRays>(ExampleRays::ExampleType::REFLECTING, LEVEL_REGION);
				example_title_ = EXAMPLE_NAMES[7];
			}

			currentTime = SDL_GetTicks();
			elapsedTime = currentTime - previousTime;
			previousTime = currentTime;
			example->update(input, elapsedTime);

			graphics.clear();
			example->draw(graphics);
			setFPSDisplay(graphics, elapsedTime);
			graphics.setWindowTitle(fps_display_ + window_title_ + example_title_);
			graphics.present();
		}
		close();
		return 0;
	}
}