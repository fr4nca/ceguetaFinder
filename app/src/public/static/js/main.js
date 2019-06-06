const socket = io();

const initialize = async () => {
  const locs = await getLocations();

  const mapOptions = {
    center: new google.maps.LatLng(
      locs[locs.length - 1].lat,
      locs[locs.length - 1].long
    ),
    zoom: 8,
    mapTypeId: google.maps.MapTypeId.ROADMAP
  };

  const map = new google.maps.Map(
    document.getElementById('map_canvas'),
    mapOptions
  );

  socket.on('newLoc', loc => {
    addMarker(loc, map);
  });

  locs.forEach(loc => {
    addMarker(loc, map);
  });
};

const getLocations = async () => {
  const res = await fetch('/api/map');
  const locs = await res.json();
  return locs;
};

const addMarker = (loc, map) => {
  const ponto = new google.maps.LatLng(loc.lat, loc.long);
  const marker = new google.maps.Marker({
    position: ponto,
    map: map,
    animation: google.maps.Animation.DROP,
    title: `Neste ponto em: ${moment(loc.createdAt)
      .utc()
      .format('DD/MM/YY HH:mm:ss')}`
  });
  const latLng = marker.getPosition();
  map.setCenter(latLng);
};

document.addEventListener('DOMContentLoaded', () => {
  initialize();
});
