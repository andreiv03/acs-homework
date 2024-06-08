function [x, err, steps] = perform_iterative(G, c, x0, tol, max_steps)
	x = x0;
	err = inf;
	steps = 0;

	while err > tol && steps < max_steps
		x0 = x;
		x = G * x + c;
		err = norm(x - x0);
		steps = steps + 1;
	end
end
