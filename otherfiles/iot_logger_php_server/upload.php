<html>
<head>
<style type="text/css">
.column {
            float: left;
            width: 33%;
            padding: 1px;
            height: auto;
            display: block;
            /* Should be removed. Only for demonstration */
        }
        /* Clear floats after the columns */
        
        .row:after {
            content: "";
            display: table;
            clear: both;
        }
        /* Data Table Styling */
        
        #dataTable {
            font-family: "Trebuchet MS", Arial, Helvetica, sans-serif;
            border-collapse: collapse;
            width: 100%;
        }
        
        #dataTable td,
        #dataTable th {
            border: 1px solid #ddd;
            padding: 8px;
        }
        
        #dataTable tr:nth-child(even) {
            background-color: #f2f2f2;
        }
        
        #dataTable tr:hover {
            background-color: #ddd;
        }
        
        #dataTable th {
            padding-top: 12px;
            padding-bottom: 12px;
            text-align: left;
            background-color: #4CAF50;
            color: white;
        }
        </style>
</head>
   <body>
   <div align="center">
    <h2>Update devices</h2>
    <form action="" method="POST" enctype="multipart/form-data">
         
   <div style="display: inline-block; text-align: left;">
      
      
       <input type="file" name="image" /><br>
       <div style="border: thin solid black">
         <!-- the radio values are critical as the name -->
         <input type="radio" name="image_type" value="devt" checked> Development (Experimental)<br>
        <input type="radio" name="image_type" value="obsn"> Observational (In case of any issue needs observation)<br>
        <input type="radio" name="image_type" value="depl"> Mass Deployment (warning: this will update all the device marked as 'depl'.) <br>
        <input type="radio" name="image_type" value="prod"> Production (release for third party)<br>
       </div>
       <div>
       <label for="Config_ids">Select Config ids to update</label>

                <!--div style="display: inline-block; text-align: left; height:200px;width:100%; overflow:auto; overflow-x:scroll; overflow-y:scroll;"-->
                <div style="width:  640px; height: 170px; overflow-y: scroll; overflow-x: scroll;">
                    
                    <table  id="dataTable">

                        <thead>
                            <tr>
                                <th>Selection</th>
                                <?php 
                                $conn = mysqli_connect("localhost","id10062120_devices_logging","jUv2SjiYGhB8pkA","id10062120_devices");
                                $sqlQuery = "SELECT * FROM `device_config` WHERE `config_id`!=0";
                                // INSERT INTO `device_info`(`Device_mac_id_str`, `Device_code_type`,`Device_location_gps`) VALUES (0,'DEVT',0.0)
                                $result = mysqli_query($conn,$sqlQuery);  
                                 

                                // foreach ($data[0] as $colm=>$val) {
                                //     echo "<th>".$colm."</th>";
                                // }

                                while ($field_info = mysqli_fetch_field($result))
                                {
                                    echo "<th>{$field_info->name}</th>";
                                }

                                // for($i = 0; $i < mysqli_num_fields($result); $i++) {
                                //         $field_info = mysqli_fetch_field($result, $i);
                                //         echo "<th>{$field_info->name}</th>";
                                //     }                                  
                                    ?>
                            </tr>
                        </thead>
                        <tbody>
                       
                        
                                <?php 

                                // foreach ($data as $val) {
                                //     echo "<tr>";
                                //     echo "<td>input type radio</td>";
                                //     // foreach ($val as $element) {
                                //     //       echo "<td>".$element."</td>";
                                //     // } 
                                //    echo "</tr>";
                                // } 

                                $cntr = 1;

                                while ($row=mysqli_fetch_row($result))
                                {
                                    
                                    echo "<tr><td><input type='checkbox' name='config_ids[]' value=".$cntr."></td>";
                                    $cntr++;
                                    foreach ($row as $cell) {
                                        echo "<td >{$cell}</td>";
                                    }
                                    echo "</tr>";
                                    //echo "<th>{$field_info->name}</th>"; 
                                }
                        // while ($row = mysqli_fetch_array($query))
                        // {
                            
                        //     echo "<td>{field_data}</td>";
                        //     for($i = 0; $i < mysqli_num_fields($result); $i++) {
                        //         $field_data = $row[$i];
                        //         echo "<td>{field_data}</td>";
                        //     }  
                        // }
                        ?>
                            
                        </tbody>
                        <tfoot>
                            <tr>
                                <th colspan="4">TOTAL</th>
                                <th>
                                  <?php mysqli_num_fields($result);
                                  mysqli_free_result($result); ?>
                                </th>
                            </tr>
                        </tfoot>
                    </table>

       </div>
       <label for="code_version">Code version</label>
       <input id="code_version" name="code_version" type="text" maxlength="32" value="v0.0.0-4-g1abcdef" autocomplete="off">
         
      </form><input type="submit"/>
      </div></div>
   </body>
</html>
<?php
   if(isset($_FILES['image']))
   {
      $errors= array(); 
      $file_name = $_FILES['image']['name'];
      $file_size =$_FILES['image']['size'];
      $file_tmp =$_FILES['image']['tmp_name'];
      $file_type=$_FILES['image']['type'];
      $file_ext=strtolower(end(explode('.',$_FILES['image']['name'])));
      $radio_type="";
      $code_version="";
      $config_ids = array();
      $debug=0;

      if(isset($_POST['debug']))
      {
         $debug = $_POST['debug'];
      }

      if(isset($_POST['image_type']))
        {
            $radio_type = $_POST['image_type'];
            echo nl2br("You have selected :".$radio_type."\n");  //  Displaying Selected Value
        }

        if(isset($_POST['code_version']))
        {
            $code_version = $_POST['code_version'];
            echo nl2br("version number given :".$code_version."\n");  //  Displaying Selected Value
        }
 
        if(!empty($_POST['config_ids'])) 
        {
            // Counting number of checked checkboxes.
            // //if($debug=='1')
            // { 
            //    $checked_count = count($_POST['config_ids']);
            //    echo "You have selected following ".$checked_count." option(s): <br/>";
            // }// Loop to store and display values of individual checked checkbox.
            
            foreach($_POST['config_ids'] as $selected) 
            {
            //echo "<p>".$selected ."</p>";
               $config_ids[] += $selected;
            }
         //echo "<br/><b>Note :</b> <span>Similarily, You Can Also Perform CRUD Operations using These Selected Values.</span>";
         }
 
    
      $extensions= array("bin");
      
      if(in_array($file_ext,$extensions)=== false){
         $errors[]="extension not allowed, please choose a bin file.";
      }
      
      if($file_size > 2097152){
         $errors[]='File size must be excately 2 MB';
      }
      
      if(empty($errors)==true)
      {
         $folder_name = strtoupper($radio_type);
         move_uploaded_file($file_tmp,"uploads/".$folder_name."/".$code_version.".bin");

         
         $conn = mysqli_connect("localhost","id10062120_devices_logging","jUv2SjiYGhB8pkA","id10062120_devices");
         $sqlQuery = "UPDATE `device_config` SET `device_code_to_update_to`='$code_version' WHERE `config_id` IN (".implode(',',$config_ids).")";
          
       
         if(mysqli_query($conn, $sqlQuery))
         {   
            echo "Affected number of configs: " . mysqli_affected_rows($conn);
            echo "<br>Success";
            echo "<font color='red'> All the devices with related config ids will be updated soon.</font>";
         }
         else
         { 
            echo "Failed";
            echo "Error: " . "<br>" .  mysqli_error($conn);
         }

         mysqli_close($conn); 
      }
      else
      {
         print_r($errors);
      } 
   }
 
?>
