#ifndef __DATAFORWARDPIPELINE_H__
#define __DATAFORWARDPIPELINE_H__

#include "Pipeline.h"

class DataForwardPipeline : public Pipeline {

    public:
    
        // constructor
        DataForwardPipeline(string inputFile);
    
        // these methods will overwrite the parent class's
        void execute();
        void print();


};

#endif

