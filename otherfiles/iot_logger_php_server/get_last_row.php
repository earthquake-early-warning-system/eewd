<?php
header('Content-Type: application/json');

$conn = mysqli_connect("localhost","id10062120_devices_logging","jUv2SjiYGhB8pkA","id10062120_devices");

$sqlQuery = "SELECT * FROM `device1` ORDER BY `data_id` DESC LIMIT 1" ; //`time` DESC LIMIT 1";
 
$result = mysqli_query($conn,$sqlQuery);

$data = array();
foreach ($result as $row) {
	$data[] = $row;
}

mysqli_close($conn);

echo json_encode($data);
?>
