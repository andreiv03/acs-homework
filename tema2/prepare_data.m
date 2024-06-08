function [train_mat, train_val] = prepare_data(name, no_train_images)
  n = 784;

  train_mat = zeros(no_train_images, n);
  train_val = zeros(1, no_train_images);
  
  d = load(name);
  train_mat = d.trainX;
  train_val = d.trainY;

  train_mat = train_mat(1 : no_train_images, :);
  train_val = train_val(1 : no_train_images);
end
