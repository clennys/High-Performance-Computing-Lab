%
% D.P & O.S for the "HPC Course" at USI and
%                   "HPC Lab for CSE" at ETH Zurich

function [part1,part2] = bisection_inertial(A,xy,picture)
% bisection_inertial : Inertial partition of a graph.
%
% [p1,p2] = bisection_inertial(A,xy) returns a list of the vertices on one side of a partition
%     obtained by bisection with a line or plane normal to a moment of inertia
%     of the vertices, considered as points in Euclidean space.
%     Input A is the adjacency matrix of the mesh (used only for the picture!);
%     each row of xy is the coordinates of a point in d-space.
%
% bisection_inertial(A,xy,1) also draws a picture.


% disp(' ');
% disp(' HPC Lab at USI:   ');
% disp(' Implement inertial bisection');
% disp(' ');


% Steps
% 1. Calculate the center of mass.
xy_mean = mean(xy, 1);
x_mean = xy_mean(1);
y_mean = xy_mean(2);
% 2. Construct the matrix M.
%  (Consult the pdf of the assignment for the creation of M)
x_shift = xy(:,1) - x_mean;
y_shift = xy(:,2) - y_mean;
Sxx = sum(x_shift.^2);
Syy = sum(y_shift.^2);
Sxy = sum(x_shift.*y_shift);
M = [Syy, Sxy; Sxy, Sxx];
% 3. Calculate the smallest eigenvector of M.
[eig_vec,~] = eigs(M,1,1e-10);
% 4. Find the line L on which the center of mass lies.
eig_vec = eig_vec/norm(eig_vec);
% 5. Partition the points around the line L.
%   (you may use the function partition.m)

% cross product 1d vector with distance to L
% cross_product = x_shift * eig_vec(2) - y_shift * eig_vec(1);
% 
% n = size(A,1);
% map = zeros(n,1);
% map(0 <= cross_product) = 0;
% map(0 > cross_product) = 1;
[part1, part2] = partition(xy, eig_vec);

if picture == 1
    gplotpart(A,xy,part1);
    title('Inertial bisection (actual) using the Fiedler Eigenvector');
end

% <<<< Dummy implementation to generate a partitioning
%n   = size(A,1);
%map = zeros(n,1);
%map(1:round((n/2)))     = 0; 
%map((round((n/2))+1):n) = 1;


%[part1,part2] = other(map);

%if picture == 1
%    gplotpart(A,xy,part1);
 %   title('Inertial bisection (dummy) using the Fiedler Eigenvector');
%end

% Dummy implementation to generate a partitioning >>>>

end
