function prediction = KNN(labels, Y, test, k)
  prediction = -1;
  
  [m, n] = size(Y);
  distance = zeros(m, 1);
  
  for i = 1 : m
    distance(i) = sqrt(sum((Y(i, :) - test) .^ 2));
  end
  
  [~, indexes] = sort(distance);
  prediction = median(labels(indexes(1 : k)));
end
