function [decoded_path] = decode_path(path, lines, cols)
	decoded_path = zeros(length(path) - 1, 2);
	
	for index = 1 : length(path) - 1
		node = path(index);

		row = ceil(node / cols);
		col = mod(node - 1, cols) + 1;
		
		decoded_path(index, :) = [row, col];
	end
end
