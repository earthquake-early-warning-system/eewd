<?php

header('Content-Type: application/json');

include "./variables.php";
include "./telegram_bot.php";

//print_r( $_SERVER['QUERY_STRING'] ); 

$querystr='';

if(!empty($_SERVER['QUERY_STRING']))
{
    $querystr = $_SERVER['QUERY_STRING'];
    
    //print_r($querystr);
}
 
$conn = mysqli_connect($db_host,$db_user,$db_pwd,$db_name);

$sqlQuery = "SELECT `Device_id` FROM `device_info` WHERE 1";// //Device_id='".$device_id."'";
$devices = array();

if ($result = mysqli_query($conn,$sqlQuery)) 
{ 
    //print_r( $result ); 
    
    $number_of_rows=mysqli_num_rows($result); 

    //if($number_of_rows>=1)
    {
        foreach ($result as $row) 
        {
            $devices[] = $row;

            //print_r($row);
            //print_r($devices);

            $device_id = $row['Device_id']; // loop through all the devices

            if($device_id==1)
            {
                continue;
            }

            $sqlQuery = "SELECT `time` FROM `device".$device_id."` ORDER BY `data_id` DESC LIMIT 1" ; //`time` DESC LIMIT 1";
            $data = array();
            
            if ($result2 = mysqli_query($conn,$sqlQuery)) 
            {
                $number_of_rows=mysqli_num_rows($result2); 
            
                if($number_of_rows==1)
                {
                    foreach ($result2 as $row) 
                    {
                        $data[] = $row;
                    }

                
                    $date = new DateTime( $row['time'] );
                    $now = new DateTime();

                    $subTime = $date->diff($now);//->format("i");
                    
                    $m = ($subTime->days * 24 * 60) +
                ($subTime->h * 60) + $subTime->i;

                    
                    $str="Device$device_id\n was offline for\n ".$date->diff($now)->format("%D days, %H hours, %I minutes and %S seconds");

                    if($m>45)
                    { 
                        telegram_bot_send($str, 'test'  ); 
                    }
                    else
                    {
                        if(!isset($querystr['debug']))
                        {
                            $str= "Device$device_id is online.";
                            echo $str;
                            //send("DEBUG\n\nDevice$device_id is online.", 55129840);
                            telegram_bot_send("DEBUG\n\n".$str, 'debug' ); 
                        } 
                    }
                } 
            }
            else
            {
                if(!isset($querystr['debug']))
                {
                    //echo nl2br("\n");
                    //echo __LINE__;
                    echo nl2br(" ".__LINE__.": Error: " .  mysqli_error($conn)); 
                    telegram_bot_send("DEBUG\n\nInvalid data for Device$device_id.", 'debug' ); 
                }
            }
        } 
    } 
}
else
{
    if(!isset($querystr['debug']))
    {
        echo $devices['Device_id'];
        print_r( $devices ); 
    }
}
                                
	
    mysqli_close($conn);
	//echo json_encode($data);
?>