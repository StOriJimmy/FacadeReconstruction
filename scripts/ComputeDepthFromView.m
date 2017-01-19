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
% Compute a depth map from a collection of views and a mesh.
% The implementation uses a depth buffer (not opengl).
%
%
% Input:
%   I        - An image of size MxNxCxL
%   P        - A 3x4xL projection matrix
%   Vertices - A 3xK set of points
%   Faces    - A 3xH set of faces
%   
% Output:
%   DepthMaps - MxNxL matrix The depth value for each point on the image
%   VertexDepths - LxK depth for each vertex for view l
%
function [DepthMaps VertexDepths] = ComputeDepthFromView( I, P, Vertices, Faces )
  assert(all([size(P,1) size(P,2) size(Vertices,1) size(Faces,1)]==[3 4 3 3]), 
         "ComputeDepthFromView: Incorrect dimensions");
  assert(size(P,3)==size(I,4), ...
          "ComputeDepthFromView: The number of cameras and \
           images must be equal");
         
  [M N C L] = size(I);
  [~,K] = size(Vertices);
  [~,H] = size(Faces);
  DepthMaps = zeros(M,N,L);
  VertexDepths = zeros(L,K);
  TextOut(sprintf('Computing depth map for %d images', L),1);
  for k = 1 : L
    [DepthMaps(:,:,k) VertexDepths(k,:)] = ...
      zbufferoct(Vertices, Faces, P(:,:,k), M, N);
    Loading(k/L);
  end
endfunction
