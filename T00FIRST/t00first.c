/* AS4, 02.04.14 */

#include <stdio.h>
#include <windows.h>


int main (int argc, char *argv[])
{
  float x = 1.5;
  char buf[101];
  printf("Hello Graphics\n");

  sprintf(bufs, "x = %f\n", x);

  MessageBox(NULL, buf, "Title test", MB_ICONHAND, MB_OKCANCEL);
  return 0;
}
