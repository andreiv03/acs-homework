function [classes] = predict_classes(X, weights, input_layer_size, hidden_layer_size, output_layer_size)
  m = size(X, 1);

  input_layer_size = input_layer_size + 1;
  theta_size = input_layer_size * hidden_layer_size;
  theta1 = reshape(weights(1 : theta_size), hidden_layer_size, input_layer_size);
  hidden_layer_size = hidden_layer_size + 1;
  theta2 = reshape(weights(theta_size + 1 : end), output_layer_size, hidden_layer_size);

  a1 = transpose([ones(m, 1) X]);
  z2 = theta1 * a1;
  a2 = [ones(1, size(z2, 2)); sigmoid(z2)];
  z3 = theta2 * a2;
  a3 = sigmoid(z3);

  [~, classes] = max(a3, [], 1);
  classes = transpose(classes);
end
