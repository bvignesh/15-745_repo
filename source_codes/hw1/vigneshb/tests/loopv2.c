int g;
int g_mess (int c, int d,...)
{
  int temp;
  temp=c+d;
  temp-=g;
  return g*2;
}
int g_incr (int c)
{
  g += c;
  g_mess(g,c);
  g_mess(g,c);
  g_mess(g,c);
  g_mess(g,c);
  return g;
}
int loop (int a, int b, int c)
{
  int i;
  int ret = 0;
  for (i = a; i < b; i++) {
   g_incr (c);
   g_mess(g,c);
  }
  return ret + g;
}

