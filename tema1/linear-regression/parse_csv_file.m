function [Y, InitialMatrix] = parse_csv_file(file_path)
  file_stream = fopen(file_path, "r");

  line = fgetl(file_stream);
  n = numel(strfind(line, ","));

  Y = zeros(0, 1);
  InitialMatrix = cell(0, n);

  while ~feof(file_stream)
    line = strsplit(fgetl(file_stream), ",");
    Y = vertcat(Y, str2double(line{1}));
    InitialMatrix = vertcat(InitialMatrix, line(2 : end));
  end

  fclose(file_stream);
end
