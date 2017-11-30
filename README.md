# 扩展类名 ElasticSearchClient

## 依赖

- libcurl
- php-x
- 支持C++11的 g++/clang编译器

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

Add/Query/Delete/Update 的参数支持两种风格:

```
// 1. 字符串形式
// 文档的uri 和 文档内容分开传
// 如:
 Add("my_index/my_type/my_id", "{my_doc}");

 //2. 奇怪的数组形式,键的名称固定
Add(
    array(
        'index' => 'my_index',
        'type' => 'my_type',
        'id' => 'my_id',
        'body' => array(
            'field' => 'value'
        )
    )
);
```

- Add( string $index, (string | array)$doc)
- Add( array $data)

- Query(string $index)
- Query(array $data)

- Update(string $index, (string | array)$new_doc)
- Update($data);

- Delete(string $index)
- Delete(array $data);

## 全局方法
- esDomain 
- esWiki
- esInfo

## 全局变量
- ES

## 静态变量
- $version

