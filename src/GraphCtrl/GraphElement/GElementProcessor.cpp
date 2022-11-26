/***************************
@Author: YeShenYong
@Contact: 1050575224@qq.com
@File: GElementProcessor.cpp
@Time: 2022/11/18 00:02 上午
@Desc: 动态图执行
***************************/

#include "GElementProcessor.h"

CGRAPH_NAMESPACE_BEGIN

GElementProcessor::GElementProcessor() {
    thread_pool_ = UThreadPoolSingleton::get();
    counter_ = CGRAPH_SAFE_MALLOC_COBJECT(GDynamicElementCount)
}


GElementProcessor::~GElementProcessor() {
    CGRAPH_DELETE_PTR(counter_)
}


CBool GElementProcessor::checkElement(GElementPtr element) {
    return element->dependence_.size() == 0 && !element->done_ && element->is_init_;
}


CVoid GElementProcessor::wait() {
    counter_->wait();
}


CVoid GElementProcessor::dynmaicAsyncRun() {
    for (const auto& element : manager_elements_) {
        if (checkElement(element)) {
            runElementTask(element, true);
        }
    }
    // todo: 监听线程防止一直超时
    // if (timeout_mode) {

    // }
}


CVoid GElementProcessor::runElementTask(GElementPtr element, CBool is_new_thread) {
    std::future<CStatus> status;
    if (is_new_thread) {
        thread_pool_->commit(std::bind(&GElementProcessor::runElement, this, element));
    } else {
        runElement(element);
    }
}


CVoid GElementProcessor::runElement(GElementPtr element) {
    printf("%s node is running\n", element->getName().c_str());
    element->fatProcessor(CFunctionType::RUN);
    afterElementRun(element);
}


CVoid GElementProcessor::afterElementRun(GElementPtr element) {
    element->done_ = true;
    GElementPtr tutorial = nullptr;

    for(auto run_before_element : element->run_before_) {
        run_before_element->left_depend_--;
        if (run_before_element->left_depend_ > 0) continue;

        if (tutorial) {
            runElementTask(run_before_element, true);
        } else {
            tutorial = run_before_element;
        }
    }
    if (tutorial) {
        runElementTask(tutorial, false);
    }
    
    if (element->run_before_.size() == 0) {
        counter_->decreaseEnd();
    }
}


CVoid GElementProcessor::setElements(GSortedGElementPtrSet manager_elements) {
    manager_elements_ = manager_elements;
    CINT end_count = 0;
    for (const auto& element : manager_elements_) {
        /* 用于动态图解析 */
        if (element->run_before_.size() == 0) {
            end_count++;
        }
    }
    setEnd(end_count);
}


CVoid GElementProcessor::setEnd(CINT end_count) {
    counter_->setEnd(end_count);
}

CGRAPH_NAMESPACE_END
