

<?php

include "./variables.php";

if($device_id > 0 )//!empty($output['device_id']))
{
	// $device_id = $output['device_id'];		
  
    $conn = mysqli_connect("localhost","id10062120_devices_logging","jUv2SjiYGhB8pkA","id10062120_devices");
    
    // Check table existence
    $val = mysqli_query($conn, "select 1 from `device".$device_id."` LIMIT 1");

    if($val !== FALSE) // Table exists
    {
        echo nl2br("Warning: device id ".$device_id." configuration already exists.\n\n Warning: Inform device admin. Device data will not be saved.");
    }
    else
    { 

        $sqlQuery = "CREATE TABLE `device".$device_id."` (
            `data_id` bigint(20) UNSIGNED NOT NULL AUTO_INCREMENT,
            `sr` int(11) NOT NULL,
            `dt` text NOT NULL,
            `time` datetime NOT NULL DEFAULT current_timestamp(),
            `uptm` int(11) NOT NULL,
            `temp_filter` float NOT NULL,
            `temp_raw` float NOT NULL,
            `curr_filter` float NOT NULL,
            `curr_raw` float NOT NULL,
            `accel_filter` float NOT NULL,
            `accel_raw` float NOT NULL,
            UNIQUE KEY (`data_id`)
        ) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=COMPACT";

        echo $sqlQuery;

        $result = mysqli_query($conn, $sqlQuery);

        if ($result) 
        { 
            mysqli_close($conn);
            echo nl2br("device".$device_id."database configured.");
        }
        else
        {
            echo "Error: <br>" .  mysqli_error($conn);
        }
    }
}
else
{
    echo nl2br("Warning: Inform device admin. Device data will not be saved.");
    echo "Error: Invalid input parameter as well.";
}

?>