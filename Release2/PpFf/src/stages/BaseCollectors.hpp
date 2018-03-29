#include <stages/BaseStage.hpp>

namespace PpFf {

    template< typename TOperator >
    class BaseCollectors: public BaseStage<TOperator> {
    protected:
        bool const isFinalStage = true;
    };

}
