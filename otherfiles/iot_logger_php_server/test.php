<?php
$arr = '[
   {
      "id":1,
      "name":"Charlie"
   },
   {
      "id":2,
      "name":"Brown"
   },
   {
      "id":3,
      "name":"Subitem",
      "children":[
         {
            "id":4,
            "name":"Alfa"
         },
         {
            "id":5,
            "name":"Bravo"
         }
      ]
   },
   {
      "id":8,
      "name":"James"
   }
]';
$arr = json_decode($arr, TRUE);
$arr[] = ['id' => '9999', 'name' => 'Name'];
$json = json_encode($arr);

echo '<pre>';
print_r($json);
echo '</pre>';