<?php
header('Content-Type: application/json');
 
include "./variables.php";
	
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
	}


	$conn = mysqli_connect($db_host,$db_user,$db_pwd,$db_name);
                                
	$sqlQuery = "SELECT * FROM `device".$device_id."` ORDER BY `data_id` DESC LIMIT 1" ; //`time` DESC LIMIT 1";
	
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
		
	mysqli_close($conn);

	echo json_encode($data);
} 
?>
