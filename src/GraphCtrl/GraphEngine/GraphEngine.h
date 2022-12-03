/***************************
@Author: YeShenYong
@Contact: 1050575224@qq.com
@File: GraphEngine.h
@Time: 2022/12/01 21:02 下午
@Desc: 
***************************/

#ifndef CGRAPH_GRAPHENGINE_H
#define CGRAPH_GRAPHENGINE_H

#include "../GraphObject.h"
#include "../../CBasic/CBasicInclude.h"
#include "../GraphElement/GElement.h"
#include "../GraphElement/GElementSorter.h"
#include "../GraphElement/GGroup/GCluster/GCluster.h"

CGRAPH_NAMESPACE_BEGIN

class GraphEngine : public GraphObject, public GraphManager<GElement> {

public:
    /**
     * 默认构造函数
     */
    explicit GraphEngine();

    virtual CStatus init() override {
        CGRAPH_EMPTY_FUNCTION
    }

    virtual CStatus run() override {
        CGRAPH_EMPTY_FUNCTION
    }

    virtual CStatus process() {
        CGRAPH_EMPTY_FUNCTION
    }

    /**
     * 添加一个element
     * @param element
     * @return
     */
    virtual CStatus add(GElementPtr element) final;

    /**
     * 删除一个element
     * @param element
     * @return
     */
    virtual CStatus remove(GElementPtr element) final;

    /**
     * 查找一个element
     * @param element
     * @return
     */
    virtual CBool find(GElementPtr element) const final;

    /**
     * 清空所有element，必须实现的内容
     * @return
     */
    virtual CStatus clear() final;

    const GSortedGElementPtrSet getElements();

    CVoid setGElementRunTtl(CMSec ttl);

    CVoid setScheduleStrategy(CInt strategy);

    virtual ~GraphEngine() = default;

protected:
    GSortedGElementPtrSet manager_elements_;                    // 保存节点信息的内容
    UThreadPoolPtr thread_pool_ { nullptr };                    // 内部执行的线程池
    CMSec element_run_ttl_ = CGRAPH_DEFAULT_ELEMENT_RUN_TTL;    // 单个节点最大运行周期
    int schedule_strategy_;                                     // 调度策略
};

using GraphEnginePtr = GraphEngine *;

CGRAPH_NAMESPACE_END

#endif // CGRAPH_GRAPHENGINE_H