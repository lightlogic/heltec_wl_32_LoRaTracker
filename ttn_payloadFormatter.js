function decodeUplink(input) {
    var ilng = 0;
    for (let i = 0; i < 4; i++){
      ilng = (ilng << 8) | input.bytes[i];
    }
    var ilat = 0;
    for (let i = 0; i < 4; i++){
      ilat = (ilat << 8) | input.bytes[i+4];
    }
    return {
      data: {
        lng: ilng,
        lat: ilat
      },
      warnings: [],
      errors: []
    };
  }