# ml编译
# release/debug版本生成可执行文件ml_engine
# gtest版本生成可执行文件ml_gtest

# release 版本编译方式
```
rm -rf build 
mkdir build
cd build 
cmake -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_BUILD_RPATH=/opt/ide ..
make
```
# debug版本编译
```
rm -rf build 
mkdir build
cd build 
cmake -DCMAKE_BUILD_TYPE=DEBUG -DCMAKE_BUILD_RPATH=/opt/ide ..
make
```
# gtest版本编译
```
rm -rf build 
mkdir build
cd build 
cmake -DCMAKE_BUILD_TYPE=GTEST -DCMAKE_BUILD_RPATH=/opt/ide ..
make
```
#生成gcov代码覆盖率
```
rm -rf build 
mkdir build
cd build 
cmake -DCMAKE_BUILD_TYPE=GTEST -DCMAKE_BUILD_RPATH=/opt/ide ..
make gen_coverage_html
```
