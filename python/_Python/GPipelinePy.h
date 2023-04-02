/***************************
@Author: YeShenYong
@Contact: 1050575224@qq.com
@File: GPipelinePy.h
@Time: 2023/4/2 10:15 下午
@Desc: 
***************************/

#ifndef CGRAPH_GPIPELINEPY_H
#define CGRAPH_GPIPELINEPY_H

#include <vector>
#include <memory>
#include <list>
#include <sstream>

#include "../GPipelineObject.h"
#include "../_GSchedule/GScheduleInclude.h"
#include "../../GraphElement/GElementInclude.h"
#include "../../GraphDaemon/GDaemonInclude.h"
#include "../../GraphEvent/GEventInclude.h"

CGRAPH_NAMESPACE_BEGIN

class GPipelinePy : public GPipelineObject,
                    public CDescInfo {
public:
    /**
     * 初始化pipeline信息
     * @return
     */
    CStatus init() override;

    /**
     * 执行pipeline信息
     * @return
     */
    CStatus run() override;

    /**
     * 逆初始化pipeline信息
     * @return
     */
    CStatus destroy() override;

    /**
     * 一次性执行完成初始化，执行runTimes次，和销毁的过程
     * @param runTimes
     * @return
     */
    CStatus process(CSize runTimes = CGRAPH_DEFAULT_LOOP_TIMES);

    /**
     * 生成图可视化 graphviz 信息
     * @param oss
     * @return
     * @notice 将输出的内容，复制到 https://dreampuf.github.io/GraphvizOnline/ 中查看效果
    */
    CStatus dump(std::ostream& oss = std::cout);

    /**
     * 设置执行的最大时间周期，单位为毫秒
     * @param ttl
     * @return
     * @notice beta接口
     */
    GPipelinePy* setGElementRunTtl(CMSec ttl);

    /**
     * 设置引擎策略
     * @param type
     * @return
     */
    GPipelinePy* setGEngineType(GEngineType type);

    /**
     * 设置本pipeline内部线程池相关信息
     * @param config
     * @return
     */
    GPipelinePy* setUniqueThreadPoolConfig(const UThreadPoolConfig& config);

    /**
     * 设置共享的线程池
     * @param ptr
     * @return
     */
    GPipelinePy* setSharedThreadPool(UThreadPoolPtr ptr);

    /**
     * 注册GParam 交互类集合
     * @return
     */
    CGRAPH_DECLARE_GPARAM_MANAGER_WRAPPER

    /**
     * Python 在图中注册一个Element信息
     * 如果注册的是GNode信息，则内部自动生成
     * 如果注册的是GGroup信息，则需外部提前生成，然后注册进来
     * @param elementRef
     * @param dependElements
     * @param name
     * @param loop
     * @return
     */
    CStatus registerGElement(GElementPtr elementRef,
                             const GElementPtrSet &dependElements,
                             const std::string &name = CGRAPH_EMPTY,
                             CSize loop = CGRAPH_DEFAULT_LOOP_TIMES);

protected:
    explicit GPipelinePy();
    ~GPipelinePy() override;

    /**
     * 初始化调度信息，包括线程池
     * @return
     */
    CStatus initSchedule();

    /** 不允许外部赋值和构造 */
    CGRAPH_NO_ALLOWED_COPY(GPipelinePy)

private:
    GElementManagerPtr element_manager_ = nullptr;              // 节点管理类（管理所有注册过的element信息）
    GParamManagerPtr param_manager_ = nullptr;                  // 参数管理类
    GDaemonManagerPtr daemon_manager_ = nullptr;                // 守护管理类
    GEventManagerPtr event_manager_ = nullptr;                  // 事件管理类

    GSchedule schedule_;                                        // 调度管理类
    GElementPtrSet element_repository_;                         // 标记创建的所有节点，最终释放使用

    friend class GPipelineFactoryPy;
    friend class UAllocator;
};

using GPipelinePyPtr = GPipelinePy *;
using GPipelinePyPtrList = std::list<GPipelinePyPtr>;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GPIPELINEPY_H
