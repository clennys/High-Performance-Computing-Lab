function handle = gplotg3D(A, xyz, lc)
    % GPLOTG3D: Plot graph edges in 3D
    %
    % A      - Adjacency matrix
    % xyz    - 3D coordinates of nodes
    % lc     - Line style/color

    if nargin < 3
        lc = 'r-';
    end

    [i, j] = find(A);
    [~, p] = sort(max(i, j));
    i = i(p);
    j = j(p);

    % Create NaN-separated lists for 3D lines
    X = [xyz(i, 1), xyz(j, 1), NaN(size(i))]';
    Y = [xyz(i, 2), xyz(j, 2), NaN(size(i))]';
    Z = [xyz(i, 3), xyz(j, 3), NaN(size(i))]';

    % Flatten for 3D plotting
    X = X(:);
    Y = Y(:);
    Z = Z(:);

    % Plot in 3D
    handle = plot3(X, Y, Z, lc);
end
