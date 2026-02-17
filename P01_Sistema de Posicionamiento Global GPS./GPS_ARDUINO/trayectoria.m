lats = Position.latitude;
lons = Position.longitude;

% colores
colorGuinda = [0.5, 0, 0];
colorBlanco = [1, 1, 1];

fig = figure('Color', 'w');
ax = geoaxes('Parent', fig);
hold(ax, 'on');

geobasemap(ax, 'satellite');
ax.Grid = 'off';

geoplot(ax, lats, lons, '-o', ...
    'Color', colorBlanco, ...
    'LineWidth', 1.5, ...
    'MarkerSize', 3, ...
    'MarkerFaceColor', colorBlanco);

% 2. Punto inicial
geoscatter(ax, lats(1), lons(1), 130, ...
    'MarkerEdgeColor', 'k', ... % Borde negro fino para que resalte sobre el blanco
    'MarkerFaceColor', colorBlanco);

% 3. Punto final
geoscatter(ax, lats(end), lons(end), 130, ...
    'MarkerEdgeColor', colorBlanco, ...
    'MarkerFaceColor', colorGuinda);

ax.Center = [mean(lats), mean(lons)];
ax.ZoomLevel = 21;

title('Ruta GPS: Trayectoria Detallada', 'Color', 'k', 'FontSize', 12);