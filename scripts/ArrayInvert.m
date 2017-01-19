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
