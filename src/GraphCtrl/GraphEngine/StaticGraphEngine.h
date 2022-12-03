/***************************
@Author: YeShenYong
@Contact: 1050575224@qq.com
@File: StaticGraphEngine.h
@Time: 2022/12/01 21:42 下午
@Desc: 
***************************/

#ifndef CGRAPH_STATICGRAPHENGINE_H
#define CGRAPH_STATICGRAPHENGINE_H

#include "GraphEngine.h"

CGRAPH_NAMESPACE_BEGIN

class StaticGraphEngine : public GraphEngine {
public:
    explicit StaticGraphEngine() = default;

    virtual CStatus init() final;

    virtual CStatus process() final;

    virtual CStatus run() final;

    /**
     * 判定哪些节点是可以分到一个cluster中的
     * @return
     */
    CStatus preRunCheck();

    /**
     * 将所有的节点，分发到para_cluster_arrs_中，运行的时候使用。
     * @return
     */
    CStatus analyse();

    virtual ~StaticGraphEngine() = default;

private:
    ParaWorkedClusterArrs para_cluster_arrs_;                   // 可以并行的cluster数组
};

using StaticGraphEnginePtr = StaticGraphEngine *;

CGRAPH_NAMESPACE_END

#endif // CGRAPH_STATICGRAPHENGINE_H