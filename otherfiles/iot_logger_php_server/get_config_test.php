<?php
header('Content-Type: application/json');

if(!empty($_SERVER['QUERY_STRING']))
{
        parse_str($_SERVER['QUERY_STRING'], $output);
} 

$device_code_type="";
$congif_id=0;
$config_type='l'; // long or short
$Device_mac_id_str="";
$Device_id=-1;
// To work on this part
// Right now selection process will be done locally
$device_code_version="0.0.3-3";  // 0.0.0-3 => 0.0.0, 3
$input_code_version = ""; // 0.0.0
$input_code_version_commit_num = 0; // 3
$input_str_array = array();
$db_code_version_min = "";
$db_code_version_max = "";

if(!empty($output['device_code_type']))
{
	$device_code_type = $output['device_code_type'];	
}

if(!empty($output['congif_id']))
{
	$congif_id = $output['congif_id'];	
}

if(!empty($output['config_type']))
{
	$config_type = $output['config_type'];	
}

if(!empty($output['device_code_version']))
{
   // $device_code_version = $output['device_code_version'];
   // $input_str_array = explode ("-", $device_code_version); // 0.0.0-3 => 0.0.0, 3
    //$input_code_version_arr = $input_str_array[0];
    //$input_code_version_commit_num = $input_str_array[1];
}

if(!empty($output['Device_mac_id_str']))
{
	$Device_mac_id_str = $output['Device_mac_id_str'];	
}


$conn = mysqli_connect("localhost","id10062120_devices_logging","jUv2SjiYGhB8pkA","id10062120_devices");

$number_of_rows = -1;

if($config_type!='s')
{ 
    // Check for device info
    $sqlQuery = "SELECT * FROM `device_info` WHERE Device_mac_id_str=".$Device_mac_id_str;
    $result = mysqli_query($conn,$sqlQuery);
    //$number_of_rows=mysql_num_rows($query);

    if (!$result) 
    {
        if($number_of_rows == 0) // No entry found therefore it must be registered
        {
             //return device id as -1
        }
        //echo 'Could not run query: ' . mysql_error();
       // exit;
    }
    else
    {
        $number_of_rows=mysqli_num_rows($query);
        if($number_of_rows == 1) // existing entry and valid device 
        {
            $row = mysqli_fetch_array($result);
            $Device_id=$row['Device_id'];
        }
        if($number_of_rows == 0) // No entry found therefore it must be registered
        {
             //return device id as -1
        }

        mysqli_free_result($result);
    }

    //$sqlQuery = "INSERT INTO `device_info`(`Device_id`, `Device_config_id`, `Device_mac_id`, `Device_mac_id_str`, `Device_code_type`, `Device_code_version`, `Device_location_gps`, `Device_code_update_datetime`, `This_info_entry_datetimestamp`) VALUES ( )";
}

// Get config
$sqlQuery = "SELECT * FROM `device_config` WHERE device_code_type='".$device_code_type."' AND config_id=".$congif_id." LIMIT 1" ; //`time` DESC LIMIT 1";
 
//if(strcmp($config_type,"s") !== 0)
if($config_type==='s')
{
    $sqlQuery = "SELECT  `config_id` ,`whether_update_available` ,`device_code_to_update_to` ,`server_host_address_config` ,`server_host_port_config` ,`host_config_server_query_path` FROM `device_config` WHERE device_code_type='".$device_code_type."' AND config_id=".$congif_id." LIMIT 1" ; //`time` DESC LIMIT 1"
} 

$result = mysqli_query($conn,$sqlQuery); // could be error point
//echo $result;
//$number_of_rows=mysqli_num_rows($result);

if (!$result) {
    echo 'Could not run query: ' . mysqli_error();
    exit;
}

$data = array();
 
//$row = mysqli_fetch_array($result);
foreach ($result as $row) 
{
    $data[] = $row; 
}
// Following data is not available in the db.table `device_config'
print_r ($data);
$data[0]['Device_id'] = $Device_id;
//$data[] += ['Device_id'=> $Device_id]; // appending data at the last of the db data
print_r ($data);
// $arr = json_decode($arr, TRUE);
// $arr[] = ['id' => '9999', 'name' => 'Name'];
// $json = json_encode($arr);

//foreach ($result as $row) {
    // check with code_version
    // https://stackoverflow.com/questions/14067736/compare-two-version-strings-in-php

    // $fieldinfo=mysqli_fetch_field($result); 
    // mysqli_fetch_field is not correct way of getting the name based data
    
    // foreach($fieldinfo as $field)
    // {
    //     if($field->name == 'device_min_code_version')
    //     {

    //     }

    //     if($field->name == 'device_max_code_version')
    //     {

    //     }
    // } 
    // Following data is not available in the db.table `device_config'
//	$data += ['Device_id'=> $Device_id]; // appending data at the last of the db data
//}
  
mysqli_free_result($result);

mysqli_close($conn);

echo json_encode($data);
?>
