function [path] = heuristic_greedy(start_position, probabilities, Adj)
	[m, n] = size(Adj);
	
	path = [start_position];
	visited(start_position) = 1;

	while ~isempty(path)
		position = ;

		if position == m * n - 1
			return
		end


	end
end
