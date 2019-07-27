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
    <div id='form_place' align="center">
    <h2>Register a new devices</h2>
    <h3><font color='red'>Device once registered can not be modified ever.</font></h3>
        
        <form action="" method="POST" enctype="multipart/form-data">

            <div style="display: inline-block; text-align: left;">
                <label for="MAC_ID">MAC ID</label>
                <input id="MAC_ID" name="MAC_ID" type="text" maxlength="12" value="000000000000" autocomplete="off">
                Enter only alpaneumerals. No symboles. <br><font color='red'>Make sure you are aware of the device mac id</font><br>
                <br><label for="GPS">GPS </label><input id="GPS" name="GPS" type="text" maxlength="17" value="000.0000,000.0000" autocomplete="off">
                <br><label id="GPS_Err">Allow your browser to get your location.</label>
                <br>
                <br><label for="device_type">Device type</label>
                <div style="border: thin solid black">
                    <!-- the radio values are critical as the name -->
                    <input type="radio" name="device_type" value="DEVT" checked> Development (Experimental)<br>
                    <input type="radio" name="device_type" value="OBSN"> Observational (In case of any issue needs observation)
                    <br>
                    <input type="radio" name="device_type" value="DEPL"> Mass Deployment (Do not use it) <br>
                    <input type="radio" name="device_type" value="PROD"> Production (recommended)<br>
                </div>
                <br>
                
                <label for="Config_id">Config id (most stable is (tried to be) config id with 0.)</label>

                <!--div style="display: inline-block; text-align: left; height:200px;width:100%; overflow:auto; overflow-x:scroll; overflow-y:scroll;"-->
                <div style="width:  640px; height: 170px; overflow-y: scroll; overflow-x: scroll;">
                    
                    <table  id="dataTable">

                        <thead>
                            <tr>
                                <th>Selection</th>
                                <?php 
                                $conn = mysqli_connect("localhost","id10062120_devices_logging","jUv2SjiYGhB8pkA","id10062120_devices");
                                $sqlQuery = "SELECT * FROM `device_config` WHERE 1";
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

                                $cntr = 0;

                                while ($row=mysqli_fetch_row($result))
                                {
                                    
                                    echo "<tr><td><input type='radio' name='config_id' value=".$cntr."></td>";
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

        </form><input type="submit" />
        </div>
    </div>

    <script>
        var x = document.getElementById("GPS");

        function getLocation() {
            if (navigator.geolocation) {
                navigator.geolocation.getCurrentPosition(showPosition, errfn, {
                    maximumAge: 600000,
                    timeout: 5000,
                    enableHighAccuracy: true
                });

            } else {
                //x.innerHTML = "Geolocation is not supported by this browser.";
            }
        }

        function errfn(error) {
            var y = document.getElementById("GPS_Err");
            y.value = error.message;
        }

        function showPosition(position) {
            x.value = position.coords.latitude +
                "," + position.coords.longitude;
        }

        //$(document).ready(getLocation);
        window.onload = getLocation;
    </script>

</body>

</html>