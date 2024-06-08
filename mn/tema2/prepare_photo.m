function sir = prepare_photo(im)
  sir = zeros(1, 784);
  im = 255 - im;
  sir = reshape(transpose(im), 1, 784);
end
