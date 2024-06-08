function new_X = task2(photo, pcs)
  [m, n] = size(photo);
  new_X = zeros(m, n);

  photo = double(photo);
  miu = sum(photo, 2) / n;
  photo = photo - miu;

  Z = transpose(photo) / sqrt(n - 1);
  [~, ~, V] = svd(Z);

  W = V(:, 1 : pcs);
  Y = transpose(W) * photo;

  new_X = W * Y + miu;
  new_X = uint8(new_X);
end
