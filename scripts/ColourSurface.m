#!/usr/local/bin/octave

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

#
# Colour the surface
#
# ./ColourSurface.m pathtomlpfile tempfolder pathtoplyfile outputply (NumberOfImages) (NormalThreshold)
# ./ColourSurface.m pathtomlpfile tempfolder pathtoplyfile outputply ("[listofimages]") (NormalThreshold)

pkg load image
pkg load statistics

L = strsplit(mfilename ("fullpath"),'/');
L(end)=[];
lib = sprintf('%s/',L{:});
cd(lib);

addpath([pwd,'/oct']);

args = argv;

mlpfile = args{1};
tempfolder = args{2};
plyfile = args{3};
outputply = args{4};

NumberOfImages = 5;
if(length(args)>=5)
  NumberOfImages = eval(args{5});
end

DepthThresh = 0.05;
NormalThresh = -0.4;  % Lower gives a more stringent threshold
if(length(args)>=6)
  NormalThresh = str2num(args{6});
end


spl = strsplit(mlpfile,{'.','/'});
mlpfolder = sprintf('%s/',spl{1:end-2});

[P, K] = ReadData(mlpfile, tempfolder, 0);
N = NumberOfLines([tempfolder,'/Names']);

% Choose 5 views (this can be changed)
%findex = round(linspace(1,N,NumberOfImages));
%pms = randperm(N);
%findex = sort(pms(1:NumberOfImages));
Pn = cat(3,P{:});
Null = permute(ArrayMultiply( -ArrayInvert(Pn(1:3,1:3,:)), Pn(:,end,:) ), ...
                 [1 3 2]);
if(numel(NumberOfImages)==1)
  findex = FindDisparateSubset( Null, NumberOfImages );
else
  findex = NumberOfImages;
end

P = cat(3, P{findex});

TextOut('Loading images',1);
I = cell(1,N);
ii=1;
for k = findex
  imnames = GetLineFromFile([tempfolder,'/Names'],k);
  I{ii} = imread([mlpfolder,'/',imnames(1:end-1)]);
  ii=ii+1;
  Loading(k/N);
end
Loading(1);

Im = cat(4,I{:});
ImS = cat(1,I{:});

TextOut('Loading ply file',1);
[v, f] = read_ply(plyfile);
V = [v.x,v.y,v.z]';
N = [v.nx, v.ny, v.nz]';
F = f(:,[1 3 2])'-1;

TextOut('Computing colour',1);
[Colour, Depth, VD, UVIndexes, Ve, Fe] = ...
    ComputeVertexColour2(V, F, N, Im, P, DepthThresh, NormalThresh);
    
[UV, texIm] = CreateUVIndexes(UVIndexes, size(Im,1), size(Im,2), I);

spl = strsplit(outputply,{'.','/'});
texture_file = [sprintf('%s/',spl{1:end-2}), spl{end-1},'_tex.png'];

TextOut('Writing texture file',1);
ts = size(texIm);
rs = 30000;
if(ts(1)>rs)
  TextOut('Resize',2);
  texImr = texIm(round(linspace(1,ts(1),rs)), :,:);
else
  texImr = texIm;
end
%texImr = imresize(texIm,[round(ts(1)/4), ts(2)]);
size(texImr)
TextOut('Write',2);
imwrite(texImr, texture_file);

TextOut('Writing ply',1);
WritePly(Ve', []', Fe', outputply, UV(1:2,:)', strsplit(texture_file,'/'){end});

TextOut('Done', 1);
