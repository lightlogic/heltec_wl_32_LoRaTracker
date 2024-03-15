/**
 * specifications: The fields to
 *   include or exclude.
 */
{
  _id: 0,
  type: 0,
  granularity: 0,
  properties: 0,
  "features.type": 0,
  "features.geometry.type": 0,
  "features.geometry.coordinates": 1
}


IoTdata.rgrPositions.aggregate([
 {
     $project:{
      timestamp: 1,
      'latitude': '$features.geometry.coordinates',
      _id: 0
      }
  }     
])

IoTdata.rgrPositions.aggregate([
 {
     $project:{ 
         latitude: { $arrayElemAt: [ "$coordinates", 0 ] },
         longitude: { $arrayElemAt: [ "$coordinates", 1 ] }
     }
 }     
])