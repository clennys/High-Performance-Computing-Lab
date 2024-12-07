function handle = gploteigen(A,xy, part1, fiedler_vec)
scatter3(xy(:,1), xy(:,2), fiedler_vec, 50, fiedler_vec, 'filled');
colormap(jet); colorbar;
title('Fiedler Vector (\lambda_2)');
xlabel('x-coordinates');
ylabel('y-coordinates');
zlabel('Fiedler Vector Values');
hold on;

% Partition the graph based on part1
[n,n] = size(A);
part1 = part1(:);
part2 = 1:n;
part2(part1)=zeros(size(part1));
part2 = find(part2);
part2 = part2(:);
cut = spaugment(A(part1,part2),1);
cutxy = xy([part1; part2],:);

% Plot edges for part1 in gray
[edgeX, edgeY] = gplot(A(part1, part1), xy(part1, :));
line(edgeX, edgeY, 'Color',  [0.5, 0.5, 0.5] , 'LineWidth', 1); 
% Plot edges for part2 in light gray
[edgeX, edgeY] = gplot(A(part2, part2), xy(part2, :));
line(edgeX, edgeY, 'Color', [0.8, 0.8, 0.8], 'LineWidth', 1); 

% Plot cut edges in black
[edgeX, edgeY] = gplot(cut, cutxy);
line(edgeX, edgeY, 'Color', 'red', 'LineWidth', 1); 

hold off;

end