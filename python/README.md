# CGraph Python

因为`CGraph` 本身是C++ 编写的图执行引擎，所以如果Python 想要调用CGraph，需要将CGraph 源码编译成 `.whl` 库，以提供使用

我们以 `ubuntu 20.04`  based on `gcc version 12.2.0` 为例，来说明源码编译的步骤



### 安装依赖库

```shell
sudo apt-get -y update
python -m pip install --upgrade pip setuptools wheel

git clone https://github.com/ChunelFeng/CGraph
cd CGraph
cd python
sudo ./install_dependencies.sh
```



### 编译

```sh
python setup.py bdist_wheel
```



### 安装python wheel 包

```sh
pip install dist/your_wheel_name.whl
```



### 测试

```sh
python test/T01-simple.py
```







### 注意

1. 由于Python 全局持有gil 锁，所以Python 程序使用CGraph 进行图化运行的过程中，默认为释放gil 锁，以达到并行化目的
2. C++ 多线程执行Python 注册算子以实现图化