/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: Graphic.cpp
@Time: 2021/6/2 10:15 下午
@Desc: 
***************************/

#include "GPipeline.h"

CGRAPH_NAMESPACE_BEGIN

GPipeline::GPipeline() {
    thread_pool_ = UThreadPoolSingleton::get();
    element_manager_ = CGRAPH_SAFE_MALLOC_COBJECT(GElementManager)
    param_manager_ = CGRAPH_SAFE_MALLOC_COBJECT(GParamManager)
    daemon_manager_ = CGRAPH_SAFE_MALLOC_COBJECT(GDaemonManager)
    event_manager_ = CGRAPH_SAFE_MALLOC_COBJECT(GEventManager)
    is_init_ = false;
}


GPipeline::~GPipeline() {
    CGRAPH_DELETE_PTR(daemon_manager_)
    CGRAPH_DELETE_PTR(element_manager_)
    CGRAPH_DELETE_PTR(param_manager_)
    CGRAPH_DELETE_PTR(event_manager_)

    // 结束的时候，清空所有创建的节点信息。所有节点信息，仅在这一处释放
    for (GElementPtr element : element_repository_) {
        CGRAPH_DELETE_PTR(element)
    }
}


CStatus GPipeline::init() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)    // 必须是非初始化的状态下，才可以初始化。反之同理
    CGRAPH_ASSERT_NOT_NULL(thread_pool_)
    CGRAPH_ASSERT_NOT_NULL(element_manager_)
    CGRAPH_ASSERT_NOT_NULL(param_manager_)
    CGRAPH_ASSERT_NOT_NULL(daemon_manager_)
    CGRAPH_ASSERT_NOT_NULL(event_manager_)

    status += param_manager_->init();
    status += event_manager_->init();
    status += element_manager_->init();
    status += daemon_manager_->init();    // daemon的初始化，需要晚于所有element的初始化
    CGRAPH_FUNCTION_CHECK_STATUS

    is_init_ = true;
    CGRAPH_FUNCTION_END
}


CStatus GPipeline::run() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(true)
    CGRAPH_ASSERT_NOT_NULL(element_manager_)
    CGRAPH_ASSERT_NOT_NULL(param_manager_)

    status = element_manager_->run();
    CGRAPH_FUNCTION_CHECK_STATUS

    param_manager_->reset();
    CGRAPH_FUNCTION_END
}


CStatus GPipeline::destroy() {
    CGRAPH_FUNCTION_BEGIN

    CGRAPH_ASSERT_INIT(true)
    CGRAPH_ASSERT_NOT_NULL(element_manager_)
    CGRAPH_ASSERT_NOT_NULL(param_manager_)
    CGRAPH_ASSERT_NOT_NULL(daemon_manager_)
    CGRAPH_ASSERT_NOT_NULL(event_manager_)

    status += event_manager_->destroy();
    status += daemon_manager_->destroy();
    status += element_manager_->destroy();
    status += param_manager_->destroy();
    CGRAPH_FUNCTION_CHECK_STATUS

    is_init_ = false;
    CGRAPH_FUNCTION_END
}


CStatus GPipeline::process(CSize runTimes) {
    CGRAPH_FUNCTION_BEGIN
    status = init();
    CGRAPH_FUNCTION_CHECK_STATUS

    while (runTimes-- > 0) {
        status = run();
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    status = destroy();
    CGRAPH_FUNCTION_END
}


std::string GPipeline::dump() const {
    // todo
    // CGRAPH_ASSERT_INIT(false)
    std::ostringstream oss;

    _dump(oss);
    // std::cout << oss.str() << std::endl;
    return oss.str();
}


CVoid GPipeline::_dump(std::ostream& oss) const {
    oss << "digraph CGraph {\n";

    dumpGraph(oss);

    oss << "}\n";
}


CVoid GPipeline::dumpGraph(std::ostream& oss) const {
    for (const auto& node : element_repository_) {
        // todo 区分
        if (node) {
            // 针对不同Node 做区分
            dumpNode(node, oss);
        }
    }
}


CVoid GPipeline::dumpNode(GElementPtr element, std::ostream& oss) const {
    oss << 'p' << element << "[label=\"";
    std::stringstream name;
    std::string nameStr = "";
    if (element->getName().empty()) {
        oss << 'p' << element;
        name << 'p' << element;
    } else {
        oss << element->getName();
        name << element->getName();
    }
    nameStr = name.str();
    oss << "\" ";

    if (typeid(*element) == typeid(GCluster) || typeid(*element) == typeid(GGroup)) {
        oss << "shape=\"record\""
            << "color=\"red\"";
    }
    oss << "];\n";
    if (typeid(*element) == typeid(GCluster) || typeid(*element) == typeid(GGroup)) {
        oss << "subgraph ";
        oss << nameStr;
        oss << " {\nlabel=\"";
        oss << nameStr;
        oss << "\";\n";
        size_t idx = 0;
        GElementPtr pre = nullptr;
        GClusterPtr cluster = dynamic_cast<GClusterPtr>(element);
        for (const auto& node : cluster->group_elements_arr_) {
            if (0 != idx) {
                oss << 'p' << pre << " -> p" << node << ";\n";
            }
            idx++;
            pre = node;
        }
        oss << "}\n";
    }

    for (const auto& node : element->run_before_) {
        oss << 'p' << element << " -> p" << node << ";\n";
    }
    if (element->loop_ > 1) {
        oss << 'p' << element << " -> p" << element << "[label=\"" << element->loop_ << "\"]" << ";\n";
    }
}


GPipelinePtr GPipeline::setGElementRunTtl(CMSec ttl) {
    CGRAPH_ASSERT_INIT_RETURN_NULL(false)
    CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(element_manager_)
    CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(element_manager_->engine_)

    // 在element_manager中区执行信息了，所以ttl放到
    element_manager_->engine_->element_run_ttl_ = ttl;
    return this;
}


GPipelinePtr GPipeline::setGEngineType(GEngineType type) {
    CGRAPH_ASSERT_INIT_RETURN_NULL(false)
    CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(element_manager_)

    element_manager_->setEngineType(type);
    return this;
}

CGRAPH_NAMESPACE_END
