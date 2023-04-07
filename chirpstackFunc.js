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
  return {
    data: {
      mydata: toascii
    }
  };
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
