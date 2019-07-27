
<?php
   
   include "./variables.php";

    if(isset($_POST['device_type']))
    {
        $radio_type = $_POST['device_type'];
        echo nl2br("\n\nYou have selected :".$radio_type."\n");  //  Displaying Selected Value
      

      if(isset($_POST['MAC_ID']))
      {
         $mac_id = $_POST['MAC_ID'];
         echo nl2br("\nMAC ID :".$mac_id."\n");  
      }  
   
      if(isset($_POST['GPS']))
      {
         $gps = $_POST['GPS'];
         echo nl2br("\nGPS :".$gps."\n");  
      }   

      if(isset($_POST['config_id']))
      {
         $cid = $_POST['config_id'];
         echo nl2br("Configuration id :".$cid."\n");  
      }   

      $conn = mysqli_connect("localhost","id10062120_devices_logging","jUv2SjiYGhB8pkA","id10062120_devices");

      $sqlQuery = "INSERT INTO `device_info`(`Device_config_id`,`Device_mac_id_str`, `Device_code_type`,`Device_location_gps`) VALUES (".$cid.",".$mac_id.",'".$radio_type."','".$gps."' )";
                // INSERT INTO `device_info`(`Device_mac_id_str`, `Device_code_type`,`Device_location_gps`) VALUES (0,'DEVT',0.0)
      $result = mysqli_query($conn,$sqlQuery);

      if ($result) { 
         //form_place
         $device_id = mysqli_insert_id($conn);
         echo nl2br("\nDevice registered having ID ".$device_id."\n"); 
         mysqli_close($conn);
        // mysqli_free_result($result);

         include "create_table.php"; 

      }else {
        echo nl2br("\nError: " . "<br>" .  mysqli_error($conn));
        //mysqli_free_result($result);
        mysqli_close($conn);
    }
    
  }
  else
  {
    //$doc = new DOMDocument();
    //$doc->loadHTML("<html><body>Test<br></body></html>");
    //echo $doc->saveHTML();
    include "device_reg_popu.php";
    //$doc = new DOMDocument();
    //$doc->loadHTMLFile("device_reg_popu.php");
    //echo $doc->saveHTML();
  }

      //echo "Device registered\n"; 
    
?>

