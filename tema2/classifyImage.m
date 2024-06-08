function prediction = classifyImage(im, train_mat, train_val, pcs)
  prediction = -1;
  
  im = double(im);
  [train, miu, Y, Vk] = magic_with_pca(train_mat, pcs);
  
  m = size(train_mat, 1);
  miu = sum(train_mat, 1) / m;

  im = im - miu;
  im = im * Vk;
  
  prediction = KNN(train_val, Y, im, 5);
end
