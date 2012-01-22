var map;
var directionsService;

var routes = [];
var curRoute = null;
var curPosMarker = null;

///////////////////////////////////////////////////////////////////
// tests
var startLocation = new google.maps.LatLng(48.15494, 11.56468);;

function setCenter(lat, lng) {
  startLocation = new google.maps.LatLng(lat, lng);
};

function moveToCenter() {
  map.setCenter(startLocation);
};

///////////////////////////////////////////////////////////////////

// README:
// - getPolyline, getCurPos, getPrevPos return arrays of numbers
//   so that QT's evaluateJavaScript function can recognize these
// - change distance only via setDistance (or addToDistance)
//   because this function gives feedback to qt application

///////////////////////////////////////////////////////////////////
// class MapRoute
// stores all variables necessary for pedometer
// historySteps and historyDists are needed for undo function
// the polygon (path) is saved in this.poly, a MVC object
// all other function should be self explanatory
//
function MapRoute(aid) {
//public:
  if (aid === null) {
    this.id = 0;
  } else {
    this.id = aid;
  }
  this.curPos = null;
  this.prevPos = null;
  
//private:
  var modus = 1;  // 1: automatic, 2: manual
  var distance = 0;
  var historySteps = [];
  var historyDists = []; 
  var name = "";
  var distMarkers = [];
  var unit = 1000;
  
// setup polygon for route
  var polyOptions = {
    strokeColor: '#000000',
    strokeOpacity: 0.9,
    strokeWeight: 3,
    clickable: false
  };
  this.poly = new google.maps.Polyline(polyOptions);
  this.poly.setMap(map);

//privileged methods:
  this.getId = function() {
    return this.id;
  };
  this.setId = function(aid) {
    this.id = aid;
  };
  
  this.getCurPos = function() {
    return [this.curPos.lat(), this.curPos.lng()];
  };
  
  this.setCurPos = function(acurPos) {  
    this.curPos = acurPos;
  };
  
  this.getPrevPos = function() {
    return [this.prevPos.lat(), this.prevPos.lng()];
  };
  
  this.setPrevPos = function(aprevPos) {
    this.prevPos = aprevPos;
  };
  
  this.getName = function() {
    return name;
  };
  
  this.setName = function(aname) {
    name = aname;
  };
  
  this.getModus = function() {
    return modus;
  };
  
  this.setModus = function(amodus) {
    //if (amodus != 1 || amodus != 2) { return; }
    modus = amodus;
  };
  
  this.getHistorySteps = function() {
    return historySteps;
  };
  
  this.setHistorySteps = function(ahistorySteps) {
    historySteps = ahistorySteps;
  };
  
  this.getHistoryDists = function() {
    return historyDists;
  };
  
  this.setHistoryDists = function(ahistoryDists) {
    historyDists = ahistoryDists;
  };
  
  
  this.updateHistory = function(aidx, adist) {
    historySteps.push(aidx);
    historyDists.push(adist);
  };
  
  this.getDistance = function() {
    return distance;
  };
  
  this.setDistance = function(adist) {
    distance = adist;
    // this is a QT object which was made available via QT's addToJavaScriptWindowObject function
    QTDistanceSpinBoxNew.setValue(distance/1000);
    QTDistanceSpinBoxExist.setValue(distance/1000);
  };
  
  this.addToDistance = function(adist) {
    this.setDistance(distance + adist);
  };
  
  this.addDistMarker = function(amarker) {
    distMarkers.push(amarker);
  };
  
  this.removeAllMarkers = function() {
    for (var i=0; i<distMarkers.length; i++) {
      distMarkers[i].setMap(null);
    }
  };
  
  this.undoStep = function() {
    if (historySteps.length == 1) {
      return false;
    }
    // undo polyline
    var endIdx = historySteps[historySteps.length-1];
    var startIdx = historySteps[historySteps.length-2];
    var numToRemove = endIdx - startIdx;
    for (var i=0; i<numToRemove; i++) {
      this.poly.getPath().pop();
    }
    historySteps.pop();
    var dist = historyDists.pop();
    // undo distance markers
    var numToRemoveMarkers = Math.floor(distance / unit) - Math.floor((distance-dist) / unit);
    for (var i=0; i<numToRemoveMarkers; i++) {
      var amarker = distMarkers.pop();
      amarker.setMap(null);
    }
    // set distance and positions
    this.setDistance(distance - dist);
    this.prevPos = this.getLastPolyPos(); 
    this.curPos = this.getLastPolyPos();
    curPosMarker.setPosition(this.curPos);
    return true;
  };
  
  // poly: the polygon on which to set the distance markers
  // distToPolyStart: the current distance at the first point in "poly"
  this.updateDistMarkers = function(poly, distToPolyStart) { 
    var prevDist = distToPolyStart;
    var curDist = distToPolyStart;
    
    // go along the polyline
    for (var i=1; i<poly.length; i++) {
      var from = poly[i-1];
      var to = poly[i];
      var dist = distanceBetween(from, to);
      curDist += dist;
      
      // decide if there are markers on the current edge
      var numDistMarkers = Math.floor(curDist / unit) - Math.floor(prevDist / unit);
      if (numDistMarkers > 0) {
        var heading = google.maps.geometry.spherical.computeHeading(from, to);
        // compute offset from 'from' to (first) marker
        var offdist = Math.ceil(Math.max(0.0001,prevDist) / unit) * unit - prevDist;
        var startLabel = Math.ceil(Math.max(0.0001,prevDist) / unit);
        // draw marker(s)
      	for (var j=0; j<numDistMarkers; j++) {
      	  // find coordinates of marker
	  var offset = google.maps.geometry.spherical.computeOffset(from, offdist + j*unit, heading);
	  var label = startLabel + j;
	  var image = 'http://chart.apis.google.com/chart?chst=d_map_pin_letter&chld=' + label + '|FF0000|0000FF';
	  var marker = new google.maps.Marker({
            position: offset,
            map: map,
            icon: image
  	  });
  	  distMarkers.push(marker);
        }
      }
      prevDist = curDist;
    }
  };
  
  this.addSinglePosToPoly = function(pos) {
    var path = this.poly.getPath();
    path.push(pos); 
  };

  this.getLastPolyPos = function() {
    return this.poly.getPath().getAt(this.poly.getPath().length-1);
  };

  this.getPolyline = function() {
    var ps = [];
    var path = this.poly.getPath();
    for (var i=0; i<path.length; i++) {
      ps.push(path.getAt(i).lat(), path.getAt(i).lng());
    }
    return ps;
  };

// TODO: split up responsibilities of this function
  this.setPolyline = function(apoly) {
    this.poly.setPath(apoly);
    if (apoly.length > 0) {
      // set start marker
      var start = this.poly.getPath().getAt(0);
      var marker = new google.maps.Marker({
        position: start,
        map: map,
        title:"start"
      });
      distMarkers.push(marker);
      curPosMarker.setPosition(this.getLastPolyPos());
      map.setCenter(start);
    }
  };
  
  this.setPolylineExt = function(apoly) {
    this.poly.setPath(apoly);
    if (apoly.length > 0) {
      // set start marker
      var start = this.poly.getPath().getAt(0);
      var marker = new google.maps.Marker({
        position: start,
        map: map,
        title:"start"
      });
      distMarkers.push(marker);
      curPosMarker.setPosition(this.getLastPolyPos());
      
      // find bounds of route and pan
      if (apoly.length > 1) {
        var curBound = new google.maps.LatLngBounds();
        for (var i=0; i<apoly.length; i++) {
          curBound.extend(apoly[i]);
        }
        //alert(curBound.getNorthEast().lat() + "," + curBound.getNorthEast().lng() + "," + curBound.getSouthWest().lat() + "," + curBound.getSouthWest().lng());
        map.fitBounds(curBound);
      } else {
	map.setCenter(start);
      }
    }
  };
};

/////////////////////////////////////////////////////////////////////////////////////////
// this is the initializer for the pedometer widget 
function initialize() {
  // Instantiate a directions service.
  directionsService = new google.maps.DirectionsService();
  // Create a map and center it on Munich
  //var munich = new google.maps.LatLng(48.15494, 11.56468);
  var myOptions = {
    zoom: 15,
    backgroundColor: 'grey',
    mapTypeId: google.maps.MapTypeId.ROADMAP,
    center: startLocation,
    disableDoubleClickZoom: true,
    draggableCursor: 'pointer'
  };
  map = new google.maps.Map(document.getElementById("map_canvas"), myOptions);
  
  google.maps.event.addListener(map, 'dblclick', function(event) {
    handleMapDblClick(event.latLng);
  });
}

//////////////////////////////////////////////////////////////////////////////////////
// this is the initializer for the start position widget
var startLocationMarker;

function initializeStartPosition() {
  var myOptions = {
    zoom: 15,
    backgroundColor: 'grey',
    mapTypeId: google.maps.MapTypeId.ROADMAP,
    center: startLocation,
    disableDoubleClickZoom: true,
    draggableCursor: 'pointer'
  };

  map = new google.maps.Map(document.getElementById("map_canvas"), myOptions);
  
  // set startlocation marker
  startLocationMarker = new google.maps.Marker({
    position: startLocation,
    map: map,
    title:"start"
  });
  
  google.maps.event.addListener(map, 'dblclick', function(event) {
    handleMapDblClickStartPosition(event.latLng);
  });
}

function handleMapDblClickStartPosition(pos) {
  // this are QT objects which were made available via QT's addToJavaScriptWindowObject function
  // round
  lat = pos.lat();
  lng = pos.lng();
  lat *= 1000000;
  lat = Math.round(lat);
  lat /= 1000000;
  lng *= 1000000;
  lng = Math.round(lng);
  lng /= 1000000;
  
  QTLatLineEdit.setText(lat);
  QTLngLineEdit.setText(lng);
  
  // update marker position
  startLocationMarker.setPosition(pos);
  
  map.setCenter(pos);
}
/////////////////////////////////////////////////////////////////////////////////////

function getNextFreeId() {
  var nfi = 0;
  for (var i=0; i<routes.length; i++) {
    if (routes[i].id > nfi) { nfi = routes[i].id; }
  }
  return nfi+1;
}

// TODO: pass id parameter, check if route id already exists
// if yes: somehow load this route
function startNewRoute() {
  var nfid = getNextFreeId();
  var aroute = new MapRoute(nfid);
  curRoute = aroute;
  routes.push(aroute);
  
  return nfid;
}

function startNewSingleRoute(aid) {
  var id = aid
  if (aid === null) {
    id = getNextFreeId();
  }
  deleteCurRoute();
  curRoute = new MapRoute(id);
  
  curPosMarker = new google.maps.Marker({
      map: map,
      title:"start"
    });
  
  return id;
}

function deleteRoute(aid) {
  for (var i=0; i<routes.length; i++) {
    if (routes[i].id == aid) {
      // remove all markers and polyline
      routes[i].removeAllMarkers();
      routes[i].poly.setMap(null);
      // remove this route
      routes.splice(i, 1);
      if (aid == curRoute.id) {
        curRoute = null;
      }
      return true;
    }
  }
  return false;
}

function deleteCurRoute() {
  if (curRoute === null) { return false; }
  curRoute.setDistance(0);
  curRoute.removeAllMarkers();
  curRoute.poly.setMap(null);
  curRoute = null;
  curPosMarker.setMap(null);
  return true;
}

function isCurRouteNull() {
  if (curRoute === null) {
    return 1;
  }
  return 0;
}

function switchToRoute(aid) {
  for (var i=0; i<routes.length; i++) {
    if (routes[i].id == aid) {
      curRoute = routes[i];
      return true;
    }
  }
  return false;
}

function handleMapDblClick(pos) {
  if (curRoute === null) { map.setCenter(pos); return; }
  // first point
  if (curRoute.curPos === null) {
    curRoute.prevPos = pos;
    curRoute.curPos = pos;
    curRoute.addSinglePosToPoly(pos);
    curRoute.updateHistory(1, 0);
    var startMarker = new google.maps.Marker({
      position: curRoute.curPos,
      map: map,
      title:"start"
    });
    curRoute.addDistMarker(startMarker);
  } 
  else { 
    // continue existing route
    var oldPathLen = curRoute.poly.getPath().length;
    curRoute.curPos = pos;

    // manual or automatic route to curPos
    if (curRoute.getModus() == 1) {  
      calcRoute(curRoute.prevPos, curRoute.curPos);
    } else if (curRoute.getModus() == 2) {
      curRoute.addSinglePosToPoly(curRoute.curPos);
      var dist = distanceBetween(curRoute.prevPos, curRoute.curPos);
      var edge = [curRoute.prevPos, curRoute.curPos];
      
      curRoute.updateDistMarkers(edge, curRoute.getDistance());
      curRoute.addToDistance(dist);
      curRoute.updateHistory(oldPathLen+1, dist);
    }
    curPosMarker.setPosition(pos);
    curRoute.prevPos = curRoute.curPos;   
  }
  map.setCenter(pos);
}

function calcRoute(start, end) { 
  // create a DirectionsRequest using WALKING directions.
  var request = {
      origin: start,
      destination: end,
      travelMode: google.maps.DirectionsTravelMode.WALKING
  };

  // Route the directions
  directionsService.route(request, function(response, status) {
    if (status == google.maps.DirectionsStatus.OK) {
      // update path
      var pathNewPart = getAllSteps(response);
      var path = curRoute.poly.getPath();
      for (var i=0; i<pathNewPart.length; i++) {
        path.push(pathNewPart[i]);
      }
      
      // update distance markers
      curRoute.updateDistMarkers(pathNewPart, curRoute.getDistance());
     
      // update distance
      var dist = response.routes[0].legs[0].distance.value;
      curRoute.addToDistance(dist);
     
      // update history
      curRoute.updateHistory(path.length, dist); 
    }
  });
}

function getAllSteps(directionResult) {
  var myLegs = directionResult.routes[0].legs[0];
  var idx = 0;
  var allSteps = [];

  for (var i = 0; i < myLegs.steps.length; i++) {
     var myStep = myLegs.steps[i];
     //alert("myStep.path.length=" + myStep.path.length);
     for (var j = 0; j < myStep.path.length; j++) {
	allSteps[idx] = myStep.path[j];
      //  alert("allSteps[" + idx + "]=" + myStep.path[j]);
        idx++;
     }
  }
  return allSteps;
}

function distanceBetween(latLngFrom,latLngTo) {
    return google.maps.geometry.spherical.computeDistanceBetween(latLngFrom, latLngTo);
}

