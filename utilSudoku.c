#include <stdlib.h>
#include <stdio.h>
#include <string.h>
         
     
int lireSudoku(FILE *fich,int sudoku[9][9])
{int i,j,res;
 int val; 
 for (i=0; i<9; i++) 
   for (j=0; j<9; j++) 
     {
       res=fscanf(fich, "%d",&val);
       if (res==EOF)
         {
           fprintf(stderr,"Fin de ficher atteinte: manque des coefficients\n");
           exit(-1);
         }
       sudoku[i][j]=val;
     }
 return(0);
}

int ecrireSudoku(FILE *fich,int sudoku[9][9])
{int i,j;
 for (i=0; i<9; i++) 
   {
     for (j=0; j<9; j++) 
       {
         fprintf(fich, "%d ",sudoku[i][j]);
       }
     fprintf(fich, "\n");
   }
 return(0);
}

