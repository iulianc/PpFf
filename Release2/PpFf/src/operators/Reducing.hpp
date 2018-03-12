#ifndef REDUCING_HPP
#define REDUCING_HPP

#include <functional>

namespace PpFf{

	template< typename In, typename Out >
	class Reducing{
	public:
		Out identity{};
		std::function< void(Out*, In*) > const& accumulator;
		std::function< void(Out*, Out*) > const& combiner;
		bool const isCombiner;

		Reducing(std::function< void(Out*, In*) > const& accumulator): accumulator(accumulator), combiner([](Out*, Out*) {}), isCombiner(false) { }
		Reducing(std::function< void(Out*, In*) > const& accumulator, std::function< void(Out*, Out*) > const& combiner): accumulator(accumulator), combiner(combiner), isCombiner(true) { }
		Reducing(Out identity, std::function< void(Out*, In*) > const& accumulator): identity(identity), accumulator(accumulator), combiner([](Out*, Out*) {}), isCombiner(false) { }
		Reducing(Out identity, std::function< void(Out*, In*) > const& accumulator, std::function< void(Out*, Out*) > const& combiner): identity(identity), accumulator(accumulator), combiner(combiner), isCombiner(true) { }
		~Reducing() { }
	};

}

#endif
