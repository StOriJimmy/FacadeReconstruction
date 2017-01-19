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
% Write a collection of verteces, colours and faces to a ply file.
%
% Input:
%   Verts   - Mx3 matrix of verteces
%   Colours - Mx3 matrix of colours
%   Faces   - Nx3 indexes in to the vertecies, a triangle for each face
%
% Output:
%   -
%
function WritePly( Verts, Colours, Faces, FileName, UV=[], texim='')
  assert(size(Verts,2)==3, ...
         "WritePly: Verts must be Nx3");
  assert(size(Colours,2)==3 || numel(Colours)==0,...
         "WritePly: Colours must Nx3");
  assert(size(Faces,2)==3 || numel(Faces)==0, ...
         "WritePly: Faces must Kx3");
  assert(size(Verts,1)==size(Colours,1) || numel(Colours)==0, ...
         "WritePly: Verts and Colours must be the same size");
         
  
  fd = fopen(FileName, 'w+');
  fprintf(fd, 'ply\n');
  fprintf(fd, 'format ascii 1.0\n');
  fprintf(fd, 'comment Daniel Beale (c) 2014\n');
  if(numel(UV) > 0)
    fprintf(fd, 'comment TextureFile %s\n', texim);
  end
  fprintf(fd, 'element vertex %d\n', size(Verts,1));
  fprintf(fd, 'property float x\n');
  fprintf(fd, 'property float y\n');
  fprintf(fd, 'property float z\n');
  if( numel(UV) > 0 )
    fprintf(fd, 'property float texture_u\n');
    fprintf(fd, 'property float texture_v\n');
  else
    if(numel(Colours)>0)
      fprintf(fd, 'property uchar red\n');
      fprintf(fd, 'property uchar green\n');
      fprintf(fd, 'property uchar blue\n');
    end
  end
  fprintf(fd, 'element face %d\n', size(Faces,1));
  fprintf(fd, 'property list uchar int vertex_indices\n');
  fprintf(fd, 'end_header\n');
  if(numel(UV) > 0)
    %fprintf(fd, '%f %f %f %f %f %d %d %d\n', [Verts UV Colours]');
    fprintf(fd, '%f %f %f %f %f\n', [Verts UV]');
    fprintf(fd, '%d %d %d %d\n', [repmat(3,size(Faces,1),1) Faces]');
  else
    if(numel(Colours)>0)
      fprintf(fd, '%f %f %f %d %d %d\n', [Verts Colours]');
    else
      fprintf(fd, '%f %f %f\n', [Verts]');
    end
    fprintf(fd, '%d %d %d %d\n', [repmat(3,size(Faces,1),1) Faces]');
  end
  fclose(fd);
endfunction
