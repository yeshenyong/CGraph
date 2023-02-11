/*
 * @Author: 1050575224@qq.com 1050575224@qq.com
 * @Date: 2023-02-11 09:40:57
 * @LastEditors: 1050575224@qq.com 1050575224@qq.com
 * @LastEditTime: 2023-02-11 14:34:04
 * @FilePath: /CGraph/src/GraphCtrl/GraphElement/GGroup/GRegion/GRegion.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GRegion.h
@Time: 2021/6/1 10:14 下午
@Desc: 实现多个element，根据依赖关系执行的功能
***************************/


#ifndef CGRAPH_GREGION_H
#define CGRAPH_GREGION_H

#include "../GGroup.h"
#include "../../GElementManager.h"

CGRAPH_NAMESPACE_BEGIN

class GRegion : public GGroup {
public:
    GRegion* setGEngineType(GEngineType type);

protected:
    explicit GRegion();
    ~GRegion() override;

    CStatus init() final;
    CStatus run() final;
    CStatus destroy() final;

    CStatus addElement(GElementPtr element) final;

    /**
     * graphviz dump 逻辑
     * @param oss
     * @return
    */
    CVoid dump(std::ostream& oss) final;

private:
    GElementManagerPtr manager_ = nullptr;    // region 内部通过 manager来管理其中的 element 信息

    CGRAPH_NO_ALLOWED_COPY(GRegion)

    friend class GPipeline;
    friend class UAllocator;
};

using GRegionPtr = GRegion *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GREGION_H
