function new_X = task3(photo, pcs)
  [m, n] = size(photo);
  new_X = zeros(m, n);
  
  photo = double(photo);
  miu = sum(photo, 2) / n;
  photo = photo - miu;

  Z = photo * transpose(photo) / (n - 1);
  [V, ~] = eig(Z);

  W = V(:, end - pcs + 1 : end);
  Y = transpose(W) * photo;

  new_X = W * Y + miu;
  new_X = uint8(new_X);
end
