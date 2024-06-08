function im = visualise_image(train_mat, number)
  im = zeros(28, 28);
  row = train_mat(number, :);
  im = transpose(reshape(row, 28, 28));
  im = uint8(im);
end
