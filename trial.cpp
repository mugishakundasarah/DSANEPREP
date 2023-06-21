#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
char lasttoken[16];
int cont = 1;

typedef struct
{
    float prevPower;
    float prevAmount;
	int set;
    int month;
}previousData;

typedef struct 
{
    char customerName[50];
    char customerCategory[50];
    int cashPowerNumber;
	previousData customerData;
}Customer;


// declaration of variables
Customer cust[20];

// keep customers in structure
void keepCustomers(){
    Customer *ptr;
    ptr = cust;
    FILE *customers;
    char Buffer[100];

    customers = fopen("customers.txt", "r");
    int i;
    char *token;
    int fieldCount;

    while (fgets(Buffer, 100, customers))
    {
        token = strtok(Buffer, ",");
        fieldCount = 0;
        while (token != NULL)
        {
            if (fieldCount == 0)
            {
                strcpy(ptr->customerName, token);
            }
            else if(fieldCount == 1){
                strcpy(ptr->customerCategory, token);
            }
            else{
                ptr->cashPowerNumber = atoi(token);
            }
            token = strtok(NULL, ",");
            fieldCount++;
        }
        i++;
        ptr++;
    }
}

// check category of the customer
char* checkCategory(int cashPower){
 	for (int j = 0; j < 8; j++)
    {
        if (cust[j].cashPowerNumber == cashPower)
        {
            return cust[j].customerCategory;
        }
    }
    return "no";
}

Customer getCustomer(int cashPower){
      for (int j = 0; j < 8; j++)
      {
          if (cust[j].cashPowerNumber == cashPower)
          {
              return cust[j];
          }
      }
}
// residential
float residential(int amount){
    float power;
    int currAmount;
    if(amount <= 1335){
        power = amount/89;
        return power;
    }
    else if(amount > 1335 && amount<=8755){
        power = 15;
        currAmount = amount - 1335;
        power+=currAmount/212;
        return power;
    }else{
        power = 50;
        currAmount = amount-8775;
        power+=currAmount/249;
        return power;
    }
}

int checkTime(){
     time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return tm.tm_mon+1;
}
previousData updatePrevData(int amount, float pwr, int cshPwr){
    int time = checkTime();
    for (int j = 0; j < 8; j++)
      {
          if (cust[j].cashPowerNumber == cshPwr)
          {
              cust[j].customerData.prevAmount = amount;
              cust[j].customerData.prevPower = pwr;
              cust[j].customerData.set = 1;
              cust[j].customerData.month = time;
              return cust[j].customerData;
              break;
          }
      }
}

previousData getPreviousData(int customerCashPowerNo){
    for (int j = 0; j < 8; j++)
    {
        if (cust[j].cashPowerNumber == customerCashPowerNo)
        {
            if (cust[j].customerData.set)
            {
                return cust[j].customerData;
            }
        }
    }
}
// generate token
char *GenerateToken(int cashPower, int amount){
    srand(time(NULL));
    int  token1 = (rand()% (999999 + 1 ));
    int token2 = (rand() % (100000 + 1));
    sprintf(lasttoken, "%d %d 1%d 23%d", token1, token2, cashPower, amount);
    return lasttoken;
}

void keepLastData(){
      Customer *ptr;
    ptr = cust;
    FILE *customers;
    char Buffer[100];
    customers = fopen("customers.txt", "r");
    int i;
    char *token;
    int fieldCount;

    while (fgets(Buffer, 100, customers))
    {
        token = strtok(Buffer, ",");
        fieldCount = 0;
        while (token != NULL)
        {
            if (fieldCount == 0)
            {
                strcpy(ptr->customerName, token);
            }
            else if(fieldCount == 1){
                strcpy(ptr->customerCategory, token);
            }
            else{
                ptr->cashPowerNumber = atoi(token);
            }
            token = strtok(NULL, ",");
            fieldCount++;
        }
        i++;
        ptr++;
    }
}


void main(){
    keepCustomers();
    printf("\n\n\n");
    printf("---------------Welcome to E-Pay-----------------\n\n\n");
    while (cont == 1){
    int cashPower;
    float providedPower;
    char *category;
    int amount;
    Customer custom;
    printf("Enter amount: ");
	scanf("%d",&amount);
    printf("enter cash power number: ");
    scanf("%d",&cashPower);
    category = checkCategory(cashPower);


	if(strcmp(category,"residential")==0){
        float currPower;
        float givenPower;
         
        previousData currentCustomer = {0, 0, 0};
        currentCustomer = getPreviousData(cashPower);
         if (currentCustomer.set == 1)
         {
            amount+=currentCustomer.prevAmount;
           
            currPower = residential(amount); 

            updatePrevData(amount, currPower,cashPower);
            givenPower = currPower - currentCustomer.prevPower;
            printf("the power is : %f kwh\n", givenPower);
            printf("Enter this number: %s\n", GenerateToken(cashPower, amount));

         }
         else{
             currPower = residential(amount);
             currentCustomer = updatePrevData(amount, currPower,cashPower);
             printf("%f kwh\n", currPower);
            printf("Enter this number: %s\n", GenerateToken(cashPower, amount));
         }
	    }else if(strcmp(category,"non-residential")==0){
            	
	    }else if(strcmp(category,"Hotels")==0){
             providedPower = amount/157;
		printf("%f kwh\n",providedPower);
	    	printf("Enter this number: %s\n",GenerateToken(cashPower,amount));
	}else if(strcmp(category,"telecom-towers")==0){
                providedPower = amount/201;
		printf("%f kwh\n",providedPower);
	    	printf("Enter this number: %s\n",GenerateToken(cashPower,amount));
        }else if(strcmp(category,"Water Treatment plants and Water pumping stations")==0){
                providedPower = amount/126;
		printf("%f kwh\n",providedPower);
	    	printf("Enter this number: %s\n",GenerateToken(cashPower,amount));
	}else if(strcmp(category,"Health Facilities")==0){
	    	providedPower = amount/186;
	    	printf("%f kwh\n",providedPower);
	    	printf("Enter this number: %s\n",GenerateToken(cashPower,amount));
	    }else if(strcmp(category,"Broadcasters")==0){
            providedPower = amount/192;
	    	printf("%f kwh\n",providedPower);
	    	printf("Enter this number: %s\n",GenerateToken(cashPower,amount));
        }else if(strcmp(category,"Commercial Data Centers")==0){
            providedPower = amount/179;
	    	printf("%f kwh\n",providedPower);
	    	printf("Enter this number: %s\n",GenerateToken(cashPower,amount));

	    }else{
	    	printf("The customer does not exist");
	    	exit(-1);
	    }
          printf("Do you want to continue (1,0): ");
          scanf("%d", &cont);
    }
}

