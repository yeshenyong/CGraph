/***************************
@Author: yeshenyong
@Contact: 1050575224@qq.com
@File: T17-ElementComplex.cpp
@Time: 2022/11/27 00:47 ����
@Desc: ������Ҫ��ʾ��GElement �����߼�
***************************/

#include "MyGNode/MyNode1.h"
#include "MyGNode/MyNode2.h"
#include "MyGNode/MyNode3.h"

using namespace CGraph;

void tutorial_simple() {
    /* ����ͼ��Ӧ��pipeline */
    GPipelinePtr pipeline = GPipelineFactory::create();

    /* ����GElementPtr���͵ı��� */
    GElementPtr a, b, c, e, f, g, h, k, l, m, n = nullptr;

    /**
     * ע��ڵ㣬����MyNode1��MyNode2����ΪGNode�����࣬�����޷�ͨ�����롣
     * status+= �������������ڼ�¼��·�쳣����
     * */
    CStatus status = pipeline->registerGElement<MyNode1>(&a, {}, "nodeA");    // ����ΪnodeA����ִ��������node��Ϣ��ע����pipeline��
    status += pipeline->registerGElement<MyNode2>(&b, {a}, "nodeB");    // ����ΪnodeB������aִ�е�node��Ϣ��ע����pipeline��
    status += pipeline->registerGElement<MyNode1>(&c, {b}, "nodeC");    // ����ΪnodeC������bִ�е�node��Ϣ��ע����pipeline��
    status += pipeline->registerGElement<MyNode2>(&e, {c}, "nodeE");    // ����ΪnodeE������{c}ִ�е�node��Ϣ��ע����pipeline��
    status += pipeline->registerGElement<MyNode2>(&f, {e}, "nodeF");    // ����ΪnodeF������{e}ִ�е�node��Ϣ��ע����pipeline��
    status += pipeline->registerGElement<MyNode2>(&g, {f}, "nodeG");    // ����ΪnodeG������{f}ִ�е�node��Ϣ��ע����pipeline��
    status += pipeline->registerGElement<MyNode2>(&h, {f}, "nodeH");    // ����ΪnodeH������{f}ִ�е�node��Ϣ��ע����pipeline��
    status += pipeline->registerGElement<MyNode2>(&k, {g, h}, "nodeK");    // ����ΪnodeK������{g, h}ִ�е�node��Ϣ��ע����pipeline��
    status += pipeline->registerGElement<MyNode2>(&l, {c}, "nodeL");    // ����ΪnodeL������{c}ִ�е�node��Ϣ��ע����pipeline��
    status += pipeline->registerGElement<MyNode3>(&m, {l}, "nodeM");    // ����ΪnodeM������{l}ִ�е�node��Ϣ��ע����pipeline��
    status += pipeline->registerGElement<MyNode2>(&n, {k, m}, "nodeN");    // ����ΪnodeN������{k, m}ִ�е�node��Ϣ��ע����pipeline��
    if (!status.isOK()) {
        return;    // ʹ��ʱ���������CGraph�ӿڵķ���ֵ���ж������tutorial������ʡ�Ըò�����
    }

    /* ͼ��Ϣ��ʼ����׼����ʼ���� */
    status = pipeline->setDynamicModule();
    status = pipeline->init();

    /* ������ͼ��Ϣ����ʼ����֧�ֶ��ѭ������ */
    for (int i = 0; i < 3; i++) {
        status = pipeline->run();
        CGRAPH_ECHO("==== tutorial_simple, loop : [%d], and run status = [%d].", i + 1, status.getCode());
    }

    /* ͼ��Ϣ���ʼ����׼���������� */
    status = pipeline->destroy();
    GPipelineFactory::remove(pipeline);
}

int main () {
    tutorial_simple();
    return 0;
}
