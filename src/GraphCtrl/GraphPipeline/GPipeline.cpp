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


std::string GPipeline::dump() {
    // todo
    // CGRAPH_ASSERT_INIT(false)
    std::ostringstream oss;

    _dump(oss);
    // std::cout << oss.str() << std::endl;
    return oss.str();
}


CVoid GPipeline::_dump(std::ostream& oss) {
    oss << "digraph CGraph {\n";

    oss << "compound=true;\n";

    dumpGraph(oss);

    oss << "}\n";
}


CVoid GPipeline::dumpGraph(std::ostream& oss) {
    /* 预处理: 删除cluster 内的node */
    auto element_dump = preproessorNode(element_repository_);
    for (const auto& node : element_dump) {
        // todo 区分
        if (node) {
            // 针对不同Node 做区分
            dumpNode(node, oss);
        }
    }
}


CVoid GPipeline::dumpNode(GElementPtr element, std::ostream& oss) {
    oss << 'p' << element << "[label=\"";
    std::stringstream name;
    std::string nameStr = "";
    if (element->getName().empty()) {
        // oss << 'p' << element;
        name << 'p' << element;
    } else {
        // oss << element->getName();
        name << element->getName();
    }
    nameStr = name.str();
    element->setName(nameStr);

    printf("before rename = %s\n", element->getName().c_str());
    std::cout << "typeid(GElement) = " << typeid(*element).name() << std::endl;
    std::cout << "typeid(GRegion) = " << typeid(GRegion).name() << std::endl;
    std::cout << "typeid(GGroup) = " << typeid(GGroup).name() << std::endl;
    std::cout << "typeid(GCluster) = " << typeid(GCluster).name() << std::endl;

    if (isGroup(element)) {
        element->setName("cluster_" + element->getName());
    }

    oss << element->getName();
    oss << "\" ";
    printf("after rename = %s\n", element->getName().c_str());

    if (isGroup(element)) {
        oss << "shape=\"record\""
            << "color=\"red\"";
    }
    oss << "];\n";
    if (isGroup(element)) {
        dumpGroup(element, oss);
    }

    for (const auto& node : element->run_before_) {
        if (isGroup(element) && isGroup(node)) {
            oss << 'p' << element << " -> p" << node << "[ltail=" << element->getName() << " lhead=" << node->getName() << "];\n";
        } else if (isGroup(element) && !isGroup(node)) {
            oss << 'p' << element << " -> p" << node << "[ltail=" << element->getName() << "];\n";
        } else if (!isGroup(element) && isGroup(node)) {
            oss << 'p' << element << " -> p" << node << "[lhead=" << node->getName() << "];\n";
        } else {
            oss << 'p' << element << " -> p" << node << ";\n";
        }
    }
    if (element->loop_ > 1) {
        oss << 'p' << element << " -> p" << element << "[label=\"" << element->loop_ << "\"]" << ";\n";
    }
}


CVoid GPipeline::dumpGroup(GElementPtr element, std::ostream& oss) {
    /* cluster node 可见 */
    oss << "subgraph ";
    oss << element->getName();
    oss << " {\nlabel=\"";
    oss << element->getName();
    oss << "\";\n";
    oss << 'p' << element << "[label=\"" << element->getName() << "\" shape=point height=0 " << "];\n";
    oss << "color=blue;\n";
    size_t idx = 0;
    GElementPtr pre = nullptr;
    std::vector<GElementPtr> groupElement;
    if (isRegion(element)) {
        GRegionPtr region = dynamic_cast<GRegionPtr>(element);
        groupElement.insert(groupElement.end(), region->manager_->manager_elements_.begin(), region->manager_->manager_elements_.end());
    } else if (isCluster(element)) {
        GClusterPtr cluster = dynamic_cast<GClusterPtr>(element);
        groupElement.insert(groupElement.end(), cluster->group_elements_arr_.begin(), cluster->group_elements_arr_.end());
    }
    for (const auto& node : groupElement) {
        if (isGroup(node)) {
            dumpGroup(node, oss);
        } else {
            oss << 'p' << node << "[label=\"" << node->getName() << "\"];\n";
        }
        if (node->loop_ > 1) {
            oss << 'p' << node << " -> p" << node << "[label=\"" << node->loop_ << "\"]" << ";\n";
        }
        /**
         * 1. node to node
         * 2. group to node
         * 3. node to group
         * 4. group to group
        */
        printf("element %s, run_before_ size = %ld\n", node->getName().c_str(), node->run_before_.size());
        if (isRegion(element)) {
            for (const auto& run_before : node->run_before_) {
                if (isGroup(node) && isGroup(run_before)) {
                    oss << 'p' << node << " -> p" << run_before << "[ltail=" << run_before->getName() << " [lhead=" << run_before->getName() << "];\n";
                } else if (isGroup(node) && !isGroup(run_before)) {
                    oss << 'p' << node << " -> p" << run_before << "[ltail=" << run_before->getName() << "];\n";
                } else if (!isGroup(node) && isGroup(run_before)) {
                    oss << 'p' << node << " -> p" << run_before << "[lhead=" << run_before->getName() << "];\n";
                } else {
                    oss << 'p' << node << " -> p" << run_before << ";\n";
                }
            }
        } else if (isCluster(element)) {
            if (idx != 0) {
                if (isGroup(pre) && isGroup(node)) {
                    oss << 'p' << pre << " -> p" << node << "[ltail=" << node->getName() << " [lhead=" << node->getName() << "];\n";
                } else if (isGroup(pre) && !isGroup(node)) {
                    oss << 'p' << pre << " -> p" << node << "[ltail=" << node->getName() << "];\n";
                } else if (!isGroup(pre) && isGroup(node)) {
                    oss << 'p' << pre << " -> p" << node << "[lhead=" << node->getName() << "];\n";
                } else {
                    oss << 'p' << pre << " -> p" << node << ";\n";
                }
            }
        }
        idx++;
        pre = node;
    }
    oss << "}\n";
}


CBool GPipeline::isGroup(GElementPtr element) const {
    return typeid(*element) == typeid(GCluster) || typeid(*element) == typeid(GGroup) || typeid(*element) == typeid(GRegion);
}


CBool GPipeline::isRegion(GElementPtr element) const {
    return typeid(*element) == typeid(GRegion);
}


CBool GPipeline::isCluster(GElementPtr element) const {
    return typeid(*element) == typeid(GCluster);
}


GElementPtrSet GPipeline::preproessorNode(GElementPtrSet elementSet) {
    std::vector<GElementPtr> eraseElement;
    for (const auto& element : elementSet) {
        if (isCluster(element)) {
            auto clusterPtr = dynamic_cast<GClusterPtr>(element);
            eraseElement.insert(eraseElement.end(), clusterPtr->group_elements_arr_.begin(), clusterPtr->group_elements_arr_.end());
        } else if (isRegion(element)) {
            auto regionPtr = dynamic_cast<GRegionPtr>(element);
            eraseElement.insert(eraseElement.end(), regionPtr->manager_->manager_elements_.begin(), regionPtr->manager_->manager_elements_.end());
        }
    }
    // elementSet.erase(eraseElement.begin(), eraseElement.end());
    for (const auto& element : eraseElement) {
        elementSet.erase(element);
    }
    printf("after erase element\n");
    for (const auto& element : elementSet) {
        printf("element %s\n", element->getName().c_str());
    }
    return elementSet;
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
