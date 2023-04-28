function [Link] = get_link_matrix(Labyrinth)
	[m, n] = size(Labyrinth);

	Link = sparse(m * n + 2, m * n + 2);
	Link(m * n + 1, m * n + 1) = 1;
	Link(m * n + 2, m * n + 2) = 1;

	di = [0, 0, 1, -1]; % directions for i
	dj = [-1, 1, 0, 0]; % directions for j

	for i = 1 : m
		for j = 1 : n
			bits = bitget(Labyrinth(i, j), 1 : 4);
			bits_0 = sum(bitget(Labyrinth(i, j), 1 : 4) == 0);
			node = (i - 1) * n + j;

			for d = 1 : 4
				if bits(d) == 0
					in = i + di(d); % neigh i
					jn = j + dj(d); % neigh j

					% check if neighbor is within bounds
					if in >= 1 && in <= m && jn >= 1 && jn <= n
						neigh = (in - 1) * n + jn;
						Link(node, neigh) = 1 / bits_0;
					else
						if d == 1 || d == 2
							Link(node, m * n + 2) = 1 / bits_0;
						else
							Link(node, m * n + 1) = 1 / bits_0;
						end
					end
				end
			end
		end
	end
end
