/***************************
@Author: YeShenYong
@Contact: 1050575224@qq.com
@File: GPipelineFactoryPy.cpp
@Time: 2023/4/2 10:15 下午
@Desc: 
***************************/

#include "GPipelineFactoryPy.h"

CGRAPH_NAMESPACE_BEGIN

GPipelinePyPtrList GPipelineFactoryPy::s_pipeline_list_;
std::mutex GPipelineFactoryPy::s_lock_;

GPipelinePyPtr GPipelineFactoryPy::create() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_LOCK_GUARD lock(s_lock_);

    auto pipeline = CGRAPH_SAFE_MALLOC_COBJECT(GPipelinePy)
    s_pipeline_list_.emplace_back(pipeline);
    return pipeline;
}


CStatus GPipelineFactoryPy::remove(GPipelinePyPtr pipeline) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(pipeline)

    CGRAPH_LOCK_GUARD lock(s_lock_);
    s_pipeline_list_.remove(pipeline);
    CGRAPH_DELETE_PTR(pipeline)

    CGRAPH_FUNCTION_END
}


CStatus GPipelineFactoryPy::clear() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_LOCK_GUARD lock(s_lock_);

    for (GPipelinePyPtr pipeline : GPipelineFactoryPy::s_pipeline_list_) {
        CGRAPH_DELETE_PTR(pipeline)
    }

    s_pipeline_list_.clear();
    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END
