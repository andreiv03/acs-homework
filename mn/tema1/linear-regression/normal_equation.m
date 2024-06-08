function [Theta] = normal_equation(FeatureMatrix, Y, tol, iter)  
  A = FeatureMatrix' * FeatureMatrix;
  b = FeatureMatrix' * Y;
  
  [m, n] = size(A);
  Theta = zeros(n, 1);

  if ~all(eig(A) > 0) || m ~= n
    Theta = [0; Theta];
    return;
  end

  r = b - A * Theta;
  v = r;
  tol_squared = tol ^ 2;
  k = 1;

  while k <= iter && transpose(r) * r > tol_squared
    t = (transpose(r) * r) / (transpose(v) * A * v);
    Theta = Theta + t * v;
    r0 = r;
    r = r0 - t * A * v;
    s = (transpose(r) * r) / (transpose(r0) * r0);
    v0 = v;
    v = r + s * v0;
    k = k + 1;
  end

  Theta = [0; Theta];
end
