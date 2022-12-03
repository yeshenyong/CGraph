/***************************
@Author: YeShenYong
@Contact: 1050575224@qq.com
@File: StaticGraphEngine.h
@Time: 2022/12/01 21:42 ����
@Desc: 
***************************/

#ifndef CGRAPH_STATICGRAPHENGINE_H
#define CGRAPH_STATICGRAPHENGINE_H

#include "GraphEngine.h"

CGRAPH_NAMESPACE_BEGIN

class StaticGraphEngine : public GraphEngine {
public:
    explicit StaticGraphEngine() = default;

    virtual CStatus init() final;

    virtual CStatus process() final;

    virtual CStatus run() final;

    /**
     * �ж���Щ�ڵ��ǿ��Էֵ�һ��cluster�е�
     * @return
     */
    CStatus preRunCheck();

    /**
     * �����еĽڵ㣬�ַ���para_cluster_arrs_�У����е�ʱ��ʹ�á�
     * @return
     */
    CStatus analyse();

    virtual ~StaticGraphEngine() = default;

private:
    ParaWorkedClusterArrs para_cluster_arrs_;                   // ���Բ��е�cluster����
};

using StaticGraphEnginePtr = StaticGraphEngine *;

CGRAPH_NAMESPACE_END

#endif // CGRAPH_STATICGRAPHENGINE_H