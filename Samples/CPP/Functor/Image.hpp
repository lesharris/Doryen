// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_IMAGE_HPP
#define DORYEN_IMAGE_HPP

#include <Doryen/libtcod.hpp>

#include "Interface/ISample.hpp"

using namespace Doryen;

namespace Functor
{

	class Image : public ISample
	{

	private:

		Console sample;

	public:

		Image(const std::string _name, const Console& _console);

		void render(KeyCode key, const Mouse& mouse) override;

	};

}

#endif //DORYEN_IMAGE_HPP