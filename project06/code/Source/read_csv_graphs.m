% Script to load .csv lists of adjacency matrices and the corresponding 
% coordinates. 
% The resulting graphs should be visualized and saved in a .csv file.
%
% D.P & O.S for the "HPC Course" at USI and
%                   "HPC Lab for CSE" at ETH Zurich

addpaths_GP;

countries = {'CH-4468', 'CL-13042', 'GB-5946', 'GR-3117', 'NO-9935', 'RU-40527', 'VN-4031'};

for i = 1:length(countries)
    % Steps
    % 1. Load the .csv files
    country = countries{i};
    adj = ['Datasets/Countries_Meshes/csv/' country '-adj.csv'];
    pts = ['Datasets/Countries_Meshes/csv/' country '-pts.csv'];
    M = readtable(adj);
    P = readtable(pts);
    
    % 2. Construct the adjaceny matrix (NxN). There are multiple ways
    %    to do so.
    from = M{:, 1}; % First column
    to = M{:, 2};   % Second column
    
    % Determine the number of nodes
    nodes = max([from; to]);
    
    % Use sparse to create the adjacency matrix
    W = sparse(from,to, 1, nodes, nodes);
    
    if ~issymmetric(W)
        W = (W+W')/2;
        disp('The adjacency matrix was not symmetric and has been symmetrized.');
    end
    
    % 3. Visualize the resulting graphs
    coords = P{:, 1:2};
    
    %figure;
    %gplotg(W, coords);
    %pause;
    
    % 4. Save the resulting graphs
    save_file = ['../Datasets/Countries_Mat/' country '_graph.mat'];
    save(save_file, "W", "coords");
    disp(['Graph saved for ', country, ' as ', save_file]);
end

% Example of the desired graph format for CH

load ../Datasets/Countries_Mat/NO-9935_graph.mat
adj_no      = W;
coords_no = coords;   

figure;
gplotg(adj_no,coords_no);
pause;

load ../Datasets/Countries_Mat/VN-4031_graph.mat
adj_vn      = W;
coords_vn = coords;   

figure;
gplotg(adj_vn,coords_vn);
