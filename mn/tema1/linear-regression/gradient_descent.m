function [Theta] = gradient_descent(FeatureMatrix, Y, n, m, alpha, iter)
  Theta = zeros(n, 1);

  while iter
    H = FeatureMatrix * Theta;
    gradient = 1 / m * transpose(FeatureMatrix) * (H - Y);
    Theta = Theta - alpha * gradient;
    iter = iter - 1;
  end

  Theta = vertcat(0, Theta);
end
