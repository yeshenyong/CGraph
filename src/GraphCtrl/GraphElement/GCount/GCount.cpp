/***************************
@Author: YeShenYong
@Contact: 1050575224@qq.com
@File: GCount.cpp
@Time: 2022/11/19 01:02 上午
@Desc: 
***************************/
#include "GCount.h"

CGRAPH_NAMESPACE_BEGIN

void GDynamicElementCount::wait() {
    CGRAPH_UNIQUE_LOCK lock(lock_);
    while(end_count_ > 0) {
        cv_.wait(lock);
    }
}


CVoid GDynamicElementCount::setEnd(CINT end_count) {
    end_count_ = end_count;
}


CINT GDynamicElementCount::decreaseEnd() {
    CGRAPH_UNIQUE_LOCK lock(lock_);
    --end_count_;
    if (end_count_ <= 0) {
        cv_.notify_all();
    }
    return end_count_;
}


CINT GDynamicElementCount::getEndCount() {
    CGRAPH_UNIQUE_LOCK lock(lock_);
    return end_count_;
}


CGRAPH_NAMESPACE_END

