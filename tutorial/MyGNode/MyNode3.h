/***************************
@Author: yeshenyong
@Contact: 1050575224@qq.com
@File: MyNode3.h
@Time: 2022/11/27 00:57
@Desc: 
***************************/

#ifndef CGRAPH_MYNODE3_H
#define CGRAPH_MYNODE3_H

#include "../../src/CGraph.h"

class MyNode3 : public CGraph::GNode {

public:
    CStatus run () override {
        CStatus status;
        CGraph::CGRAPH_ECHO("[%s], enter MyNode3 run function. Sleep for 20 second ... ", this->getName().c_str());
        CGRAPH_SLEEP_SECOND(20)
        return status;
    }
};

#endif //CGRAPH_MYNODE3_H
