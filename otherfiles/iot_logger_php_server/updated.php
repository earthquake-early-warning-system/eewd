<?php

include "./variables.php";

if(!empty($_SERVER['QUERY_STRING']))
{
        parse_str($_SERVER['QUERY_STRING'], $output);
} 
 
if(!empty($output['device_code_type']))
{
	$device_code_type = $output['device_code_type'];		
} 
   
if(!empty($output['device_id']))
{
	$device_id = $output['device_id'];	
}

if(!empty($output['device_code_version']))
{
    // v0.0.3-3-g53a0111 
    $device_code_version = $output['device_code_version'];
 
}

$conn = mysqli_connect("localhost","id10062120_devices_logging","jUv2SjiYGhB8pkA","id10062120_devices");
//UPDATE `device_info` SET `Device_code_version`='v0.0.2-51-g35d2244' WHERE `Device_id`=1; 

$sqlQuery = "UPDATE `device_info` SET `Device_code_version`='".$device_code_version."' WHERE `Device_id`='".$device_id."' AND  `Device_code_type`='".$device_code_type."'"; 
 
$data = array();

if(mysqli_query($conn, $sqlQuery))
{  
    $data[] = ['whether_updated' => '1'];
}
else
{
    $data[] = ['whether_updated' => '0'];
}

mysqli_close($conn);

echo json_encode($data);
?>