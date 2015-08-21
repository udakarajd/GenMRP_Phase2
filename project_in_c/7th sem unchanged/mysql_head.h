#include <mysql/my_global.h>
#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "structures.h"


int no_of_supplier(MYSQL *con, char* comp);
int no_of_part(MYSQL *con, char* comp);



/** making query string  **/
char* concat(char *s1, char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the zero-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    strcat(result, "'");
    
    return result;
}

char* concat1(char *s1, char *s2,char *s3,char *s4)
{
    char *result = malloc(strlen(s1)+strlen(s2)+strlen(s3)+strlen(s4)+1);//+1 for the zero-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    strcat(result, s3);
    strcat(result, s4);
    strcat(result, "'");
   
    return result;
}


void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);        
}

char ** partid(MYSQL *con, char* comp){
	
	
	if (mysql_query(con, concat("SELECT partid FROM part where compid = '", comp))) 
  {
      finish_with_error(con);
  }
  
  MYSQL_RES *result = mysql_store_result(con);
  
  if (result == NULL) 
  {
      finish_with_error(con);
  }

  int a = 0;
  int num_fields = mysql_num_fields(result);
  char** prtid = malloc(sizeof(char*)*25);

  MYSQL_ROW row;
 
  while ((row = mysql_fetch_row(result))) 
  { 
      for(int i = 0; i < num_fields; i++) 
      { 
        // printf("%s \n", row[i] ? row[i] : "NULL");
         char* new = malloc(100);
         prtid[a]= new;
         strcpy(prtid[a],row[i]);
         
      } 
      a++;
         // printf("\n"); 
  }
  
  prtid[a] = NULL;
  mysql_free_result(result);
  
  
	return prtid;
	
	
	}
	


char ** supplierid(MYSQL *con, char* comp){
	
	
	if (mysql_query(con, concat("SELECT supplierid FROM supplier where compid = '", comp))) 
  {
      finish_with_error(con);
  }
  
  MYSQL_RES *result = mysql_store_result(con);
  
  if (result == NULL) 
  {
      finish_with_error(con);
  }

  int a = 0;
  int num_fields = mysql_num_fields(result);
  char** suppid = malloc(sizeof(char*)*no_of_supplier(con,comp));

  MYSQL_ROW row;
 
  while ((row = mysql_fetch_row(result))) 
  { 
      for(int i = 0; i < num_fields; i++) 
      { 
        // printf("%s \n", row[i] ? row[i] : "NULL");
         char* new = malloc(100);
         suppid[a]= new;
         strcpy(suppid[a],row[i]);
         
      } 
      a++;
         // printf("\n"); 
  }
  
  suppid[a] = NULL;
  mysql_free_result(result);
  
  
	return suppid;
	
	
	}


supplier * insert_to_suppary(MYSQL *con, char* comp){
	
	
	int no_of_supp;
	
	no_of_supp = no_of_supplier(con,comp);
	
	supplier * supplier_array= malloc(sizeof(supplier)*no_of_supp);
	
	char** suppid = supplierid(con,comp);
	
	 for(int i=0;i<no_of_supp;i++ ){
		 supplier_array[i].SupId=malloc(10);
		strcpy(supplier_array[i].SupId,suppid[i]);
		//parts_array[i].children= NULL;
		//printf("%s\n",supplier_array[i].SupId);
		}
	
	return supplier_array;	
	}

part * bom_detail(MYSQL *con, char* comp){
	
	int no_of_prts;
	
	no_of_prts = no_of_part(con,comp);
	
	part * parts_array= malloc(sizeof(part)*no_of_prts);
	
	char** prtid = partid(con,comp);
	
	 for(int i=0;prtid[i] != NULL;i++ ){
		strcpy(parts_array[i].PartId,prtid[i]);
		parts_array[i].children= NULL;
		
	//printf("%s\n",parts_array[i].PartId);
		}
	

	if (mysql_query(con, concat("SELECT partid, childpartid, quantity FROM bom where compid = '", comp))) 
  {
      finish_with_error(con);
  }
  
  MYSQL_RES *result = mysql_store_result(con);
  
  if (result == NULL) 
  {
      finish_with_error(con);
  }

  int num_fields = mysql_num_fields(result);
  int a=0;

	
	
  MYSQL_ROW row;
 
  while ((row = mysql_fetch_row(result))) 
  { 
	  int i;
       for(i=0 ; i<no_of_prts;i++){
		   if(strcmp(parts_array[i].PartId,row[0]) ==0)
		   break;
		   }
		  
			  if(parts_array[i].children == NULL){
				  
				  childpart* child = malloc(sizeof(childpart));
				  strcpy(child->childid,row[1]);
				  child->quantity = atoi(row[2]);
				  parts_array[i].children= child;
				  parts_array[i].children->next = NULL;
				  
				  }
			  else{
				  while(parts_array[i].children->next != NULL){
					  
					  parts_array[i].children=parts_array[i].children->next;
					  }
				  
				  childpart* child = malloc(sizeof(childpart));
				  strcpy(child->childid,row[1]);
				  child->quantity = atoi(row[2]);
				  parts_array[i].children->next= child;
				  parts_array[i].children->next->next = NULL;
				  
				  }
			  
			  
         
	 
  }
  
  mysql_free_result(result);
  
  
  for(int i=0; i<no_of_prts;i++){
	  
	  while(parts_array[i].children != NULL ){
	   
	  parts_array[i].children = parts_array[i].children->next;
			
			}
	  
	  
	  }
  
  
  
 // printf("part id = %s\n child part = %s\n quantity = %d\n",b->partid,b->childpart,b->quantity);

	
return parts_array;

}

/** this function return the maximun day **/

int max_day(MYSQL *con,char* comp)
{
  
  
  if (mysql_query(con, concat("SELECT MAX(days) FROM orders where compid = '", comp))) 
  {
      finish_with_error(con);
  }
  
  MYSQL_RES *result = mysql_store_result(con);
  
  if (result == NULL) 
  {
      finish_with_error(con);
  }

  int num_fields = mysql_num_fields(result);

  MYSQL_ROW row;
  int days;
  while ((row = mysql_fetch_row(result))) 
  { 
      for(int i = 0; i < num_fields; i++) 
      { 
         // printf("%s ", row[i] ? row[i] : "NULL");
          days = atoi(row[i]); 
      } 
         // printf("\n"); 
  }
  
  mysql_free_result(result);
  
  
  return days;

}


/** this function return the maximun quantity **/

int max_quantity(MYSQL *con, char* comp)
{
  
 // concat("SELECT MAX(quantity) FROM orders where compid = ", comp)
  //printf("%s\n",concat("SELECT MAX(quantity) FROM orders where compid = '", comp));
  if (mysql_query(con,concat("SELECT MAX(quantity) FROM orders where compid = '", comp))) 
  {
      finish_with_error(con);
  }
  
  MYSQL_RES *result = mysql_store_result(con);
  
  if (result == NULL) 
  {
      finish_with_error(con);
  }

  int num_fields = mysql_num_fields(result);

  MYSQL_ROW row;
  int quantity;
  while ((row = mysql_fetch_row(result))) 
  { 
      for(int i = 0; i < num_fields; i++) 
      { 
         // printf("%s ", row[i] ? row[i] : "NULL");
          quantity = atoi(row[i]); 
      } 
         // printf("\n"); 
  }
  
  mysql_free_result(result);
 
  
  return quantity;

}

/** this function return the number of parts **/

int no_of_part(MYSQL *con, char* comp)
{
  
 // concat("SELECT MAX(quantity) FROM orders where compid = ", comp)
  //printf("%s\n",concat("SELECT COUNT(pratid) FROM orders where compid = '", comp));
  if (mysql_query(con,concat("SELECT COUNT(*) FROM orders where compid = '", comp))) 
  {
      finish_with_error(con);
  }
  
  MYSQL_RES *result = mysql_store_result(con);
  
  if (result == NULL) 
  {
      finish_with_error(con);
  }

  int num_fields = mysql_num_fields(result);

  MYSQL_ROW row;
  int no_of_prt;
  while ((row = mysql_fetch_row(result))) 
  { 
      for(int i = 0; i < num_fields; i++) 
      { 
         
          no_of_prt = atoi(row[i]); 
      } 
         
  }
  
  mysql_free_result(result);
 
  
  return no_of_prt;

}


/** this function return the number of supplier **/

int no_of_supplier(MYSQL *con, char* comp)
{
  
 // concat("SELECT MAX(quantity) FROM orders where compid = ", comp)
  //printf("%s\n",concat("SELECT COUNT(pratid) FROM orders where compid = '", comp));
  if (mysql_query(con,concat("SELECT COUNT(*) FROM supplier where compid = '", comp))) 
  {
      finish_with_error(con);
  }
  
  MYSQL_RES *result = mysql_store_result(con);
  
  if (result == NULL) 
  {
      finish_with_error(con);
  }

  int num_fields = mysql_num_fields(result);

  MYSQL_ROW row;
  int no_of_suppliers;
  while ((row = mysql_fetch_row(result))) 
  { 
      for(int i = 0; i < num_fields; i++) 
      { 
         
          no_of_suppliers = atoi(row[i]); 
      } 
         
  }
  
  mysql_free_result(result);
 
  
  return no_of_suppliers;

}







/** this function return the all quantity **/

int* all_quantity(MYSQL *con, char* comp)
{
   MYSQL_ROW row;
 // concat("SELECT MAX(quantity) FROM orders where compid = ", comp)
  //printf("%s\n",concat("SELECT MAX(quantity) FROM orders where compid = '", comp));
  if (mysql_query(con,concat("SELECT quantity FROM orders where compid = '", comp))) 
  {
      finish_with_error(con);
  }
  
  MYSQL_RES *result = mysql_store_result(con);
  
  if (result == NULL) 
  {
      finish_with_error(con);
  }

  int num_fields = mysql_num_fields(result);

 int a = 0;
  int* quantity= (int*)malloc(num_fields*sizeof(int));
  while ((row = mysql_fetch_row(result))) 
  { 
	  
      for(int i = 0; i < num_fields; i++) 
      { 
		  quantity[a] = atoi(row[i] ? row[i] : "NULL"); 
         
          
      } 
        
     a++;   
  }
  
   
  
  mysql_free_result(result);
 
  
  return quantity;

}


/** this function return the pool size  **/

int pool_size(MYSQL *con,char* comp)
{
  
  
  if (mysql_query(con, concat("SELECT poolsize FROM genengine where compid = '", comp))) 
  {
      finish_with_error(con);
  }
  
  MYSQL_RES *result = mysql_store_result(con);
  
  if (result == NULL) 
  {
      finish_with_error(con);
  }

  int num_fields = mysql_num_fields(result);

  MYSQL_ROW row;
  int size;
  while ((row = mysql_fetch_row(result))) 
  { 
      for(int i = 0; i < num_fields; i++) 
      { 
         // printf("%s ", row[i] ? row[i] : "NULL");
          size = atoi(row[i]); 
      } 
         // printf("\n"); 
  }
  
  mysql_free_result(result);
  
  
  return size;

}


/** this function return the no. of iterations  **/

int iterations(MYSQL *con,char* comp)
{
  
  
  if (mysql_query(con, concat("SELECT iterations FROM genengine where compid = '", comp))) 
  {
      finish_with_error(con);
  }
  
  MYSQL_RES *result = mysql_store_result(con);
  
  if (result == NULL) 
  {
      finish_with_error(con);
  }

  int num_fields = mysql_num_fields(result);

  MYSQL_ROW row;
  int iter;
  while ((row = mysql_fetch_row(result))) 
  { 
      for(int i = 0; i < num_fields; i++) 
      { 
         // printf("%s ", row[i] ? row[i] : "NULL");
          iter = atoi(row[i]); 
      } 
         // printf("\n"); 
  }
  
  mysql_free_result(result);
  
  
  return iter;

}


/** this function return the maximum order solution can have  **/

int maxorder(MYSQL *con,char* comp)
{
  
  
  if (mysql_query(con, concat("SELECT maxorder FROM genengine where compid = '", comp))) 
  {
      finish_with_error(con);
  }
  
  MYSQL_RES *result = mysql_store_result(con);
  
  if (result == NULL) 
  {
      finish_with_error(con);
  }

  int num_fields = mysql_num_fields(result);

  MYSQL_ROW row;
  int maxorder;
  while ((row = mysql_fetch_row(result))) 
  { 
      for(int i = 0; i < num_fields; i++) 
      { 
         // printf("%s ", row[i] ? row[i] : "NULL");
          maxorder = atoi(row[i]); 
      } 
         // printf("\n"); 
  }
  
  mysql_free_result(result);
  
  
  return maxorder;

}


/** this function return the maximum order solution can have  **/

double mutate_rate(MYSQL *con,char* comp)
{
  
  
  if (mysql_query(con, concat("SELECT mutaterate FROM genengine where compid = '", comp))) 
  {
      finish_with_error(con);
  }
  
  MYSQL_RES *result = mysql_store_result(con);
  
  if (result == NULL) 
  {
      finish_with_error(con);
  }

  int num_fields = mysql_num_fields(result);

  MYSQL_ROW row;
  double mutaterate;
  while ((row = mysql_fetch_row(result))) 
  { 
      for(int i = 0; i < num_fields; i++) 
      { 
         // printf("%s ", row[i] ? row[i] : "NULL");
          mutaterate = atof(row[i]); 
      } 
         // printf("\n"); 
  }
  
  mysql_free_result(result);
  
  
  return mutaterate ;

}



/** this function return the maximum order solution can have  **/

char* cost_fun(MYSQL *con,char* comp,char* partid,char* suppid)
{
  
  
 /* if (mysql_query(con, concat1("SELECT mutaterate FROM costfunction where compid = '", comp ,"and partid ='", partid))) 
  {
      finish_with_error(con);
  }*/
  char* query= malloc(100);
  
  strcpy(query,concat1("SELECT functxt FROM costfunction where compid = '",comp,"' and partid ='",partid));
  strcat(query,"and supplierid ='");
  strcat(query,suppid);
  strcat(query,"'");
  
  //printf("%s\n",query);
  
  if (mysql_query(con, query)) 
  {
      finish_with_error(con);
  }
  
  MYSQL_RES *result = mysql_store_result(con);
  
  if (result == NULL) 
  {
      finish_with_error(con);
  }

  int num_fields = mysql_num_fields(result);

  MYSQL_ROW row;
  int a =0;
  char** costfunction= malloc(sizeof(char*)*10);
  while ((row = mysql_fetch_row(result))) 
  { 
      for(int i = 0; i < num_fields; i++) 
      { 
         
         char* new = malloc(100);
         costfunction[a]= new;
          strcpy(costfunction[a],row[i]); 
      } 
      a++;  
  }
  costfunction[a] = NULL;
  mysql_free_result(result);
  
  char* resultfun = malloc(250);
  resultfun[0]='\0';
 
if(!costfunction[0]){
	printf("No costfunction");
	
	exit (-1);
	}
	
	 strcat(resultfun,"x=");
	 strcat(resultfun,costfunction[0]);
	 
	 
	 for(int i=1;costfunction[i] != NULL;i++ ){
	 
	 strcat(resultfun,"+");
	 strcat(resultfun,costfunction[i]);
	 
 }
 strcat(resultfun,"");
 
  
  
  return resultfun;

}

void strreplace(char s[], char chr, char repl_chr)
{
     int i=0;
     while(s[i]!='\0')
     {
           if(s[i]==chr)
           {
               (s[i]=repl_chr);
           }  
           i++; 
     }
        //  printf("%s",s);
}



int* details_from_order(MYSQL *con, char* comp,char* partid)
{
   MYSQL_ROW row;
 // concat("SELECT MAX(quantity) FROM orders where compid = ", comp)
  //printf("%s\n",concat("SELECT MAX(quantity) FROM orders where compid = '", comp));
  if (mysql_query(con,concat1("SELECT days,quantity FROM orders where compid = '", comp,"'and partid ='",partid))) 
  {
      finish_with_error(con);
  }
  
  MYSQL_RES *result = mysql_store_result(con);
  
  if (result == NULL) 
  {
      finish_with_error(con);
  }

  int num_fields = mysql_num_fields(result);

  int* detail= (int*)malloc(num_fields*sizeof(int));
  while ((row = mysql_fetch_row(result))) 
  { 
	  
      for(int i = 0; i < num_fields; i++) 
      { 
		  //printf("%s\n ", row[i] ? row[i] : "NULL");
		  detail[i] = atoi(row[i] ? row[i] : "NULL"); 
         
          
      } 
          
  }
  
   
  
  mysql_free_result(result);
 
  
  return detail;

}


int* details_from_part(MYSQL *con, char* comp,char* partid)
{
   MYSQL_ROW row;
 // concat("SELECT MAX(quantity) FROM orders where compid = ", comp)
  //printf("%s\n",concat("SELECT MAX(quantity) FROM orders where compid = '", comp));
  if (mysql_query(con,concat1("SELECT leadtime,onhand FROM part where compid = '", comp,"'and partid ='",partid))) 
  {
      finish_with_error(con);
  }
  
  MYSQL_RES *result = mysql_store_result(con);
  
  if (result == NULL) 
  {
      finish_with_error(con);
  }

  int num_fields = mysql_num_fields(result);


  int* detail= (int*)malloc(num_fields*sizeof(int));
  while ((row = mysql_fetch_row(result))) 
  { 
	  
      for(int i = 0; i < num_fields; i++) 
      { 
		  //printf("%s\n ", row[i] ? row[i] : "NULL");
		  detail[i] = atoi(row[i] ? row[i] : "NULL"); 
         
          
      } 
           
  }
  
   
  
  mysql_free_result(result);
 
  
  return detail;

}






