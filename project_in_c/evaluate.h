#include<stdio.h>
#include "eval.h"

int evaluate_cost(char *expressionString) {
	
  int i;
  int errorFlag;
  

  /* evaluate expressions */
  errorFlag = evaluateExpression(expressionString);     // Call parser function
  

  /* print values of variables */
  for (i = 0; i < nVariables; i++);
  /* check for error */
  if (errorFlag) fprintf(stderr, "%s at line %d, column %d\n",
                            errorRecord.message,
                            errorRecord.line,
                            errorRecord.column);
  /* done */
  return variable[i-1].value;
}
