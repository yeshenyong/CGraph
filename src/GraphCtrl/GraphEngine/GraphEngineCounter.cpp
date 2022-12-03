/***************************
@Author: YeShenYong
@Contact: 1050575224@qq.com
@File: GraphEngineCounter.cpp
@Time: 2022/12/01 21:02 ??
@Desc: 
***************************/

#include "GraphEngineCounter.h"

CGRAPH_NAMESPACE_BEGIN

void GraphEngineCounter::wait() {
    CGRAPH_UNIQUE_LOCK lock(lock_);
    while(end_count_ > 0) {
        cv_.wait(lock);
    }
}


CVoid GraphEngineCounter::setEnd(CInt end_count) {
    end_count_ = end_count;
}


CInt GraphEngineCounter::decreaseEnd() {
    CGRAPH_UNIQUE_LOCK lock(lock_);
    --end_count_;
    if (end_count_ <= 0) {
        cv_.notify_all();
    }
    return end_count_;
}


CGRAPH_NAMESPACE_END

