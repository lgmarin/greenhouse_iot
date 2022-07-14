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
      updateValues(myObj);
    }, false);
}


function updateValues(jsonObj) {
    document.getElementById("dht_t").innerHTML = jsonObj.sensors.dht_t;
    document.getElementById("dht_h").innerHTML = jsonObj.sensors.dht_h;
    document.getElementById("soil_h").innerHTML = jsonObj.sensors.soil_p;
}


function goConfig() {
    location.replace("https://%DEVICE_IP%/config");
}


function goWifi() {
    location.replace("https://%DEVICE_IP%/wifi");
}