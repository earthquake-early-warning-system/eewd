<?php
header('Content-Type: application/json');


// If uncommented the gui will not load 
// As it requires to be publish only the data

//echo 'Current PHP version: ' . phpversion();

//echo ' | MySQL server version: '. mysqli_get_client_info();

//echo "\r\n\r\n\r\n";


$conn = mysqli_connect("localhost","id10062120_devices_logging","jUv2SjiYGhB8pkA","id10062120_devices");

$sqlQuery = "SELECT * FROM `device1` ORDER BY time DESC LIMIT 20";

$sql = "INSERT INTO `device1` (`sr`, `dt`, `time`, `uptm`, `temp_filter`, `temp_raw`, `curr_filter`, `curr_raw`, `accel_filter`, `accel_raw`) VALUES (\'1\', \'675,986\', \'2019-06-21 11:00:00\', \'80366\', \'35.9\', \'36.0\', \'0.07\', \'0.07\', \'1.0\', \'1.02\')";

$result = mysqli_query($conn,$sqlQuery);


$data = array();
foreach ($result as $row) {
	$data[] = $row;
}


//while ($row = mysqli_fetch_array($result)) {
 //       $data[] = $row;
//}

mysqli_close($conn);

//echo json_encode($data, JSON_NUMERIC_CHECK);

//array_push($json, $data);
//echo json_encode($json);


echo json_encode($data);
?>


