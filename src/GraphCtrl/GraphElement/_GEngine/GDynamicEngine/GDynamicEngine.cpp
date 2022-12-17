/***************************
@Author: YeShenYong
@Contact: 1050575224@qq.com
@File: GDynamicEngine.h
@Time: 2022/12/16 22:46 下午
@Desc: 
***************************/

#include "GDynamicEngine.h"

CGRAPH_NAMESPACE_BEGIN

CStatus GDynamicEngine::setUp(const GSortedGElementPtrSet& elements) {
    CGRAPH_FUNCTION_BEGIN

    end_size_ = 0;
    for (GElementPtr element : elements) {
        if (element->run_before_.empty()) {
            end_size_++;
        }
    }
    manager_elements_ = elements;

    CGRAPH_FUNCTION_END
}


CStatus GDynamicEngine::run() {
    CGRAPH_FUNCTION_BEGIN

    status = beforeRun();
    CGRAPH_FUNCTION_CHECK_STATUS

    asyncRun();

    wait();
    CGRAPH_FUNCTION_END
}


CStatus GDynamicEngine::afterRunCheck() {
    CGRAPH_FUNCTION_BEGIN

    for (const GElementPtr element : manager_elements_) {
        if (!element->done_) {
            CGRAPH_RETURN_ERROR_STATUS("pipeline done status check failed...");
        }
    }

    CGRAPH_FUNCTION_END
}


CVoid GDynamicEngine::asyncRun() {

    execute_end_size_ = end_size_;
    for (const auto& element : manager_elements_) {
        if (element->dependence_.empty() && !element->done_) {
            runElementTask(element);
        }
    }
}


CStatus GDynamicEngine::beforeRun() {
    CGRAPH_FUNCTION_BEGIN

    for (GElementPtr element : manager_elements_) {
        element->done_ = false;
        status = element->beforeRun();
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    CGRAPH_FUNCTION_END
}


CVoid GDynamicEngine::runElementTask(GElementPtr element) {
    thread_pool_->commit(std::bind(&GDynamicEngine::runElement, this, element));
}


CVoid GDynamicEngine::runElement(GElementPtr element) {
    element->fatProcessor(CFunctionType::RUN);
    afterElementRun(element);
}


CVoid GDynamicEngine::afterElementRun(GElementPtr element) {
    element->done_ = true;

    for(auto run_before_element : element->run_before_) {
        run_before_element->left_depend_--;
        if (run_before_element->left_depend_ > 0) continue;

        runElementTask(run_before_element);
    }

    if (element->run_before_.empty()) {
        this->decreaseEnd();
    }
}


CVoid GDynamicEngine::wait() {
    CGRAPH_UNIQUE_LOCK lock(lock_);

    while(execute_end_size_ > 0) {
        cv_.wait(lock);
    }

}


CUint GDynamicEngine::decreaseEnd() {
    CGRAPH_UNIQUE_LOCK lock(lock_);
    --execute_end_size_;
    if (execute_end_size_ <= 0) {
        cv_.notify_one();
    }
    return execute_end_size_;
}

CGRAPH_NAMESPACE_END
