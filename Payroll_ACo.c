//********************-=-=-=-=-=-=-=-=-=-=-=-=-********************// 
//******************<       PAYROLL PROGRAM      >*****************//
//********************-=-=-=-=  V 2.0  -=-=-==-********************//
//********                     AUTHOR                      ********//
//---------<_>-<_>--->>>   ALISTAIR COOPER   <<<---<_>-<_>---------//
//*****************************************************************//

// C Libraries

#include <stdio.h>
#include <string.h>

// Constants 

#define OT_THRESHOLD 40.   /* threshold (hours) for overtime to take effect */
#define OT_RATE 1.5        /* rate at which overtime is paid                */

// Global

FILE *reportFile;          /* report file (for output)                      */
char fullName[22];         /* employee full name                            */

// Function Prototypes

void header(void);                         // Prototype for header function

void getInfo(int empNum,                   // Prototype for getInfo function
             float *hourlyWagep,        
             float *hoursWorkedp);
             
void processInfo(float hourlyWage,         // Prototype for processInfo function
                 float hoursWorked,
                 float *hoursRegp,       
                 float *hoursOTp, 
                 float *empGrossp,
                 float *otWagep,
                 float *totalRegHoursp,
                 float *totalOTHoursp,
                 float *totalRegWgesp,
                 float *totalOTWagesp,
                 float *totalWagesPeriodp);
                 
void printRepLine(float hoursReg,          // Prototype for printRepLine function
                  float hourlyWage, 
                  float hoursOT, 
                  float otWage, 
                  float empGross);
                      
void summary(float totalRegHours,          // Prototype for summary function
             float totalOTHours,
             float totalRegWges,
             float totalOTWages,
             float totalWagesPeriod);      

//********************-=-=-=-=-=-=-=-=-=-=-=-=-**********************//      
//****************<<<  M A I N  F U N C T I O N  >>>****************//   
//********************-=-=-=-=-=-=-=-=-=-=-=-=-********************//

int main(void){
    
// Variables 
    float totalRegHours;       /* OUTPUT - total of regular hours worked       */
    float totalOTHours;        /* OUTPUT - total of overtime hours worked      */
    float totalRegWges;        /* OUTPUT - total of regular wages              */
    float totalOTWages;        /* OUTPUT - total of overtime wages             */
    float totalWagesPeriod;    /* OUTPUT - total of wages this period          */ 
    
    float hoursReg;            /* OUTPUT - employee regular hours worked       */
    float hoursOT;             /* OUTPUT - employee overtime hours worked      */
    float empGross;            /* OUTPUT - employee gross pay                  */
    float hourlyWage;          /* OUTPUT - employee hourly wage                */
    float hoursWorked;         /* OUTPUT - employee total number of hours      */
    float otWage;              /* OUTPUT - employee overtime wage              */ 
    
    int empNum;                /* count to keep track of do-while iterations   */
    char enterAnother;         /* variable to act as loop invariant            */  
    
    reportFile = fopen("c:\\payroll-output.txt", "w");    // Open output file

// Prints Header to Report File

	header();  
	
// Main Processor Loop

    // Initialize Counter Variable
    empNum = 0;

    do {
        
        empNum++;  /* Increases accumulator count */
        
        /* Gets info for employee */            
	    getInfo(empNum, &hourlyWage, &hoursWorked);
                        
        /* Process infomation, updates totals */
        processInfo(hourlyWage, hoursWorked, &hoursReg, &hoursOT, 
                    &empGross, &otWage, &totalRegHours, &totalOTHours,
                    &totalRegWges, &totalOTWages, &totalWagesPeriod);
                    
        /* Prints employee line to report */
        printRepLine(hoursReg, hourlyWage, hoursOT, otWage, empGross);
         
        // Ask user if they want to enter another employee    
        printf("Thank you. Process another employee? y or n ");
        scanf("%s", &enterAnother);
        
        printf("\n");
        
    } while(enterAnother != 'N' && enterAnother != 'n');
    
    // print to screen     
    printf("End of processing.");
     
    /* Prints summary to report file */            
	summary(totalRegHours, totalOTHours, totalRegWges, totalOTWages, totalWagesPeriod); 
                        
    fclose(reportFile);
    return 0;
}

//--->>>   header FUNCTION   >>>==================================================
// Print the header to the report file
// Also prints header to screen and prompt for user to enter department name
//================================================================================

void header(void){

// local variables
    char department[21];       /* name of department                           */

// prints lines to report file    
    fprintf(reportFile, "Dunder Mifflin Paper Company\n");                  
    fprintf(reportFile, "Department Salary Program\n\n"); 

// prints lines to screen
    printf("Dunder Mifflin Paper Company\n");                  
    printf("Department Salary Program\n\n");    
    
// Ask user for department name and print to screen                            
    printf("Please enter the name of the department: "); 
    scanf("%s", &department);
    
    printf("\n");

// print department name to disk
    fprintf(reportFile, "Department: %s\n\n", department); 
    
// print column titles to report
    fprintf(reportFile, "Employee                Reg Hrs        Overtime Hrs       Gross\n");
    fprintf(reportFile, "-----------------------------------------------------------------\n");                           

}

//--->>>   getInfo FUNCTION   >>>=================================================
// Gathers the name and work hour information of the employee.
//================================================================================

void getInfo(int empNum,
             float *hourlyWagep, 
             float *hoursWorkedp) 
{
    
// Local variables
    char firstName[11];        /* employee first name                          */
    char lastName[11];         /* employee last name                           */
        
// Collect information from employee
    printf("Enter employee #%d ", empNum);
    scanf("%s%s", &firstName, &lastName);
        
    printf("Enter the hourly wage of %s %s: ", firstName, lastName);
    scanf("%f", &*hourlyWagep);    
        
    printf("Enter total number of hours: ");
    scanf("%f", &*hoursWorkedp); 
        
    printf("\n"); 
        
// Concatenate name
    strcpy(fullName, firstName);
    strcat(fullName, " ");
    strcat(fullName, lastName);         
        
} 

//--->>>   processInfo FUNCTION   >>>=============================================
// Takes the employee infomation, calculates the totals.
//================================================================================

void processInfo(float hourlyWage,
                 float hoursWorked,
                 float *hoursRegp,       
                 float *hoursOTp, 
                 float *empGrossp,
                 float *otWagep,
                 float *totalRegHoursp,
                 float *totalOTHoursp,
                 float *totalRegWgesp,
                 float *totalOTWagesp,
                 float *totalWagesPeriodp) 
{
// Local variables
    float payReg;              /* employee pay for regular time       */
    float payOT;               /* employee pay for overtime           */ 
    
// Do calculations

    if (hoursWorked < OT_THRESHOLD) {
                
        *hoursRegp = hoursWorked;
        *hoursOTp = 0;
        *otWagep = hourlyWage * OT_RATE;
        payOT = 0;
        payReg = hoursWorked * hourlyWage;
        *empGrossp = payReg;
         
    }
    // Else hours worked is more than overtime threshold 
    else {
            
        *hoursRegp = OT_THRESHOLD;
        *hoursOTp = hoursWorked - OT_THRESHOLD;
        *otWagep = hourlyWage * OT_RATE;
        payReg = *hoursRegp * hourlyWage; 
        payOT = *hoursOTp * *otWagep;
            
        *empGrossp = payReg + payOT;        
    }
    
// Update the running totals
    *totalRegHoursp = *totalRegHoursp + *hoursRegp;
    *totalOTHoursp = *totalOTHoursp + *hoursOTp;
    *totalRegWgesp = *totalRegWgesp + payReg;
    *totalOTWagesp = *totalOTWagesp + payOT;
    *totalWagesPeriodp = *totalWagesPeriodp + *empGrossp;    
    
}

//--->>>   printRepLine FUNCTION   >>>============================================
// Prints employee information to line on report.
//================================================================================

void printRepLine(float hoursReg, 
                  float hourlyWage, 
                  float hoursOT, 
                  float otWage, 
                  float empGross)
{      
    fprintf(reportFile, "%-22s%4.1f ($%5.2f)   %4.1f ($%5.2f)    $%7.2f\n", 
            fullName, hoursReg, hourlyWage, hoursOT, otWage, empGross);   
}

//--->>   summary FUNCTION   >>>==================================================
// The outputReport function calculates percentage total hours overtime   
// Prints totals to report
//================================================================================

void summary(float totalRegHours,
             float totalOTHours,
             float totalRegWges,
             float totalOTWages,
             float totalWagesPeriod) 
{

// Local variables
    float percentageOT;        /* Percentage of total overtime hours  */

// Do calculations

    percentageOT = (totalOTHours / (totalOTHours + totalRegHours)) * 100;
            
// Prints values to report file
	
	fprintf(reportFile, "-----------------------------------------------------------------\n\n");
    fprintf(reportFile, "Total Regular Hours Worked: %.1f\n", totalRegHours); 
    fprintf(reportFile, "Total Overtime Hours Worked: %.1f\n", totalOTHours);
    fprintf(reportFile, "Percentage of Total Hours That Are Overtime: %.1f%%\n\n", percentageOT); 
    
    fprintf(reportFile, "Total Regular Wages: $%.2f\n", totalRegWges);	
	fprintf(reportFile, "Total Overtime Wages: $%.2f\n\n", totalOTWages);
	
	fprintf(reportFile, "Total Wages This Period: $%.2f", totalWagesPeriod);
}



