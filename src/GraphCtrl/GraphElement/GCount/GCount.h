/***************************
@Author: YeShenYong
@Contact: 1050575224@qq.com
@File: GCount.h
@Time: 2022/11/19 23:02 下午
@Desc: 
***************************/

#ifndef CGRAPH_GCOUNT_H
#define CGRAPH_GCOUNT_H

#include <mutex>
#include <memory>
#include <condition_variable>

#include "../../GraphParam/GParamInclude.h"
#include "../../../CBasic/CBasicInclude.h"

CGRAPH_NAMESPACE_BEGIN

class GDynamicElementCount : public CObject {

public:
    /**
     * 等待唤醒
     * @param
     * @return
     */
    void wait();

    CStatus run() override {
        CGRAPH_NO_SUPPORT
    }

    /**
     * 写入信息
     * @param end_count
     * @return
     */
    CVoid setEnd(CINT end_count);

    /**
     * 减少末尾节点数量
     * @param
     * @return
     */
    CINT decreaseEnd();

    // CBOOL decreaseZeroEnd();
    /**
     * 获取末尾节点数量
     * @param
     * @return
     */
    CINT getEndCount();

private:
    mutable std::mutex lock_;
    std::condition_variable cv_;
    CINT end_count_ { 0 };

};

using GDynamicElementCountPtr = GDynamicElementCount *;

CGRAPH_NAMESPACE_END

#endif // CGRAPH_GCOUNT_H