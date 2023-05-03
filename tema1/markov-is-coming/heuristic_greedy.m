function [path] = heuristic_greedy(start_position, probabilities, Adj)
	size = size(Adj, 1) - 2;

	probabilities(size + 1) = 1;
	probabilities(size + 2) = 0;

	path = [start_position];
	visited = zeros(size + 2);
	visited(start_position) = 1;

	while ~isempty(path)
		position = path(end);

		if position == size + 1
			break;
		end

		neighs = find(Adj(position, :));
		neighs = neighs(visited(neighs) == 0);

		if isempty(neighs)
			path = path(1 : end - 1);
		else
			max_prob = -inf;
			max_prob_index = -1;
			
			for index = 1 : length(neighs)
				prob = probabilities(neighs(index));

				if prob > max_prob
					max_prob = prob;
					max_prob_index = index;
				end
			end

			neigh = neighs(max_prob_index);
			visited(neigh) = 1;
			path = [path neigh];
		end
	end
end
