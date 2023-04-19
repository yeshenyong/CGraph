#!/usr/bin/env python
# -*- coding:utf-8 -*-

# under development...
import time
from pyCGraph import GNode,CStatus,GPipelinePy

class MyNodeA(GNode):
    def init(self):
        print("MyNodeA init...")
        return CStatus()

    def run(self):
        status = CStatus()
        print("MyNodeA running...")
        return status

    def destroy(self):
        print("MyNodeA destroy...")
        return CStatus()


class MyNodeB(GNode):
    def init(self):
        print("MyNodeB init...")
        return CStatus()
    # @override
    def run(self):
        print("MyNodeB running...")
        count = 0
        time.sleep(2)
        return CStatus()
    
    def destroy(self):
        print("MyNodeB destroy...")
        return CStatus()

class MyNodeC(GNode):
    def init(self):
        print("MyNodeC init...")
        return CStatus()

    def run(self):
        print("MyNodeC running...")
        time.sleep(2)
        count = 0
        return CStatus()
    
    def destroy(self):
        print("MyNodeC destroy...")
        return CStatus()

class MyNodeD(GNode):
    def init(self):
        print("MyNodeD init...")
        return CStatus()

    def run(self):
        print("MyNodeD running...")
        return CStatus()

    def destroy(self):
        print("MyNodeD destroy...")
        return CStatus()



if "__main__" == __name__:
    ppln = GPipelinePy()
    print("hellow")
    nodeA = MyNodeA()
    nodeB = MyNodeB()
    nodeC = MyNodeC()
    nodeD = MyNodeD()
    # print(GPipelinePy.mro())
    status = ppln.registerGElement(nodeA)
    status = ppln.registerGElement(nodeB, {nodeA})
    status = ppln.registerGElement(nodeC, {nodeA})
    status = ppln.registerGElement(nodeD, {nodeB, nodeC})

    status = ppln.init()
    status = ppln.run()
    status = ppln.destroy()

    # PipelineRemove(pipeline)

    # ppln.run()
    # ppln.destroy()
