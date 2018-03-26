

#include <stages/BaseStage.hpp>

namespace PpFf {

    template< typename TOperator >
    class Stage : public BaseStage<TOperator> {
    public:
        bool const isFinalStage = false;
    };
}
