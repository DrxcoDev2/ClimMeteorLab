import json
import os

# Carga el TopoJSON
with open('data/ESP_adm1.topo.json', 'r') as f:
    topo = json.load(f)

# Extrae la transformación
scale = topo['transform']['scale']
translate = topo['transform']['translate']

def decode_arc(arc):
    x = 0
    y = 0
    coords = []
    for dx, dy in arc:
        x += dx
        y += dy
        lon = x * scale[0] + translate[0]
        lat = y * scale[1] + translate[1]
        coords.append((lon, lat))
    return coords

# Extrae los polígonos (puede ser MultiPolygon!)
polygons = []
for obj in topo['objects'].values():
    geometries = obj['geometries'] if 'geometries' in obj else [obj]
    for geom in geometries:
        if geom['type'] in ['Polygon', 'MultiPolygon']:
            arcs_list = geom['arcs']
            if geom['type'] == 'Polygon':
                for arc_indices in arcs_list:
                    poly = []
                    for arc_idx in arc_indices:
                        arc = topo['arcs'][arc_idx if arc_idx >= 0 else ~arc_idx]
                        pts = decode_arc(arc)
                        if arc_idx < 0:
                            pts = pts[::-1]
                        poly += pts
                    polygons.append(poly)
            elif geom['type'] == 'MultiPolygon':
                for polygon_arcs in arcs_list:
                    for arc_indices in polygon_arcs:
                        poly = []
                        for arc_idx in arc_indices:
                            arc = topo['arcs'][arc_idx if arc_idx >= 0 else ~arc_idx]
                            pts = decode_arc(arc)
                            if arc_idx < 0:
                                pts = pts[::-1]
                            poly += pts
                        polygons.append(poly)

# Imprime puntos para C++
with open('resources/polygons.txt', 'w') as file:
    for poly in polygons:
        for lon, lat in poly:
            file.write(f'    {{{lon:.6f}, {lat:.6f}}},\n')