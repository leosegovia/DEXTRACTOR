/*******************************************************************************************
 *
 *  Uncompressor for .dexqv files
 *
 *  Author:  Gene Myers
 *  Date:    Jan 18, 2014
 *
 ********************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#include "QV.h"

static char *Usage = "[-vk] <path:dexqv> ...";

static void flip_short(void *w)
{ uint8 *v = (uint8 *) w;
  uint8  x;

  x    = v[0];
  v[0] = v[1];
  v[1] = x;
}

int main(int argc, char* argv[])
{ int VERBOSE;
  int KEEP;

  { int i, j, k;
    int flags[128];

    ARG_INIT("undexqv")

    j = 1;
    for (i = 1; i < argc; i++)
      if (argv[i][0] == '-')
        { ARG_FLAGS("vk") }
      else
        argv[j++] = argv[i];
    argc = j;

    VERBOSE = flags['v'];
    KEEP    = flags['k'];

    if (argc == 1)
      { fprintf(stderr,"Usage: %s %s\n",Prog_Name,Usage);
        exit (1);
      }
  }

  //  For each .dexqv file to be decompressed

  { int     i;

    for (i = 1; i < argc; i++)
      { char     *pwd, *root;
        FILE     *input, *output;
        QVcoding *coding;

        //   Open it and the appropriately named .quiva file

        pwd   = PathTo(argv[i]);
        root  = Root(argv[i],".dexqv");
        input = Fopen(Catenate(pwd,"/",root,".dexqv"),"r");
        if (input == NULL)
          exit (1);
        output = Fopen(Catenate(pwd,"/",root,".quiva"),"w");
        if (output == NULL)
          exit (1);

        if (VERBOSE)
          { fprintf(stderr,"Processing '%s' ... ",root);
            fflush(stderr);
          }

        // Read in compression scheme

        coding = Read_QVcoding(input);

        //  For each compressed entry do

        { int well;

          well = 0;
          while (1)
            { int    beg, end, qv;
              uint16 half;
              uint8  byte;

              //  Decode the compressed header and write it out

              if (fread(&byte,1,1,input) < 1) break;
              while (byte == 255)
                { well += 255;
                  fread(&byte,1,1,input);
                }
              well += byte;

              if (coding->flip)
                { fread(&half,sizeof(uint16),1,input);
                  flip_short(&half);
                  beg = half;
                  fread(&half,sizeof(uint16),1,input);
                  flip_short(&half);
                  end = half;
                  fread(&half,sizeof(uint16),1,input);
                  flip_short(&half);
                  qv = half;
                }
              else
                { fread(&half,sizeof(uint16),1,input);
                  beg = half;
                  fread(&half,sizeof(uint16),1,input);
                  end = half;
                  fread(&half,sizeof(uint16),1,input);
                  qv = half;
                }

              fprintf(output,"%s/%d/%d_%d RQ=0.%d\n",coding->prefix,well,beg,end,qv);

              //  Decode the QV entry and write it out

              Uncompress_Next_QVentry(input,output,coding,end-beg);
            }
	}

        //  Clean up for the next file

	Free_QVcoding(coding);

        fclose(input);
        fclose(output);

        if (!KEEP)
          unlink(Catenate(pwd,"/",root,".dexqv"));
        free(root);
        free(pwd);

        if (VERBOSE)
          { fprintf(stderr," Done\n");
            fflush(stderr);
          }
      }
  }

  free(QVentry());

  exit (0);
}