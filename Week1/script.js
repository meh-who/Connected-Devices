// variables for the DOM elements:
let portSelector;

let speakingButton;
let encoderSlider;
let encoderButton;
// variable for the serialport object:
let serial;
// previous state of the serial input from the button:
let lastButtonState = 0;

let lastShaftVal = 0;
let lastButtonVal = 0;

// this function is called when the page is loaded. 
// element event listeners are  added here:
function setup(event) {
    // add listeners for the power button and the fan speed:

    speakingButton = document.getElementById('sensor');
    //speakingButton.addEventListener('click', setSpeakingState);

    encoderSlider = document.getElementById('shaft');
    //encoderSlider.addEventListener('change',setEncoderShaft);

    encoderButton = document.getElementById('button');
    //encoderButton.addEventListener('click', setEncoderButton);

    // initialize the serialport object:
    serial = new p5.SerialPort(); // new instance of the serialport library
    serial.on('list', printList); // callback function for serialport list event
    serial.on('data', serialEvent); // callback function for serialport data event
    serial.list(); // list the serial ports

    
}


// make a serial port selector object:
function printList(portList) {
  // create a select object:
  portSelector = document.getElementById('portSelector');
  // portList is an array of serial port names
  for (var i = 0; i < portList.length; i++) {
    // add this port name to the select object:
    var option = document.createElement("option");
    option.text = portList[i];
    portSelector.add(option);
  }
  // set an event listener for when the port is changed:
  portSelector.addEventListener('change', openPort);
}

function openPort() {
  let item = portSelector.value;
  // if there's a port open, close it:
  if (serial.serialport != null) {
    serial.close();
  }
  // open the new port:
  serial.open(item);
}


function serialEvent() {
  // read a line of incoming data:
  var inData = serial.readLine();
  // if the line is not empty, parse it to JSON:
  if (inData) {
    var sensors = JSON.parse(inData);

    //webpage control
    if (sensors.shaft !== lastShaftVal) {
        encoderScroll(sensors.shaft);
        lastShaftVal = sensors.shaft;
    }
    if (sensors.button != lastButtonVal) {
        encoderPress(sensors.button);
        lastButtonVal = sensors.button;
    }
    checkSound(sensors.sound);
  }

}

function encoderScroll(shaft) {
    var inputs = document.getElementsByClassName('input');
    inputs[shaft%inputs.length].focus();
    var thisSpan = document.getElementById('shaftVal');
    thisSpan.innerHTML = "Shaft is " + shaft;
}

function encoderPress(button) {
    var thisSpan = document.getElementById('buttonVal');
    thisSpan.innerHTML = "Button is " + button;
}

function checkSound(sound) {
    var thisSpan = document.getElementById('speakingVal');
    if (sound == true){
        thisSpan.innerHTML = "speaking";
    }
    else{
        thisSpan.innerHTML = "silent";
    }
    
}


// add a listener for the page to load:
window.addEventListener('DOMContentLoaded', setup);