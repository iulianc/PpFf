
#ifndef STAGE_HPP
#define STAGE_HPP

namespace PpFf {

    enum StageType { SOURCE_STAGE, INTERMEDIATE_STAGE, COLLECTOR_STAGE };
    
    class Stage {
    public:
        bool isSource() {
            return stageType == SOURCE_STAGE;
        }

        bool isCollector() {
            return stageType == COLLECTOR_STAGE;
        }
		
    protected:
        StageType stageType;
    };

}

#endif
