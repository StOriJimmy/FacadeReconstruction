%
% Project a point on to a general camera
%
% Input:
%   P - A 3x4 camera matrix
%   X - A 4x1 homogeneous world point
%
% Output:
%   x - A 2x1 camera point
%

function x = ProjectPoint(P, X)
  if(size(X,1) == 3)
    X = [X; ones(1,size(X,2))];
  end
  x = P* X; % project the mean on to the camera
  x = x(1:end-1,:) ./ repmat(x(end,:), 2,1);
endfunction
