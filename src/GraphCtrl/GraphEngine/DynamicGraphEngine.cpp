/***************************
@Author: YeShenYong
@Contact: 1050575224@qq.com
@File: DynamicGraphEngine.cpp
@Time: 2022/12/01 21:46 ??
@Desc: 
***************************/

#include "DynamicGraphEngine.h"

CGRAPH_NAMESPACE_BEGIN

DynamicGraphEngine::DynamicGraphEngine() {
    graph_counter_ = CGRAPH_SAFE_MALLOC_COBJECT(GraphEngineCounter)
}


DynamicGraphEngine::~DynamicGraphEngine() {
    CGRAPH_DELETE_PTR(graph_counter_)
}


CStatus DynamicGraphEngine::init() {
    CGRAPH_FUNCTION_BEGIN

    /** 首先判定，注册的element全部不为空 */
    for (auto element : manager_elements_) {
        CGRAPH_ASSERT_NOT_NULL(element)
    }

    CGRAPH_FUNCTION_END
}


CStatus DynamicGraphEngine::process() {
    CGRAPH_FUNCTION_BEGIN
    status = beforeRun();
    CGRAPH_FUNCTION_CHECK_STATUS

    status = run();
    CGRAPH_FUNCTION_END
}


CStatus DynamicGraphEngine::beforeRun() {
    CGRAPH_FUNCTION_BEGIN

    CInt end_size = 0;
    for (GElementPtr element : manager_elements_) {
        element->done_ = false;
        status = element->beforeRun();
        CGRAPH_FUNCTION_CHECK_STATUS
        if (element->run_before_.size() == 0) {
            end_size++;
        }
    }
    graph_counter_->setEnd(end_size);

    CGRAPH_FUNCTION_END
}


CStatus DynamicGraphEngine::run() {
    CGRAPH_FUNCTION_BEGIN
    asyncRun();
    wait();
    CGRAPH_FUNCTION_END
}


CVoid DynamicGraphEngine::asyncRun() {
    for (const auto& element : manager_elements_) {
        if (checkElement(element)) {
            runElementTask(element);
        }
    }
}


CVoid DynamicGraphEngine::wait() {
    graph_counter_->wait();
}


CVoid DynamicGraphEngine::runElementTask(GElementPtr element) {
    thread_pool_->commit(std::bind(&DynamicGraphEngine::runElement, this, element));
}


CVoid DynamicGraphEngine::runElement(GElementPtr element) {
    element->fatProcessor(CFunctionType::RUN);
    afterElementRun(element);
}


CVoid DynamicGraphEngine::afterElementRun(GElementPtr element) {
    element->done_ = true;
    // GElementPtr tutorial = nullptr;

    for(auto run_before_element : element->run_before_) {
        run_before_element->left_depend_--;
        if (run_before_element->left_depend_ > 0) continue;

        runElementTask(run_before_element);
        // if (tutorial) {
        //     runElementTask(run_before_element);
        // } else {
        //     tutorial = run_before_element;
        // }
    }
    // if (tutorial) {
    //     runElementTask(tutorial);
    // }
    
    if (element->run_before_.size() == 0) {
        graph_counter_->decreaseEnd();
    }
}


CBool DynamicGraphEngine::checkElement(GElementPtr element) {
    return element->dependence_.size() == 0 && !element->done_ && element->is_init_;
}

CGRAPH_NAMESPACE_END