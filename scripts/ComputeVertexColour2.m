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
% This is a new attempt at vertex colouring, which 
% computes the colour of each face, as opposed to each 
% vertex. This should make it easier to create duplicate colours 
% for the uv indexes.
%
function [NewColours, D, VD, UVIndexes, NewVertices, NewFaces] = ...
            ComputeVertexColour2(Vertices, Faces, Normals, ...
                                I, P, ...
                                DepthThresh=0.1, ...
                                NormalThresh=-0.4, ...
                                StartPoint = 3)
  RandomiseCameraOrder=false;
                                
  [D VD] = ComputeDepthFromView( I, P, Vertices, Faces );
  
  Normals = Normals ./repmat(sqrt(sum(Normals.^2)),3,1);
  [M,N,C,L1] = size(I); 
  assert(C==3, "ComputeVertexColour: the image must have colour")
  [~,~,L] = size(P);
  assert(L1==L);
  K = size(Vertices,2);
  IP = permute(reshape(I,[M*N C L]), [2 1 3]);

  % Compute camera positions
  Null = permute(ArrayMultiply( -ArrayInvert(P(1:3,1:3,:)), P(:,end,:) ), ...
                 [1 3 2]);
  
  NewVertices = [];
  NewFaces = [];
  NewColours = [];
  UVIndexes = [];
  visited = zeros(1,K);
  
  TextOut('Compute colours',1);
  K2 = 0;
  for DoNormals = [1 0] 
      % Fill all visible regions with best colour from the best camera 
      % according to the normals and also the depths.
      % For the remaining regions, fill in the best view
    if(DoNormals==1)
      TextOut('First run',2);
    else
      TextOut('Filling unfilled texture',2);
    end
    index = 1;
    if(RandomiseCameraOrder)
      visitedPoints = [];
      currentPoint = StartPoint;  % 
    else
      currentPoint = index; %StartPoint;  % 
    end
    while(index <= L)
      CCam = Null(:,currentPoint);
      
      % Find camera normal
      if(DoNormals==1)
        CamCenter = [N/2;M/2;1];
        BX = pinv(P(:,:,currentPoint))*CamCenter;
        BX = BX/BX(4);
        NormX = BX(1:3)-CCam(1:3);
        NormX = NormX ./ norm(NormX);
        DotProd = NormX'*Normals;
      end
      
      % Project down the vertices
      x = floor(ProjectPoint(P(:,:,currentPoint),Vertices)+1);
      
      okbounds = find(x(1,:) > 0 & x(2,:) > 0 & x(1,:) < N & x(2,:) < M);
      ind = sub2ind([M,N], x(2,okbounds), x(1,okbounds));
      if(DoNormals == 1) % find all vertices which have a resonable surface normal
        dpindex = DotProd < NormalThresh;
        withintol = abs(D(ind + M*N*(currentPoint-1)) - ...
                      VD(currentPoint,okbounds)) < DepthThresh ...
                      & D(ind + M*N*(currentPoint-1)) > 0;
      else
        dpindex = true(1,size(Vertices,2));
        withintol = abs(D(ind + M*N*(currentPoint-1)) - ...
                      VD(currentPoint,okbounds)) < DepthThresh ...
                      & D(ind + M*N*(currentPoint-1))>0;
      end
      
      % Find the vertices that are within the correct distance tolerance
      inokbd = okbounds(withintol & ~visited(okbounds) & dpindex(okbounds) );
      
      % this part of the code coulors based on the faces
      % this allows us to duplicate vertices across occlusion
      % boundaries
      if(true)
        % Find all of the faces which are contained in the vertex set
        wwf = ismember(Faces(1,:)+1, inokbd) | ...
              ismember(Faces(2,:)+1, inokbd) | ...
              ismember(Faces(3,:)+1, inokbd);
        ContainedFaces = Faces(:,wwf) + 1;
        
        nindecies = unique(ContainedFaces(:)); % indexes in to the vertex
        gnin = ismember(nindecies,okbounds);
        nindecies = nindecies(gnin); % keep the ones within bounds
        
        [~,findex] = ismember(ContainedFaces, nindecies); % find the face index
        
        findex(:,any(findex==0))=[];
        
        NewFaces = [NewFaces, findex+size(NewVertices,2)-1];
        NewVertices = [NewVertices, Vertices(:,nindecies)];
        
        indn = sub2ind([M,N], x(2,nindecies), x(1,nindecies));
        NewColours = [NewColours, IP(:, ind(gnin),currentPoint)];
        UVIndexes = [UVIndexes, [x(:,nindecies); ...
                                 currentPoint*ones(1,length(nindecies))]];
      end
      visited(inokbd) = true; 
      
      
      index = index + 1;
      if(RandomiseCameraOrder)
        visitedPoints=[visitedPoints, currentPoint];
        
        if(L-length(visitedPoints)>0)
          sd = setdiff(1:L,visitedPoints);
          [~,ii] = max(sum((Null(:,sd) - ...
                   repmat(CCam,1,L-length(visitedPoints))).^2));
          currentPoint = sd(ii);
        end
      else
        currentPoint = index;
      end
      
      Loading((1-DoNormals)/2 + 0.5*((index-1) / L));
    end
  end
  
  if((1-DoNormals)/2 + 0.5*((index-1) / L) < 1)
    Loading(1);
  end
  
  TextOut('Finalising with grays',1);
  IndexesNotDone = find(~visited);
  wwf = ismember(Faces(1,:)+1, IndexesNotDone) | ...
        ismember(Faces(2,:)+1, IndexesNotDone) | ...
        ismember(Faces(3,:)+1, IndexesNotDone);
  ContainedFaces = Faces(:,wwf) + 1;
  nindex = unique(ContainedFaces(:));
  [~,findex] = ismember(ContainedFaces, nindex);
  findex(:,any(findex==0))=[];
  
  NewFaces = [NewFaces, findex+size(NewVertices,2)-1];
  NewVertices = [NewVertices, Vertices(:,nindex)];
  NewColours = [NewColours, repmat([30;30;30], 1, length(nindex))];
  UVIndexes = [UVIndexes, repmat([0;0;1],1,length(nindex))];
  
  NewColours = double(NewColours);
endfunction
