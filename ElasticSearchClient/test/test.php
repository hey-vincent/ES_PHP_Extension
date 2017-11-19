<?php
esInfo();
// echo esWiki();
// echo ElasticSearchClient::ES_FULL_NAME;

$es = new ElasticSearchClient("localhost",9200);
// echo ElasticSearchClient::$version;
// for($i =0; $i < 10; $i++){
//     $ex = new ElasticSearchClient("localhost",$i);
//     ElasticSearchClient::$version +=  1;
//     echo ElasticSearchClient::$version . PHP_EOL;
// }




$str = array('name'=>'wensheng','age'=>'23');

$add = $es->add("/guy/wensheng/1" , $str);
echo PHP_EOL . "插入：";
var_export(json_decode($add, true));


$str = array('name'=>'Vincent','age'=>'I am 27 now');
$add = $es->Update("/guy/wensheng/1" , $str);

$query = $es->query("/guy/wensheng/1");

echo PHP_EOL . "查询：";
var_export(json_decode($query, true));


// echo PHP_EOL . "插入：";
// print_r(json_decode($add, true));


// $query = $es->query("/guy/wensheng/1");

// echo PHP_EOL . "查询：";
// var_export(json_decode($query, true));


// $del = $es->delete("/guy/wensheng/1");

// echo PHP_EOL . "删除：";
// var_export(json_decode($del, true));




// $query = $es->query("/guy/wensheng/1");

// echo PHP_EOL . "查询：";
// var_export(json_decode($query, true));

