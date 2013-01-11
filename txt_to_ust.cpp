#include <cstdlib>
#include <cstdio>
#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
  if(argc==1)
  {
    int l;
    while(cin>>l)
    {
      putc(l/256, stdout);
      putc(l%256, stdout);
    }
    return 0;
  }
  int c1, c2;
  c1 = getc(stdin);
  c2 = getc(stdin);
  while(c1!=EOF && c2!=EOF)
  {
    cout << 60*c1+c2 << " ";
    c1 = getc(stdin);
    c2 = getc(stdin);
  }
  return 0;
}
