

#include <stages/BaseStage.hpp>

namespace PpFf {

    template< typename TOperator >
    class Stage : public BaseStage<TOperator> {
    protected:
        bool const isFinalStage = false;
    };
}
