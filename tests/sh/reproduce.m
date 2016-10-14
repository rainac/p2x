function r = reproduce(s)
  r = '';
  if isfield(s, 'l')
    r = [r reproduce(s.l)];
  end
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
  if isfield(s, 'r')
    r = [r reproduce(s.r)];
  end
