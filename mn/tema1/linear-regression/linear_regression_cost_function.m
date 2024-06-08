function [Error] = linear_regression_cost_function(Theta, Y, FeatureMatrix)
	m = size(Y, 1);
	Theta = Theta(2 : end);
	H = FeatureMatrix * Theta;
	Error = 1 / (2 * m) * sum((H - Y) .^ 2);
end
