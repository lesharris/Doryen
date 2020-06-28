// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_NOISE_HPP
#define DORYEN_NOISE_HPP

#include <Doryen/libtcod.hpp>

#include "Interface/ISample.hpp"

using namespace Doryen;

namespace Functor
{
	class Noise : public ISample
	{

	private:

		Console sample;

	public:

		Noise(const std::string _name, const Console& _console);

		void render(KeyCode key, const Mouse& mouse) override;

	};
}

#endif //DORYEN_NOISE_HPP