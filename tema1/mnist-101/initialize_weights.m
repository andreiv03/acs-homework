function [matrix] = initialize_weights(L_prev, L_next)
  epsilon = sqrt(6) / sqrt(L_prev + L_next);
  matrix = rand(L_prev + 1, L_next);
  matrix = matrix * 2 * epsilon - epsilon;
end
