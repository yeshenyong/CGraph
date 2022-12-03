#include "GraphEngine.h"

CGRAPH_NAMESPACE_BEGIN

GraphEngine::GraphEngine() {
    thread_pool_ = UThreadPoolSingleton::get();
    schedule_strategy_ = CGRAPH_DEFAULT_TASK_STRATEGY;
}


CStatus GraphEngine::add(GElementPtr element) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(element)

    this->manager_elements_.emplace(element);

    CGRAPH_FUNCTION_END
}


CBool GraphEngine::find(GElementPtr element) const {
    if (nullptr == element) {
        return false;
    }

    return manager_elements_.end() != manager_elements_.find(element);
}


CStatus GraphEngine::remove(GElementPtr element) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(element)

    manager_elements_.erase(element);
    CGRAPH_FUNCTION_END
}


CStatus GraphEngine::clear() {
    CGRAPH_FUNCTION_BEGIN

    for (auto element : manager_elements_) {
        CGRAPH_DELETE_PTR(element)
    }

    manager_elements_.clear();
    CGRAPH_FUNCTION_END
}


const GSortedGElementPtrSet GraphEngine::getElements() {
    return this->manager_elements_;
}


CVoid GraphEngine::setGElementRunTtl(CMSec ttl) {
    element_run_ttl_ = ttl;
}


CVoid GraphEngine::setScheduleStrategy(CInt strategy) {
    schedule_strategy_ = strategy;
}

CGRAPH_NAMESPACE_END