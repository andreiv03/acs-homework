function [X, y] = load_dataset(path)
  mat = load(path);
  X = mat.X;
  y = mat.y;
end
