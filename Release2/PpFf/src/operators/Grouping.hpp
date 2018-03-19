#ifndef GROUPING_HPP
#define GROUPING_HPP

#include <functional>

namespace PpFf{

	template< typename In, typename K, typename V >
	class Grouping{
	public:
		std::function< K*(In*) > const& functionOnKey;
		std::function< V*(In*) > const& functionOnValue;
		bool const isFunctionOnKey;
		bool const isFunctionOnValue;

		Grouping(): isFunctionOnKey(false), isFunctionOnValue(false) { }
		Grouping(std::function< K*(In*) > const& functionOnKey): functionOnKey(functionOnKey), functionOnValue([](In*) {return new V();}), isFunctionOnKey(true), isFunctionOnValue(false) { }
		Grouping(std::function< K*(In*) > const& functionOnKey, std::function< V*(In*) > const& functionOnValue): functionOnKey(functionOnKey), functionOnValue(functionOnValue), isFunctionOnKey(true), isFunctionOnValue(true) { }
		~Grouping() { }
	};
}

#endif
