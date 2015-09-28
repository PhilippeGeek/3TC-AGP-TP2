/***********************************************************************
 School works
 Copyright (C) 2015-2016 Philippe VIENNE - Gabriel AUGENDRE

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utilSudoku.h"
#include "main.h"
#include "sudoku.h"

int  main(int argc,char *argv[])
{
  FILE *fich;
  char *nomFich ;
  int sudoku[9][9];
  
  if (argc!=2)
    {
      fprintf(stdout," usage: %s nomFich.txt \n",argv[0]);
      exit(-1);
    }
  nomFich=(char *)malloc(100*sizeof(char));
  strcpy(nomFich,argv[1]);
  fich=fopen(nomFich,"r");
  if (!fich)
    fprintf(stderr,"erreur d'ouverture du fichier\n");
  
 lireSudoku(fich,sudoku); 
 fprintf(stdout," sudoku lu: \n");
 ecrireSudoku(stdout,sudoku);

    if (sudokuValide(sudoku)) {
        printf("Et en plus il est valide !\n");
    }

    solveSudoku(sudoku);
    printf("Résolution: \n");
    ecrireSudoku(stdout, sudoku);
    if (sudokuValide(sudoku)) {
        printf("La solution est valide !\n");
    }

   return(0);
}
