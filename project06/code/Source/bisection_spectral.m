function [part1,part2, eig_vec] = bisection_spectral(A,xy,picture)
% bisection_spectral : Spectral partition of a graph.
%
% D.P & O.S for the "HPC Course" at USI and
%                   "HPC Lab for CSE" at ETH Zuric
%
% [part1,part2] = bisection_spectral(A) returns a partition of the n vertices
%                 of A into two lists part1 and part2 according to the
%                 spectral bisection algorithm of Simon et al:
%                 Label the vertices with the components of the Fiedler vector
%                 (the second eigenvector of the Laplacian matrix) and partition
%                 them around the median value or 0.



% disp(' ');
% disp(' HPC Lab at USI:   ');
% disp(' Implement spectral bisection');
% disp(' ');


% Steps
% 1. Construct the Laplacian.
D = diag(sum(A, 2));
L = D - A;
% 2. Calculate its eigensdecomposition.
%[eig_vec,~] = eigs(L,2,'smallestabs');
[eig_vec,~] = eigs(L,3,1e-10);
% 3. Label the vertices with the components of the Fiedler vector.
fiedler_vec = eig_vec(:,2);
% 4. Partition them around their median value, or 0.
fiedler_med = median(fiedler_vec);

n = size(A,1);
map = zeros(n,1);
map(fiedler_vec <= fiedler_med) = 0;
map(fiedler_vec > fiedler_med) = 1;
[part1, part2] = other(map);

if picture == 1
    gplotpart(A,xy,part1);
    title('Spectral bisection (actual) using the Fiedler Eigenvector');
end



% <<<< Dummy implementation to generate a partitioning
%n = size(A,1);
%map = zeros(n,1);
%map(1:round((n/2))) = 0; 
%map((round((n/2))+1):n) = 1;
%[part1,part2] = other(map);

%if picture == 1
%    gplotpart(A,xy,part1);
%    title('Spectral bisection (dummy) using the Fiedler Eigenvector');
%end

% Dummy implementation to generate a partitioning >>>>


end
