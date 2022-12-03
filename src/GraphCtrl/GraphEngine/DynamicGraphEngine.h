/***************************
@Author: YeShenYong
@Contact: 1050575224@qq.com
@File: DynamicGraphEngine.h
@Time: 2022/12/01 21:35 下午
@Desc: 
***************************/

#ifndef CGRAPH_DYNAMICGRAPHENGINE_H
#define CGRAPH_DYNAMICGRAPHENGINE_H

#include "GraphEngine.h"
#include "GraphEngineCounter.h"

CGRAPH_NAMESPACE_BEGIN

class DynamicGraphEngine : public GraphEngine {
public:
    explicit DynamicGraphEngine();

    virtual CStatus init() final;

    virtual CStatus process() final;

    virtual CStatus run() final;

    /**
     * graph engine 运行前重置
     * @param element
     * @return
     */
    CStatus beforeRun();

    /**
     * 动态图运行
     * @param
     * @return
     */
    CVoid asyncRun();

    /**
     * 等待唤醒
     * @param
     * @return
     */
    CVoid wait();

    /**
     * element 运行调用
     * @param element
     * @return
     */
    CVoid runElementTask(GElementPtr element);

    /**
     * element 运行
     * @param element
     * @return
     */
    CVoid runElement(GElementPtr element);

    /**
     * element 运行完成处理
     * @param element
     * @return
     */
    CVoid afterElementRun(GElementPtr element);

    /**
     * 节点运行检验
     * @param element
     * @return
     */
    CBool checkElement(GElementPtr element);

    virtual ~DynamicGraphEngine();

private:
    GraphEngineCounterPtr graph_counter_ { nullptr };
};

using DynamicGraphEnginePtr = DynamicGraphEngine *;

CGRAPH_NAMESPACE_END

#endif // CGRAPH_DYNAMICGRAPHENGINE_H