function [Adj] = get_adjacency_matrix(Labyrinth)
	[m, n] = size(Labyrinth);

	Adj = zeros(m * n + 2);
	Adj(m * n + 1, m * n + 1) = 1;
	Adj(m * n + 2, m * n + 2) = 1;

	di = [0, 0, 1, -1]; % directions for i
	dj = [-1, 1, 0, 0]; % directions for j

	for i = 1 : m
		for j = 1 : n
			bits = bitget(Labyrinth(i, j), 1 : 4);
			node = (i - 1) * n + j;

			for d = 1 : 4
				if bits(d) == 0
					in = i + di(d); % neigh i
					jn = j + dj(d); % neigh j

					if is_inside(in, jn, m, n)
						neigh = (in - 1) * n + jn;
						Adj(node, neigh) = 1;
					else
						if d == 1 || d == 2
							Adj(node, m * n + 2) = 1;
						else
							Adj(node, m * n + 1) = 1;
						end
					end
				end
			end
		end
	end

	function result = is_inside(i, j, m, n)
		if i >= 1 && i <= m && j >= 1 && j <= n
			result = 1;
		else
			result = 0;
		end
	end
end
