#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/*
Made by: Logan Puntous

Features:
Takes integers and expressions and returns a float.
Is able to take expressions such as "5/5", "2*3-4", "2^4",
using addition, subtraction, multiplication, division, and exponent.

Non-features:
Is unable to do parenthesis or take floating points as input.
Cannot do sin cos tan or inverse.
Cannot do "4-2x=4", "x=3+2".

 */


const int MAX_EXPRESSION_SIZE = 25;
char expression[MAX_EXPRESSION_SIZE];



// Reads char's from user into expression[]
void read_chars(char expression[]){
  char c;
  int p = 0;

  printf("Enter an expression: ");
  
  while ( scanf("%c", &c) ){
    if ( c == '\n' ) { return;}
    //printf("here at %c\n", c);
    expression[p] = c;
    p+=1;
  }
}


// Prints the current values in expression
void print_expression(char expression[]){
  printf("EXPRESSION: ");
  for ( int i = 0; i < MAX_EXPRESSION_SIZE; i++ ){
    if ( !expression[i] ) { break; } // END LINE CHECK
    printf("%c", expression[i]); 
  }
  printf("\n");
}

/*
int chomp_number(char expression[], int location){
  
  return 0;
}
*/

void clear_expression(char expression[]){
  for(size_t i = 0; i < MAX_EXPRESSION_SIZE; ++i){
    expression[i] = '\n';
  }
}


// Calculates the number from the expression                                                                                                          
double calculate(int a, int b, char x){
  printf("Current x is %c\n", x);
  switch ( x ){
  case '+':
    return (double)(a+b);
  case '-':
    return (double)(a-b);
  case '*':
    return (double)(a*b);
  case '/':
    if (b != 0) {
      return (double)(a/b);
    } else {
      return -1;
    }
  case '^':{
    double c = a;
    for ( int i = 1; i < b; i++ ){
      c *= a;
    }
    return c;
  }
  default:
    printf("Error\n");
    exit(1);
  }
  return -1;
}


// Parses expression for numbers and modifiers
void parse_expression(char expression[]){

  char temp_expression[MAX_EXPRESSION_SIZE];
  int first_number = 0;
  int second_number = 0;
  char curr_expression = '\n';



  for ( int i = 0; i < MAX_EXPRESSION_SIZE; i++ ){
    // If end then break
    if ( !expression[i] ){ break;}
    
    // First number reading
    if ( isdigit(expression[i]) && first_number == 0){
      int get_rid_of_me = 0;
      while ( isdigit(expression[i]) ){
	temp_expression[get_rid_of_me] = expression[i];
	get_rid_of_me++;
	i++;
      }
      
      first_number = atoi(temp_expression);
      printf("WOW: %d\n", first_number);
      clear_expression(temp_expression); // CLEAR TEMP CHAR[]
      continue;
    }

    // Expression
    switch ( expression[i] ){
    case '^':
      curr_expression = '^';
    case '*':
      curr_expression = '*';
    case '/':
      curr_expression = '/';
    case '+':
      curr_expression = '+';
    case '-':
      curr_expression =	'-';
    }
   
    // Second number reading
    if ( isdigit(expression[i]) && second_number == 0){
      int get_rid_of_me2 = 0;
      while ( isdigit(expression[i]) ){
        temp_expression[get_rid_of_me2] = expression[i];
        get_rid_of_me2++;
        i++;
      }

      second_number = atoi(temp_expression);
      printf("WOW2: %d\n", second_number);
      float tempballs = calculate(first_number, second_number, curr_expression);
      printf("LOOK: %g\n", tempballs);
      first_number = 0;
      second_number = 0;
      //clear_expression(curr_expression);
      continue;
    }
	
    printf("%c is not supported.\n", expression[i]);
     
  }
}



int main(){
  printf("Welcome to the best calculator ever made!\n");

  
  read_chars(expression);
  
  //print_expression(expression);

  parse_expression(expression);


  //printf("%g\n", calculate(2,5,'^'));
  return 0;
}
