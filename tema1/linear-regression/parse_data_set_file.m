function [Y, InitialMatrix] = parse_data_set_file(file_path)
  file_stream = fopen(file_path, "r");

  m = fscanf(file_stream, "%d", 1);
  n = fscanf(file_stream, "%d", 1);

  Y = zeros(m, 1);
  InitialMatrix = cell(m, n);

  for index = 1 : m
    Y(index) = fscanf(file_stream, "%d", 1);
    x = strsplit(fgetl(file_stream))(2 : end);
    InitialMatrix(index, 1 : end) = x;
  end

  fclose(file_stream);
end
