#include "systemc.h"
#include "stdio.h"
int sc_main(int, char *[])
{
  int maj=0, min=0, kit=0;
  FILE *fout = fopen( "conftest.out", "w" );
  if(!fout) return 1;
  printf("%s", sc_version());
  sscanf(sc_version()," SystemC %d.%d.%d-",&maj,&min,&kit);
  fprintf(fout, "%d%03d%03d\n",maj,min,kit);
  fclose(fout);
  return 0;
}

int main(int argc, char* argv[]) { sc_main(argc, argv); }
