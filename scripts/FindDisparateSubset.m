%
% Given a collection of vectors find a 
% subset which maximally distant.
%
% This currently uses a greedy algorithm which
% is randomly initialised
% 
% Input:
%   Vecs - MxK set of vectors
%   N    - The size of the subset

function ind = FindDisparateSubset( Vecs, N )
  [M K] = size(Vecs);
  sp = ceil(rand*N);
  
  ntdone = true(1,K);
  
  ntdone(sp)=0;
  for k = 1 : N-1
    in = find(~ntdone);
    d2 = zeros(1,sum(ntdone));
    for l = 1 : length(in)
      d2=d2+sum((repmat(Vecs(:,l),1,sum(ntdone)) - Vecs(:,ntdone)).^2);
    end
    [~,ii] = max(d2);
    ind=find(ntdone);
    ntdone(ind(ii))=0;
  end
  ind = find(~ntdone);
endfunction

