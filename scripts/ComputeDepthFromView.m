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
