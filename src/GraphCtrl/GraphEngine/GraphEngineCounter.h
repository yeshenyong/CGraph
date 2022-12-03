/***************************
@Author: YeShenYong
@Contact: 1050575224@qq.com
@File: GraphEngineCounter.h
@Time: 2022/12/01 21:02 ����
@Desc: 
***************************/

#ifndef CGRAPH_GRAPHENGINECOUNTER_H
#define CGRAPH_GRAPHENGINECOUNTER_H

#include <mutex>
#include <memory>
#include <condition_variable>

#include "../../CBasic/CBasicInclude.h"
#include "../GraphParam/GParamInclude.h"

CGRAPH_NAMESPACE_BEGIN

class GraphEngineCounter : public CObject {

public:
    explicit GraphEngineCounter() = default;

    virtual CStatus run() override {
        CGRAPH_NO_SUPPORT
    }

    /**
     * �ȴ�����
     * @param
     * @ret
     */
    CVoid wait();

    /**
     * д����Ϣ
     * @param end_count
     * @return
     */
    CVoid setEnd(CInt end_count);

    /**
     * ����ĩβ�ڵ�����
     * @param
     * @return
     */
    CINT decreaseEnd();

    virtual ~GraphEngineCounter() = default;

private:
    mutable std::mutex lock_;
    std::condition_variable cv_;
    CINT end_count_ { 0 };

};

using GraphEngineCounterPtr = GraphEngineCounter *;

CGRAPH_NAMESPACE_END

#endif // CGRAPH_GRAPHENGINECOUNTER_H