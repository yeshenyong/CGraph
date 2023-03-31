#include "GPipeline.h"

CGRAPH_NAMESPACE_BEGIN

CStatus GPipeline::registerPyGElement(GElementPtr elementRef,
                                      const GElementPtrSet &dependElements,
                                      const std::string &name,
                                      CSize loop) {

    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)

    // CGRAPH_ASSERT_NOT_NULL(elementRef);
    status = (elementRef)->setElementInfo(dependElements, name, loop,
                                           this->param_manager_,
                                           this->event_manager_);

    CGRAPH_FUNCTION_CHECK_STATUS

    status = element_manager_->add(dynamic_cast<GElementPtr>(elementRef));
    CGRAPH_FUNCTION_CHECK_STATUS
    element_repository_.insert(elementRef);

    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END