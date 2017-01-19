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
