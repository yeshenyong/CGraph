/***************************
@Author: YeShenYong
@Contact: 1050575224@qq.com
@File: GElementProcessor.h
@Time: 2022/11/18 20:02 下午
@Desc: 
***************************/

#ifndef CGRAPH_GELEMENTPROCESSOR_H
#define CGRAPH_GELEMENTPROCESSOR_H

#include "GElement.h"
#include "GCount/GCount.h"
#include "GElementSorter.h"

CGRAPH_NAMESPACE_BEGIN

class GElementProcessor : public GraphObject {

public:
    explicit GElementProcessor();
    ~GElementProcessor();

    CStatus run() override {
        CGRAPH_NO_SUPPORT
    }

    /**
     * 等待唤醒
     * @param
     * @return
     */
    CVoid wait();

    /**
     * 节点运行检验
     * @param element
     * @return
     */
    CBool checkElement(GElementPtr element);

    /**
     * 动态图运行
     * @param
     * @return
     */
    CVoid dynmaicAsyncRun();

    /**
     * element 运行调用
     * @param element
     * @param is_new_thread
     * @return
     */
    CVoid runElementTask(GElementPtr element, CBool is_new_thread = false);

    /**
     * element 运行
     * @param element
     * @return
     */
    CVoid runElement(GElementPtr element);

    /**
     * element 运行完成处理
     * @param element
     * @return
     */
    CVoid afterElementRun(GElementPtr element);

    /**
     * 设置动态图运行元素集
     * @param manager_elements
     * @return
     */
    CVoid setElements(GSortedGElementPtrSet manager_elements);

    /**
     * 设置动态图末尾节点数量
     * @param end_count
     * @return
     */
    CVoid setEnd(CINT end_count);

protected:
    GSortedGElementPtrSet manager_elements_;
    GDynamicElementCountPtr counter_;
    UThreadPoolPtr thread_pool_ { nullptr };         // 用于执行的线程池信息

};

using GElementProcessorPtr = GElementProcessor *;

CGRAPH_NAMESPACE_END

#endif // CGRAPH_GELEMENTPROCESSOR_H