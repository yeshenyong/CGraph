import sys
import time
from CGraph import pyCGraph

class MyNodeA(pyCGraph.GNode):

    # @override
    def run(self):
        print("MyNodeA running...")
        return pyCGraph.CStatus()
    def __del__(self):
        print("delete A")

class MyNodeB(pyCGraph.GNode):

    # @override
    def run(self):
        print("MyNodeB running...")
        count = 0
        time.sleep(2)
        return pyCGraph.CStatus()

    def __del__(self):
        print("delete B")

class MyNodeC(pyCGraph.GNode):

    def run(self):
        print("MyNodeC running...")
        time.sleep(2)
        count = 0
        return pyCGraph.CStatus()
    def __del__(self):
        print("delete C")

class MyNodeD(pyCGraph.GNode):

    def run(self):
        print("MyNodeD running...")
        return pyCGraph.CStatus()
    def __del__(self):
        print("delete D")

nodeA = MyNodeA()
nodeB = MyNodeB()
nodeC = MyNodeC()
nodeD = MyNodeD()

print(type(nodeA))
print(MyNodeA.mro())

pipeline = pyCGraph.PipelineCreate()

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

pyCGraph.PipelineRemove(pipeline)