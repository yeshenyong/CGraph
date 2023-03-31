import sys
from time import time
from cgraph import *

class MyNodeA(GNode):

    def run(self):
        print("MyNodeA running...")
        # time.sleep(1)
        return CStatus()

class MyNodeB(GNode):

    def run(self):
        print("MyNodeB running...")
        count = 0
        for i in range(0, 1000000000):
            count = i * i
        # time.sleep(1)
        return CStatus()

class MyNodeC(GNode):

    def run(self):
        print("MyNodeC running...")
        # time.sleep(1)
        count = 0
        for i in range(0, 1000000000):
            count = i * i
        return CStatus()

class MyNodeD(GNode):

    def run(self):
        print("MyNodeD running...")
        # time.sleep(1)
        return CStatus()

nodeA = MyNodeA()
nodeB = MyNodeB()
nodeC = MyNodeC()
nodeD = MyNodeD()

A_child = cast_set()
# A_child.add(1)
print(MyNodeA.mro())
print(type(A_child))

B_child = cast_set()
B_child.add(nodeA)

C_child = cast_set()
C_child.add(nodeA)

D_child = cast_set()
D_child.add(nodeB)
D_child.add(nodeC)

load_set(A_child)
load_set(B_child)
load_set(C_child)
load_set(D_child)

print("A_child: {}".format(A_child))
print("B_child: {}".format(B_child))
print("C_child: {}".format(C_child))
print("D_child: {}".format(D_child))

print(type(nodeA))
print(MyNodeA.mro())

pipeline = PipelineCreate()

status = pipeline.registerGElement(nodeA, A_child)
status = pipeline.registerGElement(nodeB, B_child)
status = pipeline.registerGElement(nodeC, C_child)
status = pipeline.registerGElement(nodeD, D_child)

print(status)

if not status.isOK():
    print(status)
    print("status is not OK")


status = pipeline.init()

print(status.isOK())

status = pipeline.run()
print("==== tutorial_simple, loop : [%d], and run status = [%d].", 1, status.getCode())

# status = pipeline.destroy()

PipelineRemove(pipeline)