/***************************
@Author: YeShenYong
@Contact: 1050575224@qq.com
@File: GraphEngine.h
@Time: 2022/12/01 21:02 ����
@Desc: 
***************************/

#ifndef CGRAPH_GRAPHENGINE_H
#define CGRAPH_GRAPHENGINE_H

#include "../GraphObject.h"
#include "../../CBasic/CBasicInclude.h"
#include "../GraphElement/GElement.h"
#include "../GraphElement/GElementSorter.h"
#include "../GraphElement/GGroup/GCluster/GCluster.h"

CGRAPH_NAMESPACE_BEGIN

class GraphEngine : public GraphObject, public GraphManager<GElement> {

public:
    /**
     * Ĭ�Ϲ��캯��
     */
    explicit GraphEngine();

    virtual CStatus init() override {
        CGRAPH_EMPTY_FUNCTION
    }

    virtual CStatus run() override {
        CGRAPH_EMPTY_FUNCTION
    }

    virtual CStatus process() {
        CGRAPH_EMPTY_FUNCTION
    }

    /**
     * ���һ��element
     * @param element
     * @return
     */
    virtual CStatus add(GElementPtr element) final;

    /**
     * ɾ��һ��element
     * @param element
     * @return
     */
    virtual CStatus remove(GElementPtr element) final;

    /**
     * ����һ��element
     * @param element
     * @return
     */
    virtual CBool find(GElementPtr element) const final;

    /**
     * �������element������ʵ�ֵ�����
     * @return
     */
    virtual CStatus clear() final;

    const GSortedGElementPtrSet getElements();

    CVoid setGElementRunTtl(CMSec ttl);

    CVoid setScheduleStrategy(CInt strategy);

    virtual ~GraphEngine() = default;

protected:
    GSortedGElementPtrSet manager_elements_;                    // ����ڵ���Ϣ������
    UThreadPoolPtr thread_pool_ { nullptr };                    // �ڲ�ִ�е��̳߳�
    CMSec element_run_ttl_ = CGRAPH_DEFAULT_ELEMENT_RUN_TTL;    // �����ڵ������������
    int schedule_strategy_;                                     // ���Ȳ���
};

using GraphEnginePtr = GraphEngine *;

CGRAPH_NAMESPACE_END

#endif // CGRAPH_GRAPHENGINE_H