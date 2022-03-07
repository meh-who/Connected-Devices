// advertised service UUID of the  to search for:
const serviceUuid = '26b68f48-bddc-42fa-8a03-38be7b2bfc6e';
// DOM elements to interact with:
let connectButton;
let dataDiv;
let deviceDiv;
let ledButton;
// TODO these could be one JSON object representing the device
// and its services and characteristics:
let myDevice;
let myCharacteristic;
let timeCharacteristic;


// this function is called when the page is loaded. 
// event listener functions are initialized here:
function setup() {
  // put the DOM elements into global variables:
  connectButton = document.getElementById('connect');
  connectButton.addEventListener('click', connectToBle);
  ledButton = document.getElementById('canvas_picker');
  ledButton.addEventListener('click', writeToCharacteristic);
  deviceDiv = document.getElementById('device');
  dataDiv = document.getElementById('data');
  calibButton = document.getElementById('calibrate');
  calibButton.addEventListener('click', calibrateTime);
}

// connect to the peripheral:
function connectToBle() {
  // options let you filter for a peripheral 
  // with a particular service UUID:
  let options = {
    filters: [{
      services: [serviceUuid]
    }]
  };
  // start scanning:
  navigator.bluetooth.requestDevice(options)
    // when you get a device:
    .then(device => {
      myDevice = device;
    //   deviceDiv.innerHTML = "Device name: " + device.name;
    //   deviceDiv.innerHTML += "<br>Service UUID: " + serviceUuid;
      return device.gatt.connect();
    })
    // get the primary service:
    .then(server => server.getPrimaryService(serviceUuid))
    .then(service => service.getCharacteristics())
    // get the characteristics of the service:
    .then(characteristics => readCharacteristics(characteristics))
    // if there's an error:
    .catch(error => console.log('Connection failed!', error));

  function readCharacteristics(characteristics) {
    // add the characterisitic UUID to the device div:
    deviceDiv.innerHTML += "<br>Connected " ;
    myCharacteristic = characteristics[0];
    // Get the initial value:
    characteristics[0].readValue()
      .then(data => {
        let ledState = data.getUint8(0);
        ledButton.checked = ledState;
      });
  }
}

function calibrateTime(){
  if (myDevice && myDevice.gatt.connected) {
    var date = new Date();
    var h = date.getHours(); 
    var m = date.getMinutes(); 
    var s = date.getSeconds(); 

    console.log(h);
    console.log(m);
    console.log(s);

    timeCharacteristic.writeValue(new Uint8Array([r,g,b,0]));
  }
}

// write to a characteristic:
function writeToCharacteristic() {
  // LED state is whether or not the button is checked:
  // let ledState = event.target.checked;
  // if you're connected, write to the peripheral:
  if (myDevice && myDevice.gatt.connected) {
    // convert to an ArrayBufferView:
    // var element = document.getElementById("zeshu");
    // alert(element.innerHTML);
    // let valueToSend = Int16Array.of(element.innerHTML);
    // debugger
    // alert(valueToSend);
    
    var r = window.img_data[0];
    var g = window.img_data[1];
    var b = window.img_data[2];

    // write to characteristic now:
    // myCharacteristic.writeValue(valueToSend);
    myCharacteristic.writeValue(new Uint8Array([r,g,b,0]));
  }
  // alert(zeshu);
}

// function testme(){
//   alert("hello");
//   var element = document.getElementById("zeshu");
//   alert(element.innerHTML);
// }

// This is a listener for the page to load.
// This is the command that actually starts the script:
window.addEventListener('DOMContentLoaded', setup);