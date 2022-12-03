/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GElementManager.cpp
@Time: 2021/6/2 10:33 下午
@Desc: 
***************************/

#include "GElementManager.h"

CGRAPH_NAMESPACE_BEGIN

GElementManager::GElementManager() {
    graph_engine_ = CGRAPH_SAFE_MALLOC_COBJECT(DynamicGraphEngine)
}


GElementManager::~GElementManager() {
    CGRAPH_DELETE_PTR(graph_engine_)
}


CVoid GElementManager::setStaticModule() {
    CGRAPH_DELETE_PTR(graph_engine_)
    graph_engine_ = CGRAPH_SAFE_MALLOC_COBJECT(StaticGraphEngine)
}


CVoid GElementManager::setDynamicModule() {
    CGRAPH_DELETE_PTR(graph_engine_)
    graph_engine_ = CGRAPH_SAFE_MALLOC_COBJECT(DynamicGraphEngine)
}


CStatus GElementManager::init() {
    CGRAPH_FUNCTION_BEGIN

    status = graph_engine_->init();
    CGRAPH_FUNCTION_CHECK_STATUS

    const GSortedGElementPtrSet manager_elements = graph_engine_->getElements();
    for (GElementPtr element : manager_elements) {
        status = element->fatProcessor(CFunctionType::INIT);
        CGRAPH_FUNCTION_CHECK_STATUS
        element->is_init_ = true;
    }

    CGRAPH_FUNCTION_END
}


CStatus GElementManager::destroy() {
    CGRAPH_FUNCTION_BEGIN

    const GSortedGElementPtrSet manager_elements = graph_engine_->getElements();
    for (GElementPtr element : manager_elements) {
        CGRAPH_ASSERT_NOT_NULL(element)

        status = element->fatProcessor(CFunctionType::DESTROY);
        CGRAPH_FUNCTION_CHECK_STATUS
        element->is_init_ = false;
    }

    CGRAPH_FUNCTION_END
}


CStatus GElementManager::run() {
    CGRAPH_FUNCTION_BEGIN
    status = graph_engine_->process();
    CGRAPH_FUNCTION_CHECK_STATUS

    status = afterRunCheck();
    CGRAPH_FUNCTION_END
}


CStatus GElementManager::afterRunCheck() {
    CGRAPH_FUNCTION_BEGIN

    const GSortedGElementPtrSet elements = graph_engine_->getElements();
    for (const GElementPtr element : elements) {
        if (!element->done_) {
            CGRAPH_RETURN_ERROR_STATUS("pipeline done status check failed...");
        }
    }

    CGRAPH_FUNCTION_END
}


CStatus GElementManager::add(GElementPtr element) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(element)

    status = graph_engine_->add(element);

    CGRAPH_FUNCTION_END
}


CBool GElementManager::find(GElementPtr element) const {
    if (nullptr == element || nullptr == graph_engine_) {
        return false;
    }

    return graph_engine_->find(element);
}


CStatus GElementManager::remove(GElementPtr element) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(element)

    status = graph_engine_->remove(element);
    CGRAPH_FUNCTION_END
}


CStatus GElementManager::clear() {
    CGRAPH_FUNCTION_BEGIN

    status = graph_engine_->clear();
    CGRAPH_FUNCTION_END
}


GElementManagerPtr GElementManager::setGElementRunTtl(CMSec ttl) {
    graph_engine_->setGElementRunTtl(ttl);
    return this;
}


GElementManagerPtr GElementManager::setScheduleStrategy(CInt strategy) {
    graph_engine_->setScheduleStrategy(strategy);
    return this;
}

CGRAPH_NAMESPACE_END
