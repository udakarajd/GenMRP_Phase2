#include "mysql_head.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "evaluate.h"


#define GA_POPSIZE 500// Population size
#define GA_MAXITER 200 // Maximum iterations
#define GA_ELITERATE 0.1 // Elitism rate
#define GA_MUTPERC 0.1 // Mutation rate
#define PRINT_INTER  0// Print status every this iterations/generations

#define GA_ELITSIZE (GA_POPSIZE * GA_ELITERATE) // Number of elites
#define GA_MUTCHANCE 10
#define INIT (GA_POPSIZE*0.01)//number of gene that has same solution as the POR

/*#define TARGET_LEN 20   /* The length of the solution*/
#define CHROMOSOME_MAX 200 //Max order

/* chromosome 
 * 
 * [partid,supid,quantity,quantity,..,quantity,partid,supid,quantity,quantity,..,quantity,.....partid,supid,quantity,quantity,..,quantity]
 * 
 * */
 
 


int sol_length;
int no_of_parts;
int no_of_periods;
int no_of_suppliers;
int * sol_store;
part * parts_array;
supplier * supplier_array;
discount * discount_array;
ga_memb best_sol;
MYSQL *con;
int* nonzero;
int** por;

/*
int bom[5][13]={
	{0, 0, 0, 0, 0, 0, 0, 15, 12, 0, 0, 0, 0},
	{1, 0, 0, 0, 0 ,0 ,0 ,0 ,0 ,27, 0, 0, 0},
	{2, 0, 0, 0, 0, 35, 36, 0, 0, 0, 0, 0, 0},
	{3, 0, 0, 0, 0, 15, 0, 0, 135, 0, 0, 0, 0},
	{4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, 0, 0 }
	}; */


void mysolution(){ //calculating the solution length & other details
	
	int i,j,count=0,count2=0,count3=0,wholecount=0;
	
	sol_store = malloc (sizeof(int)*no_of_parts); // this array has no of oder quantities for each part (cummelative)
	
	for(i=0;i<no_of_parts;i++){
		//count3=0;
		count2=0;
		for(j=0;j<no_of_periods+2;j++){
			
			if(por[i][j]>0){
				count++;
				count2++;	
			}
			
		}
		//if(i!=0)
			//count2--; // if part id is not zero, it should deducted from the count
		//wholecount=wholecount+count2+1; // counting non zero elements in whole array. part IDs are ignored
		count3=count3+count2+1;
		wholecount = count3/2;
		printf("count = %d\t count2 =%d\t count3= %d\n",count,count2,wholecount);
		sol_store[i]=count3/2; // no of order quantities for a part
		
	}
	//count++;
	sol_length=wholecount*2; //every quantity has a seller. hence => no of quantities *2
	//setting up the length of the solution 

	 nonzero = malloc(sizeof(int)*wholecount);
	count=0;
	
	for(i=0;i<no_of_parts;i++){
		for(j=1;j<no_of_periods+2;j++){
			
			if(por[i][j]>0){
			
			nonzero[count] = por[i][j]; // copying data from the por 
			count++;
			}
		}
	}
}

void init_pop(ga_memb *pop); /* Calls: */

void zero_fitness(ga_memb *pop); /* Set fitnesses to 0 */

void randall_sols(ga_memb *pop); /* Make all the solutions random */

void calc_fitness(ga_memb *pop); /* Calculate a member's fitness based on its solution */

void sort_by_fitness(ga_memb *pop); /* Sort the whole population by fitness */

/* Print the best member's solution, gen is the number of the current
 * generation which we also print */
 
void print_best(ga_memb *pop, unsigned const int gen);

void mate_pop(ga_memb *pop, ga_memb *buf); // Mates pop into buf
void cp_mems(ga_memb *src, ga_memb *targ, unsigned int size);
void mutate(ga_memb *pop); // Mutates some of the population

/* Swaps the things that pt1 and pt2 point to */
void swap_pts(ga_memb **pt1, ga_memb **pt2);

/* We use a simple insertion sort */
#define sort_by_fitness(a) (insertion_sort_ga_memb(a, GA_POPSIZE))

void insertion_sort_ga_memb(ga_memb *a, int asize);

int string_create(int quantity,char*expres);



void init_pop(ga_memb *pop)
{
  zero_fitness(pop);
  randall_sols(pop);
}

void zero_fitness(ga_memb *pop)
{
  int i;
  for (i = 0; i < GA_POPSIZE; i++)
  {
    pop[i].fitness = 0;
   
  }
}

void randall_sols(ga_memb *pop){
  int i, j,k,n,c;
  for (i = 0; i < GA_POPSIZE-INIT; i++){
	  k=0;
	  
    for (j = 1; j < sol_length; j+=2){
	  
		 n=rand();
		pop[i].sol[j] = n % CHROMOSOME_MAX;
		pop[i].sol[j-1]= n% no_of_suppliers;
    
    
		}
	}
	
	for(i = GA_POPSIZE-INIT; i < GA_POPSIZE; i++){
		
		c=0;
		
			for(k=1;k<sol_length;k+=2){
				
				pop[i].sol[k]=nonzero[c++];
				pop[i].sol[k-1]=rand()%no_of_suppliers;	
			
		}

	}

	
}

int discount_cal(int partindex,int supindex, int quantity){
	int discount_z=0;
	exprtdiscount * node;
	node=discount_array[partindex].supdisc[supindex].ExPrtDiscount;
	while(node){
		
		if(quantity >= node->quantity){
				//printf("%d %d %d \n",quantity,node->quantity, node->discountrate);
			discount_z=node->discountrate;
			break;
		}
		
		node=node->next;
		
	}
	
	
	return discount_z;
}

void calc_fitness(ga_memb *pop){
	
	float fitness;
	int i, j,k,quantity,quantity2,bad_sol,n,temp,t,supindex;
	float cost=0;
	int discount_z=0;
	
	
		for(i = 0; i < GA_POPSIZE; i++){
		
		
		
			for(k=0;k<sol_length;k++){
				
				//printf("%3d ,",pop[i].sol[k]);
			
		}
		
		//printf("\n");

	}
	
	for(i=0;i<no_of_parts;i++){
		
		//printf("!!! %d !!!\n",sol_store[i]);
		
	}
	
	

  for (i = 0; i < GA_POPSIZE; i++)
  {
	  fitness = 0.0;
	  
    cost=0;
   
    for (j=0; j < no_of_parts; j++)
    {
		//don't touch above
		quantity=0;
		
		bad_sol=0;
		
		
		////////// case
		
		if(j==0){
			n=0;
			for(k=1;k<sol_store[j]*2;k+=2){
				quantity=pop[i].sol[k];
				supindex=pop[i].sol[k-1];
				
				//printf("**** %d ****# %d\n",supindex,k);
				discount_z=discount_cal(j,pop[i].sol[k-1],quantity);
				cost=cost+(float)string_create(quantity,cost_fun(con,"IFS",parts_array[j].PartId,supplier_array[supindex].SupId,discount_z));
				//printf("%f\n",cost);
				
			}
		}

		else{
			n=sol_store[j-1];
			
			for(k=sol_store[j-1]*2+1;k<sol_store[j]*2;k+=2){
				quantity=pop[i].sol[k];
				supindex=pop[i].sol[k-1];
				
				//printf("**** %d **** %d\n",supindex,k);
				
				discount_z=discount_cal(j,pop[i].sol[k-1],quantity);
				cost=cost+(float)string_create(quantity,cost_fun(con,"IFS",parts_array[j].PartId,supplier_array[supindex].SupId,discount_z));
				//printf("%f\n",cost);
			}
		}
		

		temp=pop[i].sol[n];
		//pop[i].sol[n]=temp%no_of_suppliers;
		//printf(" <== %d ===>",t);
		
		//discount_array
		
		
		//discount_z=discount_cal(j,pop[i].sol[n],quantity);
		
		
	//	cost=cost+(float)string_create(quantity,cost_fun(con,"IFS",parts_array[j].PartId,supplier_array[pop[i].sol[n]].SupId,discount_z));
		
		      
      //don't touch below
      
		}
		//printf("%f\n ",cost);
		pop[i].fitness = (float)(1.0/cost);
	}
	
} 

int string_create(int quantity,char*expres){
	
	
	char *q=malloc(10);
	char * str=malloc(300);
	int cost=0;
	
	//itoa(quantity,q,10);
	snprintf(q, 10,"%d",quantity);
	str[0]='\0';
	
	strcat(str,"Q=");
	strcat(str,q);
	strcat(str,";");
	strcat(str,expres);
	strcat(str,"");
	
	
	cost=evaluate_cost(str);
	//printf("\n\n\t%d *****\n\n\n",cost);
	free(str);
	free(q);
	
	return cost;
	
}


#define balh(a) (insertion_sort_ga_memb(a, GA_POPSIZE))

void insertion_sort_ga_memb(ga_memb *a, int asize){
  int i, j, k;
  ga_memb d;

  k = asize;
  for (i = 0; i < k; i++)
  {
    d = a[i];
    j = i - 1;
    while (j >= 0 && a[j].fitness > d.fitness)
    {
      a[j + 1] = a[j];
      j = j - 1;
    }
    a[j + 1] = d;
  }
}

void print_best(ga_memb *pop, unsigned const int gen){
  int i,new=0;

  printf("At gen %d, best : ", gen);
  if(pop[GA_POPSIZE - 1].fitness>best_sol.fitness){
	  new=1;
	  
	 best_sol.fitness= pop[GA_POPSIZE - 1].fitness;
  }
  

	for (i = 0; i < sol_length; i++) {
		if(new==1)
			best_sol.sol[i]=pop[GA_POPSIZE - 1].sol[i];
			
		printf("%d, ",pop[GA_POPSIZE - 1].sol[i]);
	 }
		  
  printf("  %f\n", (pop[GA_POPSIZE - 1].fitness ) );
}

/* Our mating method is a simple cut and swap:
 *
 * Consider two solutions:
 *
 * parent1: |====================|
 * parent2: |XXXXXXXXXXXXXXXXXXXX|
 *
 * Pick an arbitrary point to cut at:
 *
 * parent1: |==============|======|
 * parent2: |XXXXXXXXXXXXXX|XXXXXX|
 *
 * Swap the part of the solution after that point to form the children:
 *
 * child1:  |==============XXXXXXX|
 * child2:  |XXXXXXXXXXXXXX=======|
 *
 * Note that only the children go into the new population; all the parents
 * die.
 */
 
void mate_pop(ga_memb *pop, ga_memb *buf){ // Mates pop into buf
  unsigned int i, j;
  int randint;
  
  // Copy over the elites
  cp_mems(pop, buf, GA_ELITSIZE);
  
  // Don't touch the elites, mate the others
  for (i = GA_ELITSIZE; i < GA_POPSIZE-1; i += 2)
  {
    randint = rand() % sol_length;
    
    // The first half of the chromosomes don't change
    for (j = 0; j < randint; j++)
    {
      buf[i    ].sol[j] = pop[i    ].sol[j];
      buf[i + 1].sol[j] = pop[i + 1].sol[j];
    }
    // The second half of the chromosomes are swapped
    for (j = randint; j < sol_length; j++)
    {
      buf[i    ].sol[j] = pop[i + 1].sol[j];
      buf[i + 1].sol[j] = pop[i    ].sol[j];
    }
  }
  
  mutate(buf); /* Add a bit of random mutation in the solutions,
                  'the spice of life' ;D */
}

// Swap pt1 and pt2
void swap_pts(ga_memb **pt1, ga_memb **pt2){
  ga_memb *pt_tmp;
  
  pt_tmp = *pt2;
  *pt2 = *pt1;
  *pt1 = pt_tmp;
}


// Mutates some of the population
void mutate(ga_memb *pop){
	
  int i, randint,randint1;
  for (i = 0; i < GA_POPSIZE-1; i++)
  {
    if (rand() % GA_MUTCHANCE == 0)
    {
      randint=rand() % sol_length;
      
      if((randint)%2==0){
		randint1 = randint%no_of_suppliers;
		pop[i].sol[randint] = randint1;
      
      } /* should "i" be changed??*/
      
      else{
		  randint1 = randint%CHROMOSOME_MAX;
		pop[i].sol[randint] = randint1;
		  
		  }
    }
  }
  
 
  
}

void cp_mems(ga_memb *src, ga_memb *targ, unsigned int size){
  unsigned int i, j;
  
  for (i = GA_ELITSIZE- 1; i >= size; i--)
  {
    targ[i].fitness = src[i].fitness;
    for (j = 0; j < sol_length; j++)
    {
      targ[i].sol[j] = src[i].sol[j];
    }
  }
}


int check_solution(ga_memb gene,int parts,int suppliers,int periods){
	
	int i;
	
	for(i=0;i<sol_length;i+=periods+1){
		
		if(gene.sol[i]>=parts) return 1;
		
	}
	
	for(i=1;i<sol_length;i+=periods+1){
		
		if(gene.sol[i]>=suppliers) return 1;
		
	}
	
	
	return 0;
}


void validation( ga_memb * pop){
	
	
	int i, j,k,quantity,quantity2,bad_sol,n,temp,t;
	
	
	
	for (i = 0; i < GA_POPSIZE-1; i++)
  {
	
    t=0;
    for (j=0; j < no_of_parts; j++)
    {
		//don't touch above
		quantity=0;
		quantity2=0;
		bad_sol=0;
		
		if(j==0){
			n=0;
			for(k=1;k<sol_store[j]*2;k+=2){
				quantity=quantity+pop[i].sol[k];
				quantity2= quantity2+ nonzero[t];
				if( pop[i].sol[k]< nonzero[t]){
					pop[i].fitness = 0;
					bad_sol = 1;
					break;
					}
				if(k!=1 && quantity< quantity2){
					pop[i].fitness = 0;
					bad_sol = 1;
					break;
					}
				
				t++;
			}
		}
		
		
		else{
			n=sol_store[j-1];
			for(k=sol_store[j-1]*2+1;k<sol_store[j]*2;k+=2){
				quantity=quantity+pop[i].sol[k];
				quantity2= quantity2+ nonzero[t];
				if( pop[i].sol[k]< nonzero[t]){
					pop[i].fitness = 0;
					bad_sol = 1;
					break;
					}
				if(k!=1 && quantity< quantity2){
					pop[i].fitness = 0;
					bad_sol = 1;
					break;
					}
			t++;
			}
		}
		
	if(bad_sol == 1)
	break;
      
		}
  
	}
}

void insert_to_bom(MYSQL *con, char* comp, int indexpart,int indexdate,int quty ){
	
	
	
	por[indexpart][indexdate-1] = quty;
	
	
	}


int index_of_partid(char* partsid,part * parts_array){
	
	int i;
	
	for(i=0;i<5;i++)
	{

			if(strcmp(parts_array[i].PartId,partsid) ==0){
			
			break;
		}
	}
	
	
	return i;
	
	}
	
	
void decode(MYSQL *con, char* comp, char* str, part * parts_array){
	 
	  const char s[10] = ", ) (";
	   char *token;
	   
	   /* get the first token */
	   token = strtok(str, s);
	   
	   /* walk through other tokens */
	   int i,indexpart=0,indexdate=0;
	   
	   for(i=0;i<7;i++){
		   
		if(i == 2){	
		
		indexpart=index_of_partid(token,parts_array);
		insert_to_bom(con,comp,indexpart,1,indexpart );
  
		}
		
		if(i== 5){
			
			indexdate = atoi(token)+2;
			
			}
			
		if(i== 6){
			
			insert_to_bom(con,comp,indexpart,indexdate,atoi(token) );
		
			
			}
			
			token = strtok(NULL, s);
	 }
}



int copy_best(ga_memb * pop){
	
	int i=0;
	
	//printf("\n copying....\n \n");
	   for (i = 0; i < sol_length; i++) {
		
		pop[0].sol[i]=best_sol.sol[i];
		//printf("%d ",pop[0].sol[i]);
		}
		
		//puts("");
	pop[0].fitness=best_sol.fitness; 
  
	
	return 0;
}



int main(void){
  int i, j; /* Counters */
   char str1[25];
    
  /*database connection*/
  
    con = mysql_init(NULL);

  if (con == NULL)
  {
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
  }  

  
  if (mysql_real_connect(con, "localhost", "root", "admin", 
          "GenMRP", 0, NULL, 0) == NULL) 
  {
      finish_with_error(con);
  }  

  /* Create our two populations */
  /* They are really the same population, but we have two for speed */
  ga_memb pop_a[GA_POPSIZE], pop_b[GA_POPSIZE];
  ga_memb *pop = pop_a, *buf = pop_b;
  

  no_of_parts=no_of_part(con,"IFS");
  //printf("no of parts = %d", no_of_parts);
  no_of_periods=max_day(con,"IFS");
  no_of_suppliers=no_of_supplier(con,"IFS"); 
  
  
 /* parts_array*/
  parts_array= bom_detail(con,"IFS");
  supplier_array=insert_to_suppary(con,"IFS");
  printf("Population = %d Genarations %d mutate chance=%d\n",GA_POPSIZE,GA_MAXITER,GA_MUTCHANCE);
   
   /** memory allocation for 2d array **/
   
   por=malloc(sizeof(int *)*no_of_parts);
		for(i=0;i<no_of_parts;i++){
			por[i]=malloc(4*(no_of_periods+2));
			}
			
			
			for(i=0;i< no_of_parts;i++){
			
			for(j=0;j<(no_of_periods+2);j++){
				
				por[i][j] = 0;
				//printf("%d ",por[i][j]);
				}
				//puts("");
			}
    discount_array=discount_price(con,"IFS");
  /** read por.txt**/
  
   FILE * fp;
 
   fp = fopen ("/home/udakarajd/Desktop/project_in_c/chromosome_change/por.txt", "r+");
 
      
   while((fscanf(fp, "%s", str1)) != EOF)
   {
   
   decode(con,"IFS",str1,parts_array);
	  
  }
  
  /** print 2d array **/
  

  fclose(fp);
   
  mysolution();
   best_sol.sol=malloc(sizeof(int)*sol_length);

  /* reserving memory for the solutions*/ 
  for(i=0;i<GA_POPSIZE;i++){
	  
	  pop_a[i].sol=malloc(sizeof(int)*sol_length );
	  pop_b[i].sol=malloc(sizeof(int)*sol_length );
	  
  }

  
  srand((unsigned int) time(NULL)); /* Seed rand() with the time */
 
  init_pop(pop_a);
  init_pop(pop_b);
  
 


  for (i = 0, j = 0; i < GA_MAXITER; i++, j++){
	  
	calc_fitness(pop); // Fill out ga_memb.fitness
	validation(pop);
	if (i!=0)
		copy_best(pop);
	
    sort_by_fitness(pop);
	print_best(pop, i);
   
	
    mate_pop(pop, buf); // Mate the population into the buffer
    swap_pts(&pop, &buf); // Make the buffer our population, and vice versa
  }

  print_best(pop, i);
  
  printf("\n\nBest so far  \n\n" );
    for (i = 0; i < sol_length; i++) {
	 
			printf(" %d, ", best_sol.sol[i]);
		  }
		  
  printf("  (%f)\n", (1/best_sol.fitness ) );

  mysql_close(con);
  
  return 0;
  
}
