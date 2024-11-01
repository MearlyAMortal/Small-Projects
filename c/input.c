/*
Written by Logan Puntous
input.c

This program takes a math expression from the user
and returns the solution to the expression using c.
 */

#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv){
  if ( argc == 1 ) {
    printf("Error input\n");
    exit(1);
  }

  // Returns input from user.
  for ( int i = 1; i < argc; i++ ){
    printf("Input %i is %s\n", i, argv[i]);
  }
  printf("argc = %d\n", argc);

  // Allocate space for expression
  char *expression = (char *)malloc(argc * sizeof(char));
  for ( int j = 0; j < argc-1; j++ ){
    //expression[j] = argv[j+1];
    expression[j] = 'k';
  }
  expression[argc-1] = '\n';
  
  for ( int f = 0; f < argc-1; f++ ){
    printf("item %d is %c\n", f, expression[f]);
  }
  return 0;
}
