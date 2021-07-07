/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: CThreadObject.h
@Time: 2021/7/2 10:39 下午
@Desc: 
***************************/

#ifndef CGRAPH_UTHREADOBJECT_H
#define CGRAPH_UTHREADOBJECT_H

#include "../UtilsDefine.h"
#include "../../CObject/CObject.h"

class UThreadObject : public CObject {

protected:
    /**
     * 部分thread中的算子，可以不实现run方法
     * @return
     */
    CSTATUS run() override {
        CGRAPH_NO_SUPPORT
    }
};

#endif //CGRAPH_UTHREADOBJECT_H
