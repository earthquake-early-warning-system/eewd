const char MAIN_page[] PROGMEM = R"=====(
<!doctype html>
<html>

<head>
	<title>Line Chart | air conditioner sense</title>
	<script src = "https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.7.3/Chart.min.js"></script>	
	<style>
	canvas{
		-moz-user-select: none;
		-webkit-user-select: none;
		-ms-user-select: none;
	}

	/* Data Table Styling */
	#dataTable {
	  font-family: "Trebuchet MS", Arial, Helvetica, sans-serif;
	  border-collapse: collapse;
	  width: 100%;
	}

	#dataTable td, #dataTable th {
	  border: 1px solid #ddd;
	  padding: 8px;
	}

	#dataTable tr:nth-child(even){background-color: #f2f2f2;}

	#dataTable tr:hover {background-color: #ddd;}

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
    <div style="text-align:center;"><b>Air conditioner sense</b><br>Real Time Data Logging with Graphs on ESP8266</div>
    <div class="chart-container" position: relative; height:400px; width:100%">
        <canvas id="Chart_temp" width="400" height="400"></canvas>
    </div>
    <div class="chart-container" position: relative; height:400px; width:100%">
        <canvas id="Chart_curr" width="400" height="400"></canvas>
    </div>
    <div class="chart-container" position: relative; height:400px; width:100%">
        <canvas id="Chart_acc" width="400" height="400"></canvas>
    </div>
<div>
	<table id="dataTable">
	  <tr><th>Sr.</th>
      <th>Time</th>
      <th>dt</th>
      <th>temp Value filtered</th><th>temp Value raw</th>
      <th>curr Value filtered</th><th>curr Value raw</th>
      <th>acc Value filtered</th><th>accc Value raw</th>
      </tr>
	</table>
</div>
<br>
<br>	

<script>
//Graphs visit: https://www.chartjs.org
var values = [];
var timeStamp = [];
var Chart_temp, Chart_curr, Chart_acc;
var sr=0;
var lasttimelocal=new Date();
var lasttimectrl=0;

// function updateChart()
// {
//     Chart2.data[0].datasets[0].data[0]
// }

function createGraph()
{
    // for (i = 0; i < arguments.length; i++) {
    // 	values[0].push(arguments[i]);    
    // }

    var ctx_temp = document.getElementById("Chart_temp").getContext('2d');
    Chart_temp = new Chart(ctx_temp, {
        type: 'line',
        // data: {
        //     labels: timeStamp,	//Bottom Labeling
        //     datasets: [{
        //         label: "temperature",
        //         fill: false,	//Try with true
        //         backgroundColor: 'rgba( 243, 156, 18 , 1)', //Dot marker color
        //         borderColor: 'rgba( 243, 156, 18 , 1)',	//Graph Line Color
        //         data: values[0]
        //     }],
        // },

        data: {
        datasets: [{
            label: 'temperature filtered',
            data: [],
            fill: false,
            backgroundColor: 'rgba( 243, 156, 18 , 1)', //Dot marker color
            borderColor: 'rgba( 243, 156, 18 , 1)'	//Graph Line Color
             
        }, {
            label: 'temperature raw',
            data: [],
            fill: false,
            backgroundColor: 'rgba( 156, 243, 18 , 1)', //Dot marker color
            borderColor: 'rgba( 156, 156, 18 , 1)'	//Graph Line Color
            // Changes this dataset to become a line
            //type: 'line'
        }],
        labels: timeStamp
    },

        options: {
            title: {
                    display: true,
                    text: "temperature"
                },
            maintainAspectRatio: false,
            elements: {
            line: {
                    tension: 0.5 //Smoothening (Curved) of data lines
                }
            },
            scales: {
                    yAxes: [{
                        ticks: {
                            beginAtZero:true
                        }
                    }]
            }
        }
    });

var ctx_curr = document.getElementById("Chart_curr").getContext('2d');
Chart_curr = new Chart(ctx_curr, {
        type: 'line',
        // data: {
        //     labels: timeStamp,	//Bottom Labeling
        //     datasets: [{
        //         label: "temperature",
        //         fill: false,	//Try with true
        //         backgroundColor: 'rgba( 243, 156, 18 , 1)', //Dot marker color
        //         borderColor: 'rgba( 243, 156, 18 , 1)',	//Graph Line Color
        //         data: values[0]
        //     }],
        // },

        data: {
        datasets: [{
            label: 'current filtered',
            data: [],
            fill: false,
            backgroundColor: 'rgba( 243, 156, 18 , 1)', //Dot marker color
            borderColor: 'rgba( 243, 156, 18 , 1)'	//Graph Line Color
             
        }, {
            label: 'current raw',
            data: [],
            fill: false,
            backgroundColor: 'rgba( 156, 243, 18 , 1)', //Dot marker color
            borderColor: 'rgba( 156, 156, 18 , 1)'	//Graph Line Color
            // Changes this dataset to become a line
            //type: 'line'
        }],
        labels: timeStamp
    },

        options: {
            title: {
                    display: true,
                    text: "Current"
                },
            maintainAspectRatio: false,
            elements: {
            line: {
                    tension: 0.5 //Smoothening (Curved) of data lines
                }
            },
            scales: {
                    yAxes: [{
                        ticks: {
                            beginAtZero:true
                        }
                    }]
            }
        }
    });

var ctx_acc = document.getElementById("Chart_acc").getContext('2d');
Chart_acc = new Chart(ctx_acc, {
        type: 'line',
        // data: {
        //     labels: timeStamp,	//Bottom Labeling
        //     datasets: [{
        //         label: "temperature",
        //         fill: false,	//Try with true
        //         backgroundColor: 'rgba( 243, 156, 18 , 1)', //Dot marker color
        //         borderColor: 'rgba( 243, 156, 18 , 1)',	//Graph Line Color
        //         data: values[0]
        //     }],
        // },

        data: {
        datasets: [{
            label: 'accelerometer filtered',
            data: [],
            fill: false,
            backgroundColor: 'rgba( 243, 156, 18 , 1)', //Dot marker color
            borderColor: 'rgba( 243, 156, 18 , 1)'	//Graph Line Color
             
        }, {
            label: 'accelerometer raw',
            data: [],
            fill: false,
            backgroundColor: 'rgba( 156, 243, 18 , 1)', //Dot marker color
            borderColor: 'rgba(  156,156, 18 , 1)'	//Graph Line Color
            // Changes this dataset to become a line
            //type: 'line'
        }],
        labels: timeStamp
    },

        options: {
            title: {
                    display: true,
                    text: "Acceleration"
                },
            maintainAspectRatio: false,
            elements: {
            line: {
                    tension: 0.5 //Smoothening (Curved) of data lines
                }
            },
            scales: {
                    yAxes: [{
                        ticks: {
                            beginAtZero:true
                        }
                    }]
            }
        }
    });

}

//On Page load show graphs
window.onload = function() {
	console.log(new Date().toLocaleTimeString());
	createGraph();
};

//Ajax script to get ADC voltage at every 5 Seconds 
//Read This tutorial https://circuits4you.com/2018/02/04/esp8266-ajax-update-part-of-web-page-without-refreshing/


function getDateString() {
  const date = new Date();
  const year = date.getFullYear();
  const month = `${date.getMonth() + 1}`.padStart(2, '0');
  const day =`${date.getDate()}`.padStart(2, '0');
  const hour = `${date.getHours()}`.padStart(2, '0');
  const min = `${date.getMinutes()}`.padStart(2, '0');
  const sec = `${date.getSeconds()}`.padStart(2, '0');
  
  return `${year}${month}${day}${hour}${min}${sec}`;
}

function download_csv() {
    var csv = "Sr., dt,	Time,	temp Value filtered,	temp Value raw,	curr Value filtered,	curr Value raw,	acc Value filtered,	accc Value raw\r\n";
    values.forEach(function(row) {
            csv += row.join(",");
            csv += "\r\n";
    });
 
     
    console.log(csv);
    var hiddenElement = document.createElement('a');
    hiddenElement.href = 'data:text/csv;charset=utf-8,' + encodeURI(csv);
    hiddenElement.target = '_blank';
    hiddenElement.download = 'data_'+getDateString()+'.csv';
    hiddenElement.click();

    // Do not delete the values
    //values=[];
    //values.length = 0;
    // if(values.length>4*24)
    // {
    //     values=[];
    //     values.length = 0;
    // }
}

function delete_values()
{
    //if(values.length>4*60*24) // ( (15 seconds 4 samples ) in an hour) in a whole day
    {
        values=[];
        values.length = 0;
    }
}

setInterval( getData , 10000); //5000mSeconds update rate

setInterval( download_csv , 30*60000); //30 m update rate

setInterval( delete_values , 24*60*60000); // delete in 24 hours

function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
     //Push the data in array
	var time = new Date().toLocaleTimeString();
    var timectrl;
    var ValueArr = this.responseText.split(',');
	//var ADCValue = this.responseText; 
      values.push([sr+1, time, ValueArr[2], ValueArr[3], ValueArr[4], ValueArr[5], ValueArr[6], ValueArr[7]]);
      timeStamp.push(time);

      console.log(values.length);
 
      sr = sr+1; // ValueArr[0];
      timectrl = ValueArr[1];
      //showGraph();	//Update Graphs
	//Update Data Table
	  var table = document.getElementById("dataTable");
	  var row = table.insertRow(1);	//Add after headings
      var cell0 = row.insertCell(0);
	  var cell1 = row.insertCell(1);
	  var cell2 = row.insertCell(2);
      var cell3 = row.insertCell(3);
      var cell4 = row.insertCell(4);
      var cell5 = row.insertCell(5);
      var cell6 = row.insertCell(6);
      var cell7 = row.insertCell(7);
      var cell8 = row.insertCell(8);
      

	  cell0.innerHTML = sr;
      cell1.innerHTML = time; 
      cell2.innerHTML = [new Date().getTime()-lasttimelocal.getTime() , parseInt(timectrl) - parseInt(lasttimectrl)].toString();
	  cell3.innerHTML = ValueArr[2];
      cell4.innerHTML = ValueArr[3];
      cell5.innerHTML = ValueArr[4];
      cell6.innerHTML = ValueArr[5];
      cell7.innerHTML = ValueArr[6];
      cell8.innerHTML = ValueArr[7];


      lasttimelocal = new Date();
      lasttimectrl = timectrl;
      
      Chart_temp.data.datasets[0].data.push(ValueArr[2]);
      Chart_temp.data.datasets[1].data.push(ValueArr[3]);

      Chart_curr.data.datasets[0].data.push(ValueArr[4]);
      Chart_curr.data.datasets[1].data.push(ValueArr[5]);

      Chart_acc.data.datasets[0].data.push(ValueArr[6]);
      Chart_acc.data.datasets[1].data.push(ValueArr[7]);

      if(table.rows.length>20)
      {
         Chart_temp.data.datasets[0].data.shift();
         Chart_temp.data.datasets[1].data.shift();
         Chart_curr.data.datasets[0].data.shift();
         Chart_curr.data.datasets[1].data.shift();
         Chart_acc.data.datasets[0].data.shift();
         Chart_acc.data.datasets[1].data.shift();

         table.deleteRow(table.rows.length -1);
         timeStamp.shift();

      }

      if(Chart_temp.data.datasets[1].data.length > 20)
      {
         //Chart_temp.data.datasets[1].data.shift();
      }

      
      Chart_temp.update(100);
      Chart_curr.update(100);
      Chart_acc.update(100);
    }
  };
  xhttp.open("GET", "readData", true);	//Handle readData server on ESP8266
  xhttp.send();
}
		
</script>
</body>

</html>

)=====";
