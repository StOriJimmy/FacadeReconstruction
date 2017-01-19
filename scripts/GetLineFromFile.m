%
% Return a line, given the line number, and a file handle
%

function s = GetLineFromFile(File, linenumber)
  fh = fopen(File);
  for k=1:linenumber
    s=fgets(fh);
    if(s==-1)
      break;
    end
  end
  fclose(fh);
endfunction
