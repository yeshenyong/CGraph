/***************************
@Author: YeShenYong
@Contact: 1050575224@qq.com
@File: GPipelineFactoryPy.h
@Time: 2023/4/2 10:15 下午
@Desc: 
***************************/

#ifndef CGRAPH_GPIPELINEFACTORYPY_H
#define CGRAPH_GPIPELINEFACTORYPY_H

#include <list>
#include <mutex>

#include "GPipelinePy.h"
#include "../GPipelineObject.h"

CGRAPH_NAMESPACE_BEGIN

class GPipelineFactoryPy : public GPipelineObject {
public:
    /**
     * 创建一个pipeline信息
     * @return
     */
    static GPipelinePyPtr create();

    /**
     * 销毁一个pipeline信息
     * @return
     */
    static CStatus remove(GPipelinePyPtr pipeline);

    /**
     * 清空所有的pipeline信息
     */
    static CStatus clear();

private:
    static GPipelinePyPtrList s_pipeline_list_;    // 记录所有的
    static std::mutex s_lock_;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GPIPELINEFACTORY_H
