%
% Create the proper uv indexes from the output of the colouring algorithm.
%
function [UV TexIm]= CreateUVIndexes(UVIndexes, M, N, Im=[])
  if(numel(Im)>0)
    TexIm = cat(1,Im{:});
  end
  NIMS = cellfun(@numel,Im);
  NI = sum(NIMS>0);

  % finalise the uv indexes
  UV = zeros(2,size(UVIndexes,2));
  UV(1,:) = (UVIndexes(1,:) ./ (N-1));
  UV(2,:) = UVIndexes(2,:);

  ImageNumbers = unique(UVIndexes(3,:));
  ImageNumbers(ImageNumbers==0)=[];
  
  for k = ImageNumbers
    BinaryIndex = UVIndexes(3,:)==k;
    UV(2,BinaryIndex) = UV(2,BinaryIndex) + (k-1)*M;
    Loading(k/max(ImageNumbers));
  end
  
  UV(2,:) = UV(2,:) ./ (NI*M);
  UV(2,:) = 1-UV(2,:);
 
endfunction
