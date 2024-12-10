% Benchmark for recursively partitioning meshes, based on various
% bisection approaches
%
% D.P & O.S for the "HPC Course" at USI and
%                   "HPC Lab for CSE" at ETH Zurich



% add necessary paths
addpaths_GP;
nlevels_a = 3;
nlevels_b = 4;

fprintf('       *********************************************\n')
fprintf('       ***  Recursive graph bisection benchmark  ***\n');
fprintf('       *********************************************\n')

% load cases
cases = {
    'airfoil1.mat';
    'netz4504_dual.mat';
    'stufe.mat';
    '3elt.mat';
    'barth4.mat';
    'ukerbe1.mat';
    'crack.mat';
    };

nc = length(cases);
maxlen = 0;
for c = 1:nc
    if length(cases{c}) > maxlen
        maxlen = length(cases{c});
    end
end

for c = 1:nc
    fprintf('.');
    sparse_matrices(c) = load(cases{c});
end


fprintf('\n\n Report Cases         Nodes     Edges\n');
fprintf(repmat('-', 1, 40));
fprintf('\n');
for c = 1:nc
    spacers  = repmat('.', 1, maxlen+3-length(cases{c}));
    [params] = Initialize_case(sparse_matrices(c));
    fprintf('%s %s %10d %10d\n', cases{c}, spacers,params.numberOfVertices,params.numberOfEdges);
end

%% Create results table
fprintf('\n%7s %16s %20s %16s %16s\n','Bisection','Spectral','Metis 5.0.2','Coordinate','Inertial');
fprintf('%10s %10d %6d %10d %6d %10d %6d %10d %6d\n','Partitions',8,16,8,16,8,16,8,16);
fprintf(repmat('-', 1, 100));
fprintf('\n');


for c = 1:nc
    spacers = repmat('.', 1, maxlen+3-length(cases{c}));
    fprintf('%s %s', cases{c}, spacers);
    sparse_matrix = load(cases{c});
    

    % Recursively bisect the loaded graphs in 8 and 16 subgraphs.
    % Steps
    % 1. Initialize the problem
    [params] = Initialize_case(sparse_matrices(c));
    W      = params.Adj;
    coords = params.coords;
    % 2. Recursive routines
    % i. Spectral
      [map_spec_8,~,~] = rec_bisection('bisection_spectral', 3, W, coords, 0);
      edge_cut_spec_8 = cutsize(W, map_spec_8);
      [map_spec_16,~,~] = rec_bisection('bisection_spectral', 4, W, coords, 0);
      edge_cut_spec_16 = cutsize(W, map_spec_16);

    % ii. Metis
      [map_metis_8,~,~] = rec_bisection('bisection_metis', 3, W, coords, 0);
       edge_cut_metis_8 = cutsize(W, map_metis_8);
      [map_metis_16,~,~] = rec_bisection('bisection_metis', 4, W, coords, 0);
       edge_cut_metis_16 = cutsize(W, map_metis_16);

    % iii. Coordinate
      [map_coord_8,~,~] = rec_bisection('bisection_coordinate', 3, W, coords, 0);
       edge_cut_coord_8 = cutsize(W, map_coord_8);

      [map_coord_16,~,~] = rec_bisection('bisection_coordinate', 4, W, coords, 0);
       edge_cut_coord_16 = cutsize(W, map_coord_16);

    % iv. Inertial
      [map_inert_8,~,~] = rec_bisection('bisection_inertial', 3, W, coords, 0);
       edge_cut_inert_8 = cutsize(W, map_inert_8);

      [map_inert_16,~,~] = rec_bisection('bisection_inertial', 4, W, coords, 0);
       edge_cut_inert_16 = cutsize(W, map_inert_16);


    % 3. Calculate number of cut edges

    % 4. Visualize the partitioning result
     fprintf('%6d %6d %10d %6d %10d %6d %10d %6d\n',edge_cut_spec_8,edge_cut_spec_16,...
    edge_cut_metis_8,edge_cut_metis_16,edge_cut_coord_8,edge_cut_coord_16,edge_cut_inert_8,edge_cut_inert_16);
end

figure;
gplotmap(W, coords, map_spec_16);
title("Spectral")
pause;
figure;
gplotmap(W, coords, map_metis_16);
title("Metis")
pause;
figure
gplotmap(W, coords, map_coord_16);
title("Coordinate")
pause;
figure;
gplotmap(W, coords, map_inert_16);
title("Inertial")
pause;
close all;
    

