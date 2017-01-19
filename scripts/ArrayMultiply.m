%
% Multiple two arrays of matrices
%

function C = ArrayMultiply( A, B )
  [Ma Na La] = size(A);
  [Mb Nb Lb] = size(B);
  assert(all([Mb Lb] == [Na La]));
  
  C = sum(repmat(A, [Nb 1 1])  .* permute(reshape(repmat(B, [Ma,1,1]), [Mb, Ma*Nb, Lb]),[2 1 3]),2);
  C = reshape(C, [Ma, Nb, Lb]);
endfunction
