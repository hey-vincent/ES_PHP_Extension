# 扩展类名 ElasticSearchClient

## 依赖

- libcurl
- php-x
- 支持C++11的 g++/c++

### libcurl

- brew install curl 
- 编译链接curl库 `-lcurl`

### php-x

- https://github.com/swoole/PHP-X
- cmake .
- make
- sudo make install

## 编译器
- 由于使用了C++11 lambda表达式 需要 指定C++11
- mac 环境下编译 需要指定 -shared -undefined dynamic_lookup

#### 具体参考 Makefile


## 安装
- make
- sudo make install
- 配置php.ini开启 `extension=ElasticSearchClient.so`

## 扩展名 ElasticSearchClient

## 成员
- host (public)
- port (public)
- $version (static)

## 方法：
- __construct($host, $port)
- Add($index, $doc)
- Query($index)
- Update($index, $new_doc)
- Delete($index)

## 全局方法
- esDomain 
- esWiki
- esInfo

## 全局变量
- ES

## 静态变量
- $version

