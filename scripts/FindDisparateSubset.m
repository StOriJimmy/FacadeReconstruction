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

