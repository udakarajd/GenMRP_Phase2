
/** structfor BOM  **/
typedef struct CHILDID{
	
	char childid[10];
	int quantity;
	struct CHILDID * next;
	
}childpart;

/*part details, index numbers of the parts_array will 
be used for calculations since supplier id type is 'string'*/

typedef struct {
	
	char PartId[10];
	int MaxCapacity;
	childpart * children;
	
}part;

/* A member of our population */
typedef struct
{
  int * sol; 			// This member's solution (chromosome)
  float fitness;    // How good the solution is,(it's the inverse of the total cost of the solution)
  
} ga_memb;


/*supplier details, index numbers of the supplier_array will 
be used for calculations since supplier id type is 'string'*/

typedef struct {
	
	char * SupId;
	
} supplier;




/***  discount struct ***/

typedef struct EXTRAPARTDISCOUNT {
	
	int quantity;
	char extrapart[10];
	int discountrate;
	struct EXTRAPARTDISCOUNT * next;
	
	
} exprtdiscount;


typedef struct SUPPLIERDISCOUNT {
	
	char SupId[10];
	exprtdiscount * ExPrtDiscount;
	
} suppdiscount;


typedef struct {
	char partId[10];
	suppdiscount * supdisc; 
	
} discount;
