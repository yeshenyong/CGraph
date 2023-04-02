#include "GPipelinePy.h"

CGRAPH_NAMESPACE_BEGIN

GPipelinePy::GPipelinePy() {
    session_ = URandom<>::generateSession(CGRAPH_STR_PIPELINE);
    element_manager_ = CGRAPH_SAFE_MALLOC_COBJECT(GElementManager)
    param_manager_ = CGRAPH_SAFE_MALLOC_COBJECT(GParamManager)
    daemon_manager_ = CGRAPH_SAFE_MALLOC_COBJECT(GDaemonManager)
    event_manager_ = CGRAPH_SAFE_MALLOC_COBJECT(GEventManager)
}


GPipelinePy::~GPipelinePy() {
    CGRAPH_DELETE_PTR(daemon_manager_)
    CGRAPH_DELETE_PTR(element_manager_)
    CGRAPH_DELETE_PTR(param_manager_)
    CGRAPH_DELETE_PTR(event_manager_)
}


CStatus GPipelinePy::init() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)    // 必须是非初始化的状态下，才可以初始化。反之同理
    CGRAPH_ASSERT_NOT_NULL(element_manager_)
    CGRAPH_ASSERT_NOT_NULL(param_manager_)
    CGRAPH_ASSERT_NOT_NULL(daemon_manager_)
    CGRAPH_ASSERT_NOT_NULL(event_manager_)

    status += initSchedule();
    CGRAPH_FUNCTION_CHECK_STATUS

    status += param_manager_->init();
    status += event_manager_->init();
    status += element_manager_->init();
    status += daemon_manager_->init();    // daemon的初始化，需要晚于所有element的初始化
    CGRAPH_FUNCTION_CHECK_STATUS

    is_init_ = true;
    CGRAPH_FUNCTION_END
}


CStatus GPipelinePy::run() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(true)
    CGRAPH_ASSERT_NOT_NULL(element_manager_)
    CGRAPH_ASSERT_NOT_NULL(param_manager_)

    /**
     * 1. 将所有的 GParam 设置为初始值
     * 2. 执行dag逻辑
     * 3. 将所有的 GParam 复原
     */
    status = param_manager_->setup();
    CGRAPH_FUNCTION_CHECK_STATUS

    status += element_manager_->run();
    param_manager_->resetWithStatus(status);
    CGRAPH_FUNCTION_END
}


CStatus GPipelinePy::destroy() {
    CGRAPH_FUNCTION_BEGIN

    CGRAPH_ASSERT_INIT(true)
    CGRAPH_ASSERT_NOT_NULL(element_manager_)
    CGRAPH_ASSERT_NOT_NULL(param_manager_)
    CGRAPH_ASSERT_NOT_NULL(daemon_manager_)
    CGRAPH_ASSERT_NOT_NULL(event_manager_)

    status += event_manager_->destroy();
    status += daemon_manager_->destroy();
    status += element_manager_->destroy();
    status += param_manager_->destroy();
    CGRAPH_FUNCTION_CHECK_STATUS

    // 结束单个线程池信息
    status += schedule_.destroy();
    CGRAPH_FUNCTION_CHECK_STATUS

    is_init_ = false;
    CGRAPH_FUNCTION_END
}


CStatus GPipelinePy::process(CSize runTimes) {
    CGRAPH_FUNCTION_BEGIN
    status = init();
    CGRAPH_FUNCTION_CHECK_STATUS

    while (runTimes-- > 0) {
        status = run();
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    status = destroy();
    CGRAPH_FUNCTION_END
}


CStatus GPipelinePy::dump(std::ostream& oss) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(element_manager_)
    oss << "digraph CGraph {\n";
    oss << "compound=true;\n";

    for (const auto& element : element_manager_->manager_elements_) {
        CGRAPH_ASSERT_NOT_NULL(element)
        element->dump(oss);
    }

    oss << "}\n";
    CGRAPH_FUNCTION_END
}


GPipelinePyPtr GPipelinePy::setGElementRunTtl(CMSec ttl) {
    CGRAPH_ASSERT_INIT_RETURN_NULL(false)
    CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(element_manager_)
    CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(element_manager_->engine_)

    // 在element_manager中区执行信息了，所以ttl放到
    element_manager_->engine_->element_run_ttl_ = ttl;
    return this;
}


GPipelinePyPtr GPipelinePy::setGEngineType(GEngineType type) {
    CGRAPH_ASSERT_INIT_RETURN_NULL(false)
    CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(element_manager_)

    element_manager_->setEngineType(type);
    return this;
}


GPipelinePyPtr GPipelinePy::setUniqueThreadPoolConfig(const UThreadPoolConfig& config) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT_RETURN_NULL(false)

    /**
     * 实际是将信息传递给 schedule中，如果是unique的话，就使用这个参数
     * 如果是 shared的话，其实配置是无效的
     */
    schedule_.config_ = config;
    CGRAPH_CHECK_STATUS_RETURN_THIS_OR_NULL
}


GPipelinePyPtr GPipelinePy::setSharedThreadPool(UThreadPoolPtr ptr) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(ptr)
    CGRAPH_ASSERT_INIT_RETURN_NULL(false)

    status = schedule_.makeShared(ptr);
    CGRAPH_CHECK_STATUS_RETURN_THIS_OR_NULL
}


CStatus GPipelinePy::initSchedule() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(event_manager_)
    CGRAPH_ASSERT_NOT_NULL(element_manager_)

    status = schedule_.init();
    CGRAPH_FUNCTION_CHECK_STATUS

    event_manager_->setThreadPool(schedule_.thread_pool_);
    element_manager_->setThreadPool(schedule_.thread_pool_);

    // 设置所有的element 中的thread_pool
    for (auto& iter : this->element_repository_) {
        CGRAPH_ASSERT_NOT_NULL(iter)
        iter->setThreadPool(schedule_.thread_pool_);
    }

    CGRAPH_FUNCTION_END
}

CStatus GPipelinePy::registerGElement(GElementPtr elementRef,
                                        const GElementPtrSet &dependElements,
                                        const std::string &name,
                                        CSize loop) {

    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)

    status = (elementRef)->setElementInfo(dependElements, name, loop,
                                           this->param_manager_,
                                           this->event_manager_);

    CGRAPH_FUNCTION_CHECK_STATUS

    status = element_manager_->add(dynamic_cast<GElementPtr>(elementRef));
    CGRAPH_FUNCTION_CHECK_STATUS
    element_repository_.insert(elementRef);

    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END