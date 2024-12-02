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

D = diag(sum(air_W, 2));
L = D - air_W;

[eigenvectors, eigenvalues] = eig(full(L));
eigenvalues = diag(eigenvalues);

% Sort eigenvalues and eigenvectors
[sorted_eigenvalues, idx] = sort(eigenvalues, 'ascend');
sorted_eigenvectors = eigenvectors(:, idx);

% Extract eigenvectors for λ1 and λ2
eigvec1 = sorted_eigenvectors(:, 1); % First eigenvector (associated with λ1)
eigvec2 = sorted_eigenvectors(:, 2); % Second eigenvector (associated with λ2)

% Visualize the entries of the eigenvectors
figure;
scatter(air_coords(:, 1), air_coords(:, 2), 30, eigvec1, 'filled');
colorbar;
title('Entries of Eigenvector for \lambda_1');
xlabel('x-coordinates');
ylabel('y-coordinates');

figure;
scatter(air_coords(:, 1), air_coords(:, 2), 30, eigvec2, 'filled');
colorbar;
title('Entries of Eigenvector for \lambda_2');
xlabel('x-coordinates');
ylabel('y-coordinates');


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

