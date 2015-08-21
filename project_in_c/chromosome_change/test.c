#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "evaluate.h"


int main()
{


	printf("\n\n*******%d *******\n\n",evaluate_cost("Q=6;X=((Q-(Q-(Q/50)*50))/50) *150 + ((Q-(Q/50)*50) * 20) "));


	return 0;
}
