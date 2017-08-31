var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.setRequestHeader("Accept", "application/JSON");
  xhr.send();
};

var stop;
var routes = [];
var names = [];
var countdowns = [];

function getSchedule() {
	var url = 'http://api.translink.ca/rttiapi/v1/stops/' + stop + '/estimates?apikey=hCnIQTl1g1LNlWOZhEfa&count=3';
	console.log("URL is: " + url);

	xhrRequest(url, "GET",
		function(responseText) {
			var json = JSON.parse(responseText);
			console.log(responseText);
      //
			// if (json.Code) {
			// 	console.log("An error occured: " + json.Code);
			// 	var error_code = json.Code;
      //
			// 	var err_dictionary = {
			// 		"KEY_ERROR_CODE": error_code
			// 	};
      //
			// 	Pebble.sendAppMessage(err_dictionary,
			// 		function(e) {
			// 			console.log("App message sent!");
			// 		},
			// 		function(e) {
			// 			console.log("Error sending app message");
			// 	});
			// } else {
			// 	console.log("RouteNo: " + json[0].RouteNo);
			// 	var route_number = json[0].RouteNo;
      //
			// 	console.log("RouteName: " + json[0].Schedules[0].Destination);
			// 	var route_name = json[0].Schedules[0].Destination;
      //
			// 	console.log("ExpectedCountdown: " + json[0].Schedules[0].ExpectedCountdown);
			// 	var arrival_time = json[0].Schedules[0].ExpectedCountdown;
      //
      //
			// 	var dictionary = {
			// 		"MsgKeyDummy": route_name
			// 	};
      //
			// 	Pebble.sendAppMessage(dictionary,
			// 		function(e) {
			// 			console.log("App message sent!");
			// 		},
			// 		function(e) {
			// 			console.log("Error sending app message");
			// 	});
      //
			// }

      if (json.length === undefined) {
        console.log("Error code received");

        var error_code = json.Code;

				var dict = {
					"MsgKeyErrorCode": error_code
				};

				Pebble.sendAppMessage(dict,
					function(e) {
						console.log("App message sent!");
					},
					function(e) {
						console.log("Error sending app message");
				});

      } else {
        console.log("Buses returned: " + json.length);

        for (i = 0; i < json.length; i++) {
            routes.push(json[i].RouteNo);
            names.push(json[i].Schedules[0].Destination);
            countdowns.push(json[i].Schedules[0].ExpectedCountdown);
        }

        var dict = {};

        for (i = 0; i < json.length; i++) {
          dict["MsgKeyRoute" + i] = routes[i];
          dict["MsgKeyName" + i] = names[i];
          dict["MsgKeyCountdown" + i] = countdowns[i].toString();
        }

        Pebble.sendAppMessage(dict,
					function(e) {
						console.log("App message sent!");
					},
					function(e) {
						console.log("Error sending app message");
				});

      }
		});
}

Pebble.addEventListener('ready', function() {
	console.log('PebbleKit JS Ready!');

	//getSchedule(); // Testing mode
});

Pebble.addEventListener('appmessage',
  function(e) {
    console.log('AppMessage received!');
    console.log('Message contents: ' + JSON.stringify(e.payload));

    var messageContents = e.payload;
    console.log(messageContents.MsgKeyStopNumber);

    if (messageContents.MsgKeyStopNumber) { // If KEY_CONDITIONS exists in the appmessage
      console.log('Stop number received in appmessage');

			stop = messageContents.MsgKeyStopNumber;

      getSchedule(); // Fetch the weather
    }
  }
);
