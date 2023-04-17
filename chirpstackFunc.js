// Decode uplink function.
// Source code: https://www.youtube.com/watch?v=aliHXCR564k&ab_channel=AnhQu%C3%A2nT%E1%BB%91ng
// Input is an object with the following fields:
// - bytes = Byte array containing the uplink payload, e.g. [255, 230, 255, 0]
// - fPort = Uplink fPort.
// - variables = Object containing the configured device variables.
//
// Output must be an object with the following fields:
// - data = Object representing the decoded payload.

function hex_to_ascii(tohex) {
    var hex = tohex.toString();
    var str = '';
    for (var n = 0; n < hex.length; n += 2) {
        str += String.fromCharCode(parseInt(hex.substr(n, 2), 16));
    }
    return str;
}

function toHexString(bytes) {
    return bytes.map(function(byte) {
        return ("00" + (byte & 0xFF).toString(16)).slice(-2)
    }).join('')
}


function decodeUplink(input ) {
  var tohex = toHexString(input.bytes);
  var toascii = hex_to_ascii(tohex);
  const dataArray = toascii.split("_"); // split the string into an array of substrings
  // convert the substrings to an integer and store it in a variable
  const idVariable = parseInt(dataArray[0]); 
  const firstVariable = parseInt(dataArray[1]); 
  const secondVariable = parseInt(dataArray[2]); 
  const thirdVariable = parseInt(dataArray[3]); 
  const fourthVariable = parseInt(dataArray[4]); 
  const fifthVariable = parseInt(dataArray[5]); 
  const sixthVariable = parseInt(dataArray[6]); 
  
  if (idVariable == 1) {
    return {
      data: {
        v1: firstVariable,
        v2: secondVariable,
        v3: thirdVariable,
        a1: fourthVariable,
        a2: fifthVariable,
        a3: sixthVariable
      }
    };
  }
  else if (idVariable == 2) {
    return {
      data: {
        p1: firstVariable,
        p2: secondVariable,
        p3: thirdVariable,
        k1: fourthVariable,
        k2: fifthVariable,
        k3: sixthVariable
      }
    };
  }
}

// Encode downlink function.
//
// Input is an object with the following fields:
// - data = Object representing the payload that must be encoded.
// - variables = Object containing the configured device variables.
//
// Output must be an object with the following fields:
// - bytes = Byte array containing the downlink payload.
function encodeDownlink(input) {
  return {
    bytes: [225, 230, 255, 0]
  };
}
