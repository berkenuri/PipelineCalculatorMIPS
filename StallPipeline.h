#ifndef __STALLPIPELINE_H__
#define __STALLPIPELINE_H__

#include "Pipeline.h"

using namespace std;

class StallPipeline : public Pipeline {

    public:
   
        // constructor
        StallPipeline(string inputFile);
    
        // these methods will overwrite the parent class's 
        void execute();
        void print();
    
};

#endif
