function [J, grad] = cost_function(params, X, y, lambda, input_layer_size, hidden_layer_size, output_layer_size)
  m = size(X, 1);

  input_layer_size = input_layer_size + 1;
  theta_size = input_layer_size * hidden_layer_size;
  theta1 = reshape(params(1 : theta_size), hidden_layer_size, input_layer_size);
  hidden_layer_size = hidden_layer_size + 1;
  theta2 = reshape(params(theta_size + 1 : end), output_layer_size, hidden_layer_size);

  a1 = transpose([ones(m, 1) X]);
  z2 = theta1 * a1;
  a2 = [ones(1, size(z2, 2)); sigmoid(z2)];
  z3 = theta2 * a2;
  a3 = sigmoid(z3);

  Y = eye(output_layer_size)(y, :);
  J = 1 / m * sum(sum((-Y)' .* log(a3) - (1 - Y)' .* log(1 - a3)));
  J = J + lambda / (2 * m) * (sum(sum(theta1(:, 2 : end) .^ 2)) + sum(sum(theta2(:, 2 : end) .^ 2)));

  sigma3 = a3 - transpose(Y);
  delta2 = sigma3 * transpose(a2);
  sigmoid_z2 = sigmoid([ones(1, size(z2, 2)); z2]);
  sigma2 = (transpose(theta2) * sigma3 .* (sigmoid_z2 .* (1 - sigmoid_z2)))(2 : end, :);
  delta1 = sigma2 * transpose(a1);
  
  grad_theta1 = 1 / m * delta1 + lambda / m * [zeros(size(theta1, 1), 1) theta1(:, 2 : end)];
  grad_theta2 = 1 / m * delta2 + lambda / m * [zeros(size(theta2, 1), 1) theta2(:, 2 : end)];
  grad = [grad_theta1(:); grad_theta2(:)];
end
