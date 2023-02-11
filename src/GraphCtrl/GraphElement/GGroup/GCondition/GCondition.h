/*
 * @Author: 1050575224@qq.com 1050575224@qq.com
 * @Date: 2023-02-11 09:40:40
 * @LastEditors: 1050575224@qq.com 1050575224@qq.com
 * @LastEditTime: 2023-02-11 14:35:06
 * @FilePath: /CGraph/src/GraphCtrl/GraphElement/GGroup/GCondition/GCondition.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GCondition.h
@Time: 2021/6/19 5:00 下午
@Desc: 实现在多个element中，选择一个执行的功能
***************************/

#ifndef CGRAPH_GCONDITION_H
#define CGRAPH_GCONDITION_H

#include "../GGroup.h"

CGRAPH_NAMESPACE_BEGIN

class GCondition : public GGroup {
public:
    explicit GCondition();

protected:
    /**
     * 计算需要返回第n个信息
     * 执行最后一个，返回-1即可。
     * 超出-1和size之间的范围，则不执行
     * @return
     */
    virtual CIndex choose() = 0;

    /**
     * 获取当前condition组内部元素的个数
     * @return
     */
    CSize getRange() const;

    /**
     * graphviz dump 逻辑
     * @param oss
     * @return
    */
    CVoid dump(std::ostream& oss) final;

private:
    CStatus run() override;

    CStatus addElement(GElementPtr element) override;

    friend class GPipeline;
};

using GConditionPtr = GCondition *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GCONDITION_H
