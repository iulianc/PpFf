
#include <stages/BaseStage.hpp>

namespace PpFf{

	template< typename TOperator >
	class BaseCollectors : public BaseStage< TOperator > {
	public:
		bool const isFinalStage = true;
	};

}
