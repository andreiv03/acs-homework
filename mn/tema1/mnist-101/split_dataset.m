function [X_train, y_train, X_test, y_test] = split_dataset(X, y, percent)
  m = size(X, 1);

  dataset = randperm(m);
  train = dataset(1 : floor(m * percent));
  test = dataset(floor(m * percent) + 1 : end);

  X_train = X(train, :);
  y_train = y(train);
  X_test = X(test, :);
  y_test = y(test);
end
