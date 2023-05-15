function new_X = task1(photo, k)
  [m, n] = size(photo);
  new_X = zeros(m, n);
  
  photo = double(photo);
  [U, S, V] = svd(photo);

  Uk = U(:, 1 : k);
  Sk = S(1 : k, 1 : k);
  Vk = V(:, 1 : k);

  new_X = Uk * Sk * transpose(Vk);
  new_X = uint8(new_X);
end
