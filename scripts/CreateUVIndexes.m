% ----------------------------------------------------------------------
%  Copyright (C) 2016 Daniel Beale and Luca Benedetti. All rights reserved.
% 
%  Permission is hereby granted, free of charge, to any person obtaining
%  a copy of this software and associated documentation files (the
%  "Software"), to deal in the Software without restriction, including
%  without limitation the rights to use, copy, modify, merge, publish,
%  distribute, sublicense, and/or sell copies of the Software, and to
%  permit persons to whom the Software is furnished to do so, subject to
%  the following conditions:
% 
%  The above copyright notice and this permission notice shall be
%  included in all copies or substantial portions of the Software.
% 
%  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
%  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
%  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
%  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
%  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
%  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
%  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
% ----------------------------------------------------------------------
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
