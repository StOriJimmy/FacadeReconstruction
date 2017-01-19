%
% Return the number of lines in the given filename
%
function nl=NumberOfLines(filename)
  [~,cmd] = system(['cat ', filename, ' | wc -l']);
  nl = str2double(cmd);
endfunction
