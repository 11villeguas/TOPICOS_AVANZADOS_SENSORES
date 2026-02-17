% Coordenadas
data = [
    22.783605,-102.615737; 22.783605,-102.615722; 22.783601,-102.615722;
    22.783605,-102.615722; 22.783605,-102.615722; 22.783605,-102.615722;
    22.783605,-102.615722; 22.783605,-102.615722; 22.783609,-102.615722;
    22.783609,-102.615707; 22.783609,-102.615707; 22.783609,-102.615707;
    22.783613,-102.615707; 22.783613,-102.615707; 22.783609,-102.615707;
    22.783609,-102.615707; 22.783609,-102.615707; 22.783609,-102.615707;
    22.783609,-102.615707; 22.783609,-102.615707
];

% datos
lats = data(:, 1);
lons = data(:, 2);

colorGuinda = [0.5, 0, 0];
colorBlanco = [1, 1, 1];

% figura y ejes geográficos
fig = figure('Color', 'w');
gx = geoaxes('Parent', fig);
hold(gx, 'on');

geobasemap(gx, 'satellite');
gx.Grid = 'off';

% trayectoria
geoplot(gx, lats, lons, '-o', ...
    'Color', colorBlanco, ...
    'LineWidth', 1.5, ...
    'MarkerSize', 4, ...
    'MarkerFaceColor', colorBlanco, ...
    'MarkerEdgeColor', [0.4 0.4 0.4]); 

% PUNTO INICIAL Blanco
geoscatter(gx, lats(1), lons(1), 150, ...
    'MarkerFaceColor', colorBlanco, ...
    'MarkerEdgeColor', 'k', ...
    'LineWidth', 1.2);

% PUNTO FINAL
geoscatter(gx, lats(end), lons(end), 150, ...
    'MarkerFaceColor', colorGuinda, ...
    'MarkerEdgeColor', colorBlanco, ...
    'LineWidth', 1.2);

gx.Center = [mean(lats), mean(lons)];
gx.ZoomLevel = 21; % Zoom máximo para ver los puntos (están muy cerca entre sí)

title('Trayectoria GPS: Micro-movimientos Marcados', 'FontWeight', 'bold');