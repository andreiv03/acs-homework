function [train, miu, Y, Vk] = magic_with_pca(train_mat, pcs)
  [m, n] = size(train_mat);
  
  train = zeros(m, n);
  miu = zeros(1, n);
  Y = zeros(m, pcs);
  Vk = zeros(n, pcs);
  
  train_mat = double(train_mat);
  miu = sum(train_mat, 1) / m;
  train_mat = train_mat - miu;

  cov_matrix = transpose(train_mat) * train_mat / (m - 1);
  [V, S] = eig(cov_matrix);

  [~, indexes] = sortrows(diag(S), -1);
  V = V(:, indexes);
  
  Vk = V(:, 1 : pcs);
  Y = train_mat * Vk;

  train = Y * transpose(Vk);
end
