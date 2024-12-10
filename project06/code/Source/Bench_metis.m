function [cut_recursive,cut_kway] = Bench_metis(picture)
% Compare recursive bisection and direct k-way partitioning,
% as implemented in the Metis 5.0.2 library.

%  Add necessary paths
addpaths_GP;

% Graphs in question
load usroads;
adj_us = Problem.A;
coords_us = Problem.aux.coord;

load luxembourg_osm;
adj_lux = Problem.A;
coords_lux = Problem.aux.coord;

load CH-4468_graph;
adj_ch = W;
coords_ch = coords;

load RU-40527_graph.mat;
adj_ru = W;
coords_ru = coords;

load NO-9935_graph.mat;
adj_no = W;
coords_no = coords;

load VN-4031_graph.mat;
adj_vn = W;
coords_vn = coords;

load GR-3117_graph.mat;
adj_gr = W;
coords_gr = coords;

partitions = [16, 32];
names = ["USA", "Luxembourg", "Switzerland", "Russia", "Norway", "Vietnam", "Greece"];
adjs = {adj_us, adj_lux, adj_ch, adj_ru, adj_no, adj_vn, adj_gr};
coordinates = {coords_us, coords_lux, coords_ch, coords_ru, coords_no, coords_gr};
% Steps
% 1. Initialize the cases
% 2. Call metismex to
%     a) Recursively partition the graphs in 16 and 32 subsets.
%     b) Perform direct k-way partitioning of the graphs in 16 and 32 subsets.
% 3. Visualize the results for 32 partitions.
part_recursive = cell(length(adjs), length(partitions));
part_kway = cell(length(adjs), length(partitions));
for j = 1:length(adjs)
    fprintf('############## %s ##############\n', names(j));
    for i = 1:length(partitions)
        k = partitions(i);
        [part_recursive{j,i}, cut_recursive] = metismex('PartGraphRecursive', adjs{j}, k);
        [part_kway{j,i}, cut_kway] = metismex('PartGraphKway', adjs{j}, k);
        
        fprintf('For %d partitions:\n', k);
        fprintf('Recursive cut: %d\n', cut_recursive);
        fprintf('K-way cut: %d\n', cut_kway);
    end
end

figure;
gplotmap(adj_us,coords_us, part_recursive{1,2});
pause;

figure;
gplotmap(adj_us,coords_us, part_kway{1,2});
pause;
close all;
figure;
gplotmap(adj_lux,coords_lux, part_recursive{2,2});
pause;

figure;
gplotmap(adj_lux,coords_lux, part_kway{2,2});
pause;
close all;
figure;
gplotmap(adj_ru,coords_ru, part_recursive{4,2});
pause;
figure;
gplotmap(adj_ru,coords_ru, part_kway{4,2});
pause;
close all;

end