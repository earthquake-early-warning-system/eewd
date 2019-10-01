<?php
header('Content-Type: application/json');
include "./variables.php";

// If uncommented the gui will not load 
// As it requires to be publish only the data

//echo 'Current PHP version: ' . phpversion();

//echo ' | MySQL server version: '. mysqli_get_client_info();

//echo "\r\n\r\n\r\n";
 
if(!isset($_POST['debug']))
{
	$debug = $_POST['debug'];	
}

if(!empty($_POST['device_id']))
{
	$device_id = $_POST['device_id'];	
 
	if(!isset($_POST['debug']))
	{
		echo nl2br("\n");
		echo __LINE__;
		echo " device_id = $device_id ";
		print_r(  $_POST ); 

			
		echo "<pre>";
			print_r($_POST);
		echo "</pre>";

	}


	$conn = mysqli_connect($db_host,$db_user,$db_pwd,$db_name);

	$sqlQuery = "SELECT * FROM `device".$device_id."` ORDER BY time DESC LIMIT 20";

	$sql = "INSERT INTO `device".$device_id."` (`sr`, `dt`, `time`, `uptm`, `temp_filter`, `temp_raw`, `curr_filter`, `curr_raw`, `accel_filter`, `accel_raw`) VALUES (\'1\', \'675,986\', \'2019-06-21 11:00:00\', \'80366\', \'35.9\', \'36.0\', \'0.07\', \'0.07\', \'1.0\', \'1.02\')";

	$result = mysqli_query($conn,$sqlQuery);
	$data = array();

	if($result)
	{

		foreach ($result as $row) 
		{
			$data[] = $row;
		}
	}
	else
	{
		if(!isset($_POST['debug']))
		{
			echo nl2br("\n");
			echo __LINE__;
			echo nl2br("\nError: " . "<br>" .  mysqli_error($conn));
		}
	}


	//while ($row = mysqli_fetch_array($result)) {
	//       $data[] = $row;
	//}

	mysqli_close($conn);

	//echo json_encode($data, JSON_NUMERIC_CHECK);

	//array_push($json, $data);
	//echo json_encode($json);


	echo json_encode($data);
} 
?>


