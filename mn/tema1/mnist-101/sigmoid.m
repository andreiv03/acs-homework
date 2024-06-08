function [y] = sigmoid(x)
  y = 1 ./ (1 + e .^ (-x));
end
