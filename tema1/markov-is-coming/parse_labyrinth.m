function [Labyrinth] = parse_labyrinth(file_path)
  file_stream = fopen(file_path, "r");

  m = fscanf(file_stream, "%d", 1);
  n = fscanf(file_stream, "%d", 1);
  Labyrinth = fscanf(file_stream, "%d", [n, m])';

  fclose(file_stream);
end
