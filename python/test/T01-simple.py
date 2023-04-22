import sys
import time
from pyCGraph import GNode,CStatus,GPipelinePy

class MyNodeA(GNode):
    def init(self):
        print("MyNodeA init...")
        return CStatus()
    def run(self):
        print("MyNodeA running...")
        return CStatus()
    def destroy(self):
        print("MyNodeA destroy...")
        return CStatus()

class MyNodeB(GNode):
    def init(self):
        print("MyNodeB init...")
        return CStatus()
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

nodeA = MyNodeA()
nodeB = MyNodeB()
nodeC = MyNodeC()
nodeD = MyNodeD()

print(type(nodeA))
print(MyNodeA.mro())

pipeline = GPipelinePy()

status = pipeline.registerGElement(nodeA)
status = pipeline.registerGElement(nodeB, {nodeA}, "nodeB", 2)
status = pipeline.registerGElement(nodeC, {nodeA}, "nodeC")
status = pipeline.registerGElement(nodeD, {nodeB, nodeC}, "nodeD")

print(status)

if not status.isOK():
    print("status is not OK")


status = pipeline.init()

print(pipeline.dump())

print(status.isOK())

status = pipeline.run()
print("==== tutorial_simple, loop : [%d], and run status = [%d].", 1, status.getCode())

status = pipeline.destroy()