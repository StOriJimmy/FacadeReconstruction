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
% Invert each slice of an array.
% The inversion is done manually for matrices of dimension < 4
% for everything else the matrices are put in to a cell array 
% and cell fun is used. Cell fun becomes slow when the number of 
% matrices is large, probably because of the memeory allocation required
% for the conversion between cell and array.
%
% Input:
%   A - NxNxK array
%   
% Output:
%   B - NxNxK array where B(:,:,k) is the inverse of A(:,:,k)

function B = ArrayInvert(A)
  [M N K] = size(A);
  assert(M == N, 'ArrayInvert: Must be a square array');
  
  switch M
    case 2
      DetA = A(1,1,:).*A(2,2,:) - A(2,1,:).*A(1,2,:);
      
      Cofactors = [A(2,2,:), -A(1,2,:); ...
                  -A(2,1,:), A(1,1,:)];
                  
      B = (1 ./ repmat( DetA , [2 2 1])) .* Cofactors;
          
    case 3
      DetA =  A(1,1,:).*A(2,2,:).*A(3,3,:) + ...
              A(1,2,:).*A(2,3,:).*A(3,1,:) + ...
              A(1,3,:).*A(2,1,:).*A(3,2,:) - ...
              A(1,3,:).*A(2,2,:).*A(3,1,:) - ...
              A(1,2,:).*A(2,1,:).*A(3,3,:) - ...
              A(1,1,:).*A(2,3,:).*A(3,2,:);
              
      Cofactors = [ A(2,2,:).*A(3,3,:) - A(2,3,:).*A(3,2,:), ...
                    A(1,3,:).*A(3,2,:) - A(1,2,:).*A(3,3,:), ...
                    A(1,2,:).*A(2,3,:) - A(1,3,:).*A(2,2,:); ...
                    A(2,3,:).*A(3,1,:) - A(2,1,:).*A(3,3,:), ...
                    A(1,1,:).*A(3,3,:) - A(1,3,:).*A(3,1,:), ...
                    A(1,3,:).*A(2,1,:) - A(1,1,:).*A(2,3,:); ...
                    A(2,1,:).*A(3,2,:) - A(2,2,:).*A(3,1,:), ...
                    A(1,2,:).*A(3,1,:) - A(1,1,:).*A(3,2,:), ...
                    A(1,1,:).*A(2,2,:) - A(1,2,:).*A(2,1,:) ];
                    
       B = (1 ./ repmat( DetA, [3 3 1] )) .* Cofactors; 
    otherwise
      warns = warning;
      warning off; % temporarily turn warnings off
      C = mat2cell( A(:,:), N, repmat(N,1,K) );
      Cinv = cellfun(@inv, C, 'UniformOutput', false);
      B = reshape(horzcat(Cinv{:}), [M,M,K]);
      warning(warns); % put warnings back to original state
  end
endfunction
