<?php

// config
//$device_code_type;
$config_id=-1;
$config_type;  // 's'-short or 'l'-long or 'd'-direct using config_id
$Device_mac_id_str;
$device_id=-1;
// To work on this part
// Right now selection process should be done locally
$device_code_version="0.0.3-3";  // 0.0.0-3 => 0.0.0, 3
$input_code_version = ""; // 0.0.0
$input_code_version_commit_num = 0; // 3
$input_str_array = array();
$db_code_version = ""; // 0.0.0
$db_code_version_commit_num = 0; // 3
$db_str_array = array();
$db_code_version_min = "";
$db_code_version_max = "";

// insert
$sr=0;
$dt='0,0'; 
$time='0000-00-00 00:00:00';
$uptm=0;
$temp_filter=0;
$temp_raw=0;
$curr_filter=0;
$curr_raw=0;
$accel_filter=0;
$accel_raw=0;

$sensor_state=""; // IAWC IAWC IAWC

$device_code_type="UNKNOWN";
$device_code_version="?";
//$device_id="?";
//$config_id="0";


// register
$radio_type="";
$mac_id="";
$gps="";
$cid= -1;
$device_id = -1;

$debug=0;

?>