function dorep()
  str = fread(0, inf, '*char');
  fd = fopen('in.m', 'w');
  fprintf(fd, '%s', str);
  fclose(fd);
  %  fprintf('in: %s\n', str);
  % eval(str);
  run('in.m');
  s = ans;
  rep = reproduce(s);
  fprintf('%s', rep);

