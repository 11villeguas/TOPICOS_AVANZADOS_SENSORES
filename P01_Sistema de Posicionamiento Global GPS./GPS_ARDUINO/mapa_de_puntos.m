figure
geoscatter(lat, lon, 10, 'filled')
geobasemap satellite
title('Trayectoria GPS')
figure
geoplot(lat, lon, '-o')
geobasemap streets
title('Recorrido GPS')
