// setInterval(function ( ) {
//         var xhttp = new XMLHttpRequest();
//         xhttp.onreadystatechange = function() {
//         if (this.readyState == 4 && this.status == 200) {
//             document.getElementById("temperature").innerHTML = this.responseText;
//         }
//         };
//         xhttp.open("GET", "/temperature", true);
//         xhttp.send();
//     }, 3000 
// );

if (!!window.EventSource) {
    var source = new EventSource('/events');
  
    source.addEventListener('open', function(e) {
      console.log("Events Connected");
    }, false);
  
    source.addEventListener('error', function(e) {
      if (e.target.readyState != EventSource.OPEN) {
        console.log("Events Disconnected");
      }
    }, false);
  
    source.addEventListener('message', function(e) {
      console.log("message", e.data);
    }, false);
  
    source.addEventListener('sensor_readings', function(e) {
      console.log("sensor_readings", e.data);
      var myObj = JSON.parse(e.data);
      console.log(myObj);
      plotTemperature(myObj);
    }, false);
}

function goConfig() {
    location.replace("https://%DEVICE_IP%/config");
}

function goWifi() {
    location.replace("https://%DEVICE_IP%/wifi");
}