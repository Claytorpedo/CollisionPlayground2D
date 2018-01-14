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

#include "Geometry2D/Geometry.hpp"

namespace game {
	const geom::Rect LEVEL_REGION = geom::Rect(util::tileToCoord(10), util::tileToCoord(5),
		game::SCREEN_WIDTH - util::tileToCoord(20), game::SCREEN_HEIGHT - util::tileToCoord(10)
	);

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
	void showFPS(Graphics& graphics, const game::MS elapsedTime) {
		if (elapsedTime <= 0)
			return;
		if (fpsCounter.size() >= fpsSmoothing)
			fpsCounter.erase(fpsCounter.begin());
		const game::FPS fps = util::millisToFPS(elapsedTime);
		fpsCounter.push_back(fps);
		game::FPS ave(0);
		for (std::size_t i = 0; i < fpsCounter.size(); ++i) ave += fpsCounter[i];
		std::ostringstream stream("FPS: ");
		stream << ave / fpsCounter.size() << " - Collision Playground 2D";
		graphics.setWindowTitle(stream.str());
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
		previousTime = SDL_GetTicks();
		// Start the game loop.
		while (true) {
			if (!input.refresh())
				break; // Window was closed.
			if (input.wasKeyPressed(SDLK_ESCAPE))
				break;
			if (input.wasKeyPressed(SDLK_r))
				example->reset();
			if (input.wasKeyPressed(SDLK_1))
				example = std::make_unique<ExampleShapes>(ExampleShapes::ExampleType::RECT, LEVEL_REGION);
			if (input.wasKeyPressed(SDLK_2))
				example = std::make_unique<ExampleShapes>(ExampleShapes::ExampleType::POLY, LEVEL_REGION);
			if (input.wasKeyPressed(SDLK_3))
				example = std::make_unique<ExampleShapes>(ExampleShapes::ExampleType::CIRCLE, LEVEL_REGION);
			if (input.wasKeyPressed(SDLK_4))
				example = std::make_unique<ExampleShapes>(ExampleShapes::ExampleType::MIXED, LEVEL_REGION);

			currentTime = SDL_GetTicks();
			elapsedTime = currentTime - previousTime;
			previousTime = currentTime;
			example->update(input, elapsedTime);

			graphics.clear();
			example->draw(graphics);
			graphics.present();
			showFPS(graphics, elapsedTime);
		}
		close();
		return 0;
	}
}