<?php
// esInfo(); 
// echo esWiki();
// echo esDomain();
// echo ElasticSearch::ES_FULL_NAME;

$str = array('name'=> 'wensheng');


$es = new ElasticSearch("localhost",9200);

$arr = array(
    'type'=> 'guy',
    'index' => 'wensheng',
    'id' => 2,
    'body'=> $str
);
echo PHP_EOL . "插入：";

// $add = $es->Add("/guy/wensheng/1" , $str);

$add = $es->Add($arr);
var_export(json_decode($add, true));
return;


// 静态变量测试
// echo ElasticSearchClient::$version;
// for($i =0; $i < 10; $i++){
//     $ex = new ElasticSearchClient("localhost",$i);
//     ElasticSearchClient::$version +=  1;
//     echo ElasticSearchClient::$version . PHP_EOL;
// }




$str = array('name'=>'wensheng','age'=>'23');

// Add 测试
$add = $es->Add("/guy/wensheng/1" , $str);
echo PHP_EOL . "插入：";
var_export(json_decode($add, true));

// Update 测试
$str = array('name'=>'Vincent','age'=>'I am 27 now');
$add = $es->Update("/guy/wensheng/1" , $str);

$query = $es->query("/guy/wensheng/1");

echo PHP_EOL . "更新后：";
var_export(json_decode($query, true));


// Query 测试
echo PHP_EOL . "查询：";
$query = $es->query("/guy/wensheng/1");
var_export(json_decode($query, true));


// Delete 测试
$del = $es->Delete("/guy/wensheng/1");
echo PHP_EOL . "删除：";
var_export(json_decode($del, true));



// 删除后查询
echo PHP_EOL . "查询：";
$query = $es->query("/guy/wensheng/1");
var_export(json_decode($query, true));

