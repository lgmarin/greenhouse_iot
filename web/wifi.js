//first add an event listener for page load
//document.addEventListener( "DOMContentLoaded", get_json_data, false ); // get_json_data is the function name that will fire on page load

var auto_ip = document.querySelector("#set-auto-ip");

window.onload = function() {
    document.querySelector("#ip").closest('.mdl-textfield').style.display = "none";
    document.querySelector("#gateway").closest('.mdl-textfield').style.display = "none";
    document.querySelector("#mask").closest('.mdl-textfield').style.display = "none";
    append_json(res_test);
    addRowHandlers();
};

auto_ip.onchange = function() {
    if(this.checked){
        document.querySelector("#ip").closest('.mdl-textfield').style.display = "none";
        document.querySelector("#gateway").closest('.mdl-textfield').style.display = "none";
        document.querySelector("#mask").closest('.mdl-textfield').style.display = "none";
        document.getElementById("ip").required = false;
        document.getElementById("gateway").required = false;
        document.getElementById("mask").required = false;
    }else{
        document.querySelector("#ip").closest('.mdl-textfield').style.display = "";
        document.querySelector("#gateway").closest('.mdl-textfield').style.display = "";
        document.querySelector("#mask").closest('.mdl-textfield').style.display = "";
        document.getElementById("ip").required = true;
        document.getElementById("gateway").required = true;
        document.getElementById("mask").required = true;
    }
}

function addRowHandlers() {
    var table = document.getElementById("wifi-table");
    var rows = table.getElementsByTagName("tr");
    for (i = 0; i < rows.length; i++) {
        var currentRow = table.rows[i];
        var createClickHandler = 
            function(row) 
            {
                return function() { 
                    var cell = row.getElementsByTagName("td")[0];
                    var id = cell.innerHTML;
                    document.querySelector("#wifi_ssid").parentNode.MaterialTextfield.change(id);
                };
            };
        currentRow.onclick = createClickHandler(currentRow);
    }
}

var res_test = {"networks": 
                [{"SSID":"Wifi 1", "Quality":"20"},
                {"SSID":"Wifi 2", "Quality":"50"},
                {"SSID":"Wifi 3", "Quality":"100"},
                {"SSID":"Wifi 4", "Quality":"20"}
                // {"SSID":"Wifi 5", "Quality":"50"},
                // {"SSID":"Wifi 6", "Quality":"100"}
                ]};


//this function is in the event listener and will execute on page load
function get_json_data(){
    // Relative URL of external json file
    var json_url = 'example.json';

    //Build the XMLHttpRequest (aka AJAX Request)
    xmlhttp = new XMLHttpRequest();
    xmlhttp.onreadystatechange = function() { 
        if (this.readyState == 4 && this.status == 200) {//when a good response is given do this

            var data = JSON.parse(this.responseText); // convert the response to a json object
            append_json(data); //pass the json object to the append_json function
            addRowHandlers(); //Refresh rows handlers
        }
    }
    //set the request destination and type
    xmlhttp.open("POST", json_url, true);
    //set required headers for the request
    xmlhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    // send the request
    xmlhttp.send(); // when the request completes it will execute the code in onreadystatechange section
}

//this function appends the json data to the table 'gable'
function append_json(data){
    var table = document.getElementById('wifi-table-body');
    data.networks.forEach(function(object) {
        var tr = document.createElement('tr');
        tr.classList.add("table-row");
        tr.innerHTML = 
        '<td class="mdl-data-table__cell--non-numeric">' + object.SSID + '</td>' +
        '<td>' + object.Quality + '</td>';
        table.appendChild(tr);
    });
}