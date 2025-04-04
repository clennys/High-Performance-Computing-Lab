% Visualize information from the eigenspectrum of the graph Laplacian
%
% D.P & O.S for the "HPC Course" at USI and
%                   "HPC Lab for CSE" at ETH Zurich

% add necessary paths
addpaths_GP;

% Graphical output at bisection level
picture = 0;

% Cases under consideration
load airfoil1.mat;
air_W = Problem.A;
air_coords = Problem.aux.coord;

[part1, part2, eig_vec] = bisection_spectral(air_W, air_coords, 0);
eigvec1 = -eig_vec(:,1);
eigvec2 = -eig_vec(:,2);
% Visualize the entries of the eigenvectors
figure;
subplot(2, 1, 1);
plot(1:length(eigvec1), eigvec1, '-o', 'LineWidth', 0.5, 'MarkerSize', 3);
grid on;
title('Entries of the Eigenvector v_1 (λ_1)');
xlabel('Index');
ylabel('Value');

subplot(2, 1, 2);
plot(1:length(eigvec2), eigvec2, '-s', 'LineWidth', 0.5, 'MarkerSize', 3);
grid on;
title('Entries of the Eigenvector v_2 (λ_2)');
xlabel('Index');
ylabel('Value');
pause;
close all;

figure;
subplot(2, 1, 1);
G = graph(air_W); 
gplot(air_W, air_coords, '-o');
hold on;
scatter(air_coords(:, 1), air_coords(:, 2), 30, eigvec1, 'filled');
colorbar;
title('Eigenvector for \lambda_1');
xlabel('x-coordinates');
ylabel('y-coordinates');
hold off;

subplot(2, 1, 2);
gplot(air_W, air_coords, '-o')
hold on;
scatter(air_coords(:, 1), air_coords(:, 2), 30, eigvec2, 'filled');
colorbar;
title('Eigenvector for \lambda_2');
xlabel('x-coordinates');
ylabel('y-coordinates');
hold off;
pause;
close all;

gs = {
    %'airfoil1.mat';
    '3elt.mat';
    'barth4.mat';
    'mesh3e1.mat';
    'crack.mat'
    };
num_graphs = length(gs);
figure;

for i=1:num_graphs
    g = gs{i};
    load(g);
    W = Problem.A;
    xy = Problem.aux.coord;

    [part1, part2, eig_vec] = bisection_spectral(W, xy, 0);
    fiedler = -eig_vec(:,2);
    subplot(ceil(sqrt(num_graphs)), ceil(num_graphs / ceil(sqrt(num_graphs))), i);
    gploteigen(W,xy, part1, fiedler)
    title(gs{i}, 'Interpreter', 'none');
    
end
pause;
close all;
for i=1:length(gs)
    g = gs{i};
    load(g);
    W = Problem.A;
    xy = Problem.aux.coord;
    [part1, part2, eig_vec] = bisection_spectral(W, xy, 0);
    eig1 = eig_vec(:,2);
    eig2 = eig_vec(:,3);
    xy_v = [eig1, eig2];
    figure;
    subplot(2,1,1);
    gplot_v(W,xy_v, part1);
    title(gs{i}, 'Interpreter', 'none');
    subplot(2,1,2);
    gplot_v(W,xy, part1);
    title(gs{i}, 'Interpreter', 'none');
    pause;
    close all;
    
end

% load 3elt.mat;
% load barth4.mat;
% load mesh3e1.mat;
%load crack.mat;

% Initialize the cases
%W      = Problem.A;
%coords = Problem.aux.coord;

% Steps
% 1. Construct the graph Laplacian of the graph in question.
% 2. Compute eigenvectors associated with the smallest eigenvalues.
% 3. Perform spectral bisection.
% 4. Visualize:
%   i.   The first and second eigenvectors.
%   ii.  The second eigenvector projected on the coordinate system space of graphs.
%   iii. The spectral bi-partitioning results using the spectral coordinates of each graph.

