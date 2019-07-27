<?php

include "./variables.php";

//session_start();

// if(!isset($_SESSION["last_millis_server"]))
// {
//  	$_SESSION["last_millis_server"] = 0;
// }
// else
// {
// 	//echo "lms ".$_SESSION["last_millis_server"]." ";
// }

// if(!isset($_SESSION["last_millis_iot"]))
// {
//  	$_SESSION["last_millis_iot"] = 0;
// }
// else
// {
// 	//echo "lmi ".$_SESSION["last_millis_iot"]." ";
// }

header('Content-Type: application/json');
 
 
//echo date("Y-m-d H:i:s") ;//(new DateTime("now", new DateTimeZone("America/New_York")) - $date_last);
 
//foreach (getallheaders() as $name => $value) { 
//    echo "$name: $value <br>"; 
//} 

//$header = apache_request_headers(); 
  
//foreach ($header as $headers => $value) { 
//    echo "$headers: $value <br />\n"; 
//} 
 
//print_r(PDO::getAvailableDrivers());
$output= array();

if(!empty($_SERVER['QUERY_STRING']))
{
        parse_str($_SERVER['QUERY_STRING'], $output);
} 

//print_r($output);
   
if(!empty($output['sr']))
{
	$sr = $output['sr'];	
}

if(!empty($output['dt']))
{
	$dt = $output['dt'];		
}
if(!empty($output['time']))
{
	$time = $output['time'];		
}
if(!empty($output['uptm']))
{
	$uptm = $output['uptm'];		
}
if(!empty($output['temp_filter']))
{
	$temp_filter = $output['temp_filter'];		
}
if(!empty($output['temp_raw']))
{
	$temp_raw = $output['temp_raw'];		
} 
if(!empty($output['curr_filter']))
{
	$curr_filter = $output['curr_filter'];		
}
if(!empty($output['curr_raw']))
{
	$curr_raw = $output['curr_raw'];		
} 
if(!empty($output['accel_filter']))
{
	$accel_filter = $output['accel_filter'];		
}
if(!empty($output['accel_raw']))
{
	$accel_raw = $output['accel_raw'];		
} 

if(!empty($output['sensor_state']))
{
	$sensor_state = $output['sensor_state'];	// IAWC IAWC IAWC three characters	
} 

if(!empty($output['device_code_type']))
{
	$device_code_type = $output['device_code_type'];		
} 
if(!empty($output['device_code_version']))
{
	$device_code_version = $output['device_code_version'];		
} 
if(!empty($output['device_id']))
{
	$device_id = $output['device_id'];		
} 

if(!empty($output['config_id']))
{
	$config_id = $output['config_id'];	
	//echo nl2br("\n insert.php device_id=$device_id, config_id=$config_id");
                    	
}  

 


//$millis_diff = round(microtime(true) * 1000) - $_SESSION["last_millis_server"];
//$uptm_diff = $uptm - $_SESSION["last_millis_iot"];

//$dt_str ="'" . $uptm_diff.",".$millis_diff."'";  

$dt_str ="'" . $uptm.",".round(microtime(true) * 1000)."'";  

//echo " dt_str ".$dt_str." ";

/*
try
{
$DBH = new PDO("mssql:host=$host;dbname=$dbname, $user, $pass");
 
$STH = $DBH->(
"INSERT INTO 
`air_conditioner_sense`
 ( `sr`, `dt`, `time`, `uptm`, `temp_filter`, `temp_raw`, `curr_filter`, `curr_raw`, `accel_filter`, `accel_raw`)
 VALUES (" 
. $output['sr']
.",". $output['dt']
.",". $output['time']
.",". $output['uptm']
.",". $output['temp_filter']
.",". $output['temp_raw']
.",". $output['curr_filter']
.",". $output['curr_raw']
.",". $output['accel_filter']
.",". $output['accel_raw'] 
.")");
$STH->execute($output);

}
catch(PDOException $e) {
    echo $e->getMessage();
}
*/

$conn = mysqli_connect("localhost","id10062120_devices_logging","jUv2SjiYGhB8pkA","id10062120_devices");

$sqlQuery = "INSERT INTO `device".$device_id."` ( `sr`, `dt`, `time`, `uptm`, `temp_filter`, `temp_raw`, `curr_filter`, `curr_raw`, `accel_filter`, `accel_raw`) VALUES (" 
.$sr
.','.strval($dt_str)
.', NOW()'//. strval($time)
.','. $uptm
.','. $temp_filter
.','. $temp_raw
.','. $curr_filter
.','. $curr_raw
.','. $accel_filter
.','. $accel_raw
.")";

//echo $sqlQuery;
$result = mysqli_query($conn,$sqlQuery);
 
//$_SESSION["last_millis_server"] = round(microtime(true) * 1000) ;
//$_SESSION["last_millis_iot"] = $uptm; 

if ($result) {

	// Make sure these varables are available
	//$device_code_type="";
	//$congif_id=0; 
	//$config_type=s or l short or long
	//$device_code_version="0.0.3-3";  // 0.0.0-3 => 0.0.0, 3 
	// The $data can used as output

	// Call another php 
	//echo json_encode($data); // or use the ob_start()
	//ob_start();
	mysqli_close($conn);

	include "get_config.php";

	//$content = ob_get_contents();

	//ob_end_clean();

	//echo $content;
	//echo json_encode($data);
    

    //echo "OK"; //"New record created successfully.";// for " . $sqlQuery;
} else {
    echo "Error: " . "<br>" .  mysqli_error($conn);
    mysqli_close($conn);
}


//$DBH = null;
  
?>

