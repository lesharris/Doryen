// Joan Andrés (@Andres6936) Github.

#ifndef DORYEN_NAME_HPP
#define DORYEN_NAME_HPP

#include <Doryen/libtcod.hpp>

#include "Interface/ISample.hpp"

using namespace Doryen;

namespace Functor
{

	class Name : public ISample
	{

	private:

		std::reference_wrapper<Console> sample;

	public:

		Name(const std::string _name, std::reference_wrapper<Console> _console);

		void render(KeyCode key, const Doryen::Mouse& mouse) override;

	};

}

#endif //DORYEN_NAME_HPP
