<?php
// esInfo(); 
// echo esWiki();
// echo esDomain();
// echo ElasticSearch::ES_FULL_NAME;

// string 参数
$str = "/guy/wensheng/1";

// array 参数
$arr = array(
    'index' => 'test_index',
    'type'=> 'test_type',
    'id' => 1,
    'body'=> array(
        'field1'=> 'value1',
        'field2'=> 'value1',
    )
);

$es = new ElasticSearchClientX("localhost",9200);

// Add 测试
echo PHP_EOL . "插入：";
$add = $es->Add($arr);
var_export(json_decode($add, true));


// Query 测试
echo PHP_EOL . "查询：";
$query = $es->search($arr);
var_export(json_decode($query, true));



// 静态变量测试
// echo ElasticSearchClient::$version;
// for($i =0; $i < 10; $i++){
//     $ex = new ElasticSearchClient("localhost",$i);
//     ElasticSearchClient::$version +=  1;
//     echo ElasticSearchClient::$version . PHP_EOL;
// }


// Update 测试
$str = array('name'=>'Vincent','age'=>'I am 27 now');
$add = $es->update("/guy/wensheng/1" , $str);

$query = $es->search("/guy/wensheng/1");

echo PHP_EOL . "更新后：";
var_export(json_decode($query, true));





// Delete 测试
$del = $es->remove("/guy/wensheng/1");
echo PHP_EOL . "删除：";
var_export(json_decode($del, true));



// 删除后查询
echo PHP_EOL . "查询：";
$query = $es->search("/guy/wensheng/1");
var_export(json_decode($query, true));

