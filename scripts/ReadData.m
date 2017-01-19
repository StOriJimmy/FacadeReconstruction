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
% Read the camera matrices and images from the 2 dwarf folder
% The files are generated from MeshLab
%
% Input:
%  filename - The filename to read from.
%  target   - The target directory
%  method   - The method to call.
%

function [P K] = ReadData(filename, target, method)

  if(method == 0)
    disp("Extract the projection matrices");
    disp(filename);
    % Run the sed scripts
    names = {"TranslationVector", "RotationMatrix", "FocalMm", "CenterPx", "PixelSizeMm", "ViewportPx"};
    cols = [4 16 1 2 2 2];
    [tempor WordCount] = system(["./WordCount.sh " filename]);

    N = str2double(WordCount); % The number of cameras
    %PixelSize = 0.0137581;

    for k = 1 : length(names)
      system(["./OctaveMatrixHeader.sh ", names{k} ,sprintf(" %d %d",N,cols(k))]);
      system(["./CameraFromMLP.sh ", filename, ' ', names{k}, " >> ", names{k} ]);
      system(['mv ', names{k}, ' ', target]);
      load([target, '/', names{k}]);
    end
    
    system(["./NamesFromMLP.sh ", filename, " >> Names" ]);
    system(["mv Names ", target]);
    
    % Extract the camera matrices
    P = cell(1, N);
    K = cell(1, N);
    for l = 1 : N
      c = (FocalMm(l) * [1 1]) ./ (PixelSizeMm(l,:));
      K{l} = [-c(1) 0   CenterPx(l,1); ...
               0    c(2) CenterPx(l,2); ...
               0    0    1];
      R = reshape(RotationMatrix(l,:),4,4)';
      P{l} = K{l}*[R(1:3,1:3), R(1:3,1:3)*(TranslationVector(l,1:3)')];
    end
    
    save([target, '/P'], 'P');
    save([target, '/K'], 'K');
  end
  
  if(method == 1)
    disp("Read the ply data");
    disp(filename);
    PLYData = read_ply(filename);
    X = [PLYData.x' ; PLYData.y'; PLYData.z'];
    RGB = [PLYData.red' ;PLYData.green' ;PLYData.blue'; PLYData.alpha'];
    P = [];
    K = [];
    save([target,'/X'],'X');
    save([target,'/RGB'],'RGB');
    save([target,'/PLYData'], 'PLYData');
  end
endfunction
