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
% Display a loading bar.
% The bar displays the total percentage and the estimated time to completion.
%
% Input:
%   Fraction - The fraction complete, one when computation is complete.
%
% Output:
%   -

function Loading( Fraction )
  persistent LoadingTime = 0;
  ts = terminal_size();
  c = '-';
  
  if(LoadingTime ~=0 )
    seconds = toc(LoadingTime);
  else
    seconds = 0;
    LoadingTime = tic;
  end
  
  percentage = ceil(Fraction*100);
  secondsleft = round(((1-Fraction)/(Fraction+eps))*seconds);
  eta = sprintf('~ %d:%02d ', floor(secondsleft/60), mod(secondsleft,60) );
  width = ts(2) - 7 - numel(eta);
  
  printf("%d%% %s", percentage, eta);
  printf('[');
  printf(repmat(c,1,ceil(width * Fraction)));
  printf(']');
  if(Fraction < 1)
    printf('\r');
  else
    printf('\n')
    LoadingTime = 0;
  end
  fflush(stdout);
endfunction
