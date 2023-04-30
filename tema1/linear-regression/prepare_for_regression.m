function [FeatureMatrix] = prepare_for_regression(InitialMatrix)
  [m, n] = size(InitialMatrix);
  FeatureMatrix = zeros(m, n);
  
  for row = 1 : m
    counter = 0;

    for col = 1 : n
      if isnumeric(str2double(InitialMatrix(row, col)))
        FeatureMatrix(row, col + counter) = str2double(InitialMatrix(row, col));
      end

      if strcmp(InitialMatrix(row, col), "yes")
        FeatureMatrix(row, col + counter) = 1;
      end

      if strcmp(InitialMatrix(row, col), "no")
        FeatureMatrix(row, col + counter) = 0;
      end
      
      if strcmp(InitialMatrix(row, col), "semi-furnished")
        FeatureMatrix(row, col + counter) = 1;
        FeatureMatrix(row, col + counter + 1) = 0;
        counter = counter + 1;
      end
      
      if strcmp(InitialMatrix(row, col), "unfurnished")
        FeatureMatrix(row, col + counter) = 0;
        FeatureMatrix(row, col + counter + 1) = 1;
        counter = counter + 1;
      end
      
      if strcmp(InitialMatrix(row, col), "furnished")
        FeatureMatrix(row, col + counter) = 0;
        FeatureMatrix(row, col + counter + 1) = 0;
        counter = counter + 1;
      end
    end
  end
end
