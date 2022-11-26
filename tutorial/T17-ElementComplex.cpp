/***************************
@Author: yeshenyong
@Contact: 1050575224@qq.com
@File: T17-ElementComplex.cpp
@Time: 2022/11/27 00:47 上午
@Desc: 本例主要演示，GElement 复杂逻辑
***************************/

#include "MyGNode/MyNode1.h"
#include "MyGNode/MyNode2.h"
#include "MyGNode/MyNode3.h"

using namespace CGraph;

void tutorial_simple() {
    /* 创建图对应的pipeline */
    GPipelinePtr pipeline = GPipelineFactory::create();

    /* 定义GElementPtr类型的变量 */
    GElementPtr a, b, c, e, f, g, h, k, l, m, n = nullptr;

    /**
     * 注册节点，其中MyNode1和MyNode2必须为GNode的子类，否则无法通过编译。
     * status+= 操作，可以用于记录链路异常问题
     * */
    CStatus status = pipeline->registerGElement<MyNode1>(&a, {}, "nodeA");    // 将名为nodeA，无执行依赖的node信息，注册入pipeline中
    status += pipeline->registerGElement<MyNode2>(&b, {a}, "nodeB");    // 将名为nodeB，依赖a执行的node信息，注册入pipeline中
    status += pipeline->registerGElement<MyNode1>(&c, {b}, "nodeC");    // 将名为nodeC，依赖b执行的node信息，注册入pipeline中
    status += pipeline->registerGElement<MyNode2>(&e, {c}, "nodeE");    // 将名为nodeE，依赖{c}执行的node信息，注册入pipeline中
    status += pipeline->registerGElement<MyNode2>(&f, {e}, "nodeF");    // 将名为nodeF，依赖{e}执行的node信息，注册入pipeline中
    status += pipeline->registerGElement<MyNode2>(&g, {f}, "nodeG");    // 将名为nodeG，依赖{f}执行的node信息，注册入pipeline中
    status += pipeline->registerGElement<MyNode2>(&h, {f}, "nodeH");    // 将名为nodeH，依赖{f}执行的node信息，注册入pipeline中
    status += pipeline->registerGElement<MyNode2>(&k, {g, h}, "nodeK");    // 将名为nodeK，依赖{g, h}执行的node信息，注册入pipeline中
    status += pipeline->registerGElement<MyNode2>(&l, {c}, "nodeL");    // 将名为nodeL，依赖{c}执行的node信息，注册入pipeline中
    status += pipeline->registerGElement<MyNode3>(&m, {l}, "nodeM");    // 将名为nodeM，依赖{l}执行的node信息，注册入pipeline中
    status += pipeline->registerGElement<MyNode2>(&n, {k, m}, "nodeN");    // 将名为nodeN，依赖{k, m}执行的node信息，注册入pipeline中
    if (!status.isOK()) {
        return;    // 使用时，请对所有CGraph接口的返回值做判定。今后tutorial例子中省略该操作。
    }

    /* 图信息初始化，准备开始计算 */
    status = pipeline->setDynamicModule();
    status = pipeline->init();

    /* 运行流图信息。初始化后，支持多次循环计算 */
    for (int i = 0; i < 3; i++) {
        status = pipeline->run();
        CGRAPH_ECHO("==== tutorial_simple, loop : [%d], and run status = [%d].", i + 1, status.getCode());
    }

    /* 图信息逆初始化，准备结束计算 */
    status = pipeline->destroy();
    GPipelineFactory::remove(pipeline);
}

int main () {
    tutorial_simple();
    return 0;
}
