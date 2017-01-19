%
% Display text at the specified level.
%
% This function also clears the output buffer
%
% Input:
%   Text  - The text to be displayed
%   Level - The level at which it should be displayed
%
% 

function TextOut(Text, Level=0)
  persistent BashSupportsColour;
  global verbosity;
  
  if(~isempty(verbosity))
    if(Level > verbosity)
      return;
    end
  end
  
  if(isempty(BashSupportsColour))
    %[~, sc] = system('tput colors');
    BashSupportsColour = 0; % str2num(sc)==256; 
  end
  
  if(Level == 0)
    printf('\n');
    fflush(stdout);
    Print(Text, '1;37', BashSupportsColour);
    printf('\n');
    fflush(stdout);
    return;
  elseif( Level > 0 )
    Print(repmat('--', 1, Level), '1;34', BashSupportsColour);
    Print('> ', '1;34', BashSupportsColour);
    Print(Text, '1;37', BashSupportsColour);
    printf('\n');
    fflush(stdout);
  elseif(Level < 0)
    ts = terminal_size();
    ts = ts(2);
    Print( [repmat('-', 1, ts), '\n'], '1;37', BashSupportsColour);
    Print( [' | ', repmat(' ', 1, ts-6), ' | \n'], '1;37', BashSupportsColour );
    Lines = strsplit(Text, "\n");
    
    for k = 1 : length(Lines)
      N = numel(Lines{k});
      starte = floor((ts-6-N)/2);
      Print( [' | ', repmat(' ', 1, starte)], '1;37', BashSupportsColour);
      Print( Lines{k}, '1;37', BashSupportsColour);
      Print( [repmat(' ', 1, starte+mod(ts-6-N,2)), ' | \n' ], '1;37', BashSupportsColour);
      fflush(stdout);
    end
    
    Print( [' | ', repmat(' ', 1, ts-6), ' | \n'], '1;37', BashSupportsColour);
    Print( [repmat('-', 1, ts), '\n'], '1;37', BashSupportsColour);
    fflush(stdout);
  end
endfunction

function Print(Text, Colour, SupportsColour)
  if(SupportsColour)
    system(['printf "\e[', Colour, 'm', Text, '%s\e[m"']);
  else
    printf(Text);
  end
endfunction

