function r = reproduce(s)
  r = '';
  if isfield(s, 'l')
    r = [r reproduce(s.l)];
  end
  if isfield(s, 'c') && numel(s.c)
    for k=1:numel(s.c)
      if k > 1
        printTxt(s)
      end
      r = [r reproduce(s.c(k))];
    end
  end
  if ~isfield(s, 'c') || numel(s.c) <= 1
    printTxt(s)
  end
  if isfield(s, 'r')
    r = [r reproduce(s.r)];
  end
  function printTxt(s)
    if isfield(s, 't')
      if isnumeric(s.t)
        r = [r sprintf('%.16g', s.t)];
      else
        r = [r s.t];
      end
    end
    if isfield(s, 'i')
      r = [r s.i];
    end
  end
end