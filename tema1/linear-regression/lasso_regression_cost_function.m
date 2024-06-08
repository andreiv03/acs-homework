function [Error] = lasso_regression_cost_function(Theta, Y, FeatureMatrix, lambda)
  m = size(Y, 1);
	Theta = Theta(2 : end);
	H = FeatureMatrix * Theta;
	Error = 1 / m * sum((Y - H) .^ 2) + lambda * sum(abs(Theta));
end
