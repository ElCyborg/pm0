/*
 *
 * Main things to know:
 *
 * -999 is the place in the stack that we place the separation for the new activation record. needs some touch up which I will be doing.
 *
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3
#define OUT "trace.txt"
#define IN "mcode.txt"   //<-- output and input file names. Will be adjusted for final release( To accept arguments for passing in files


//struct as outlined in the instructions.
typedef struct _instruction{

    int op; //opcode
    int l;  // L
    int m;  // M

}instruction;

// global variable declarations
int stack[MAX_STACK_HEIGHT];// set the stack size( Will be adjusted later since the stack doesn't need to be a global variable
int sp=0, bp = 1, pc = 0, temp = 0, numOfInstructions = 0; //numofinstrcutions is used to determine the number of instructions. Temp is not completely necessary.
int i = 0, scrap = 0;//possibly redundant variables. used for the loops.
instruction ir[MAX_STACK_HEIGHT];
char instructionWord[3];


//function declarations.

void opr();
void lod();
void sto();
void cal();
void inc();
void jmp();
void jpc();
void sio();
void lit();
char *opname(instruction op);
void chooseCorrectFunction();
int commandParser(char *filename);
void printTheStack();


void main(int argc, char *argv[]){
	int ret;

	//function takes in the instruction file and populates the ir stack;
	ret = commandParser(argv[1]);
	if (ret == -1){
		exit(-1);
	}

    for(i = 0; i < numOfInstructions; i++){// initialize all stack array members to 0( Probably will be changed to only the first 3 positions.
		stack[i]=0;
	}

	printf("PL/0 code:\n\n");
	for(i = 0; i < numOfInstructions; i++){

		strcpy(instructionWord,opname(ir[i]));

		//if we use STO or CAL, the level must be printed out differntly.
		if (ir[i].op == 2 || ir[i].op == 9) {
			printf("%3d%5s\n",i, instructionWord, ir[i].m);
		} else if (ir[i].op == 4 || ir[i].op == 5 ){
			printf("%3d%5s%5d%5d\n",i, instructionWord, ir[i].l,ir[i].m);
		} else {
			printf("%3d%5s%10d\n",i, instructionWord, ir[i].m);
		}
	}
	printf("\n" );


	///////////////////////////
	////START EXECUTE LOOP/////
	///////////////////////////

	printf("Executon:\n");
	printf("%24s%5s%5s%8s\n", "pc","bp","sp","stack");
	for (i = 0; i < numOfInstructions; i++) {

		strcpy(instructionWord,opname(ir[i]));


		//printf("Instruction number: %d\n",i);
		chooseCorrectFunction(); // <-----hangs here!!!!!!!!!!!!!!!!!!!!

		if (ir[i].op == 2 || ir[i].op == 9) {
			//printf("%3d%5s%5d%5d%6d%5d%5d", ir[i].m, instructionWord );
			//printTheStack();

		} else if (ir[i].op == 4 || ir[i].op == 5){
			//printf("%3d%5s%5d%5d%6d%5d%5d",i, instructionWord, ir[i].m);
			//printTheStack();

		} else {
			//printf("%3d%5s%10d%6d%5d%5d",i, instructionWord, ir[i].l, ir[i].m);
			//printTheStack();
		}

		//the formatting for the exection section is as follows
		printf("%3d%5s%5d%5d%6d%5d%5d\n",pc,instructionWord,ir[i].l,ir[i].m,pc,bp,sp);
		//printTheStack();
		pc++;
	}

	// innner for loop that outputs the current contents of the stack


    for(temp=0; temp<17; temp++){
    	printf(" %d", stack[temp]);
    	printf(" \n");
    }// end of for loop control


}

void printTheStack(){

	printf("Print the STACK\n" );

}

//written by Ryan Hoeck
//this function was taken and modified from one of my previous programs
int commandParser(char *filename){
	FILE *fp;
    char *line;
    char *temp;
    char *token;
	int n;


    if (filename == NULL) {
		perror("You did not input a filename\n");
        return -1;
    }

    fp = fopen(filename,"r");
    if (fp == NULL) {
		perror("You did not input a valid filename\n");
        return -1;
    }

    line = malloc(sizeof(char) * MAX_CODE_LENGTH + 1);
    temp = fgets(line, MAX_CODE_LENGTH + 1, fp);

	if (temp == NULL) {
		return -1;
	}

    while (line != NULL && temp != NULL) {
        token = strtok(line," ");
		ir[numOfInstructions].op = atoi(token);

		token = strtok(NULL," ");
		ir[numOfInstructions].l = atoi(token);

		token = strtok(NULL," ");
		ir[numOfInstructions].m = atoi(token);

		temp = fgets(line, MAX_CODE_LENGTH + 1, fp);
		numOfInstructions++;
    }

	free(line);
    fclose(fp);

	return 0;
}


/*
 *
 *
 * Base
 *
 *  Base function provided in the instructions, modified for global variables.
 */
int base(int base, int L){
    int b1; //find base L levels down
    b1 = base;
    while (L > 0){
        b1 = stack[b1 + 1];
        L--;
    }
    return b1;
}



/*
 *
 *
 * choose
 *
 *  initiates the appropriate function correlating to the appropriate op code.
 */
void chooseCorrectFunction(){

    switch (ir[i].op){
        case 1:
			//printf("a\n");
            lit();
            break;

        case 2:
		//	printf("b\n");
            opr();
            break;

        case 3:
			//printf("c\n");
            lod();
            break;

        case 4:
		//	printf("d\n");
            sto();
            break;

        case 5:
			//printf("e\n");
            cal();
            break;

        case 6:
			//printf("f\n");
            inc();
            break;

        case 7:
			//printf("g\n");
            jmp();
            break;

        case 8:
		//	printf("h\n");
            jpc();
            break;

        case 9:
		//	printf("i\n");
            sio();
            break;

        case 10:
			//printf("j\n");
            sio();
            break;

        case 11:
			//printf("k\n");
            sio();
            break;

        default:
			//printf("l\n");
            exit(0);
            break;
    }


}


/*
 *
 *
 * lit
 *
 *  performs the LIT operation as outlined in the homework
 */
void lit(){
    sp = sp + 1;
    //stack[sp] = -999; //already incremented sp by 1
    stack[sp] = ir[i].m;
}

/*
 *
 *
 * opr
 *
 *  performs the OPR operation as outlined in the homework
 */
void opr(){

    if(ir[i].m == 0){
        sp = bp - 1;
        bp = stack[sp + 3];
        pc = stack[sp + 4];
        stack[sp + 1] = -999;

    }

    if(ir[i].m == 1){
        stack[sp] = -stack[sp];
    }

    if(ir[i].m == 2){
        stack[sp] = stack[sp] + stack[sp + 1];
        sp = sp - 1;
    }

    if(ir[i].m == 3){
        stack[sp] = stack[sp] - stack[sp + 1];
        sp = sp - 1;
    }


    if(ir[i].m == 4){
        stack[sp] = stack[sp] * stack[sp + 1];
        sp = sp - 1;
    }

    if(ir[i].m == 5){
        stack[sp] = stack[sp] / stack[sp + 1];
        sp = sp - 1;
    }

    if(ir[i].m == 6){
        stack[sp] = stack[sp] % 2;

    }

    if(ir[i].m == 7){

        stack[sp] = stack[sp] % stack[sp + 1];
        sp = sp - 1;

    }

    if(ir[i].m == 8){
        stack[sp] =  ((stack[sp] == stack[sp + 1]) ? 1 : 0);
        sp = sp - 1;
    }

    if(ir[i].m == 9){
        stack[sp] =  ((stack[sp] != stack[sp + 1]) ? 1 : 0);
        sp = sp - 1;
    }

    if(ir[i].m == 10){
        stack[sp] =  ((stack[sp + 1] - stack[sp])>0 ? 1 : 0);
        sp = sp - 1;

    }

    if(ir[i].m == 11){
        stack[sp] =  ((stack[sp + 1] - stack[sp])>=0 ? 1 : 0);
        sp = sp - 1;
    }

    if(ir[i].m == 12){
        stack[sp] =  (stack[sp] - (stack[sp + 1])>0 ? 1 : 0);
        sp = sp - 1;
    }

    if(ir[i].m == 13){
        stack[sp] =  (stack[sp] - (stack[sp + 1])>=0 ? 1 : 0);
        sp = sp - 1;
    }

    if(ir[i].m != 0)
        pc++;

}



/*
 *
 *
 * lod
 *
 *  performs the LOD operation as outlined in the homework
 */
void lod(){
    sp = sp + 1;
    stack[sp] = stack[base(bp, ir[i].l) + ir[i].m];
}


/*
 *
 *
 * sto
 *
 *  performs the STO operation as outlined in the homework
 */
void sto(){

	stack[base(bp, ir[i].l) + ir[i].m] = stack[sp]; // remember that L is a global, so you only pass in bp
    stack[sp+1] = 0;
    stack[sp] = -999;
    sp = sp - 1;

    pc++;
}
/*
 *
 *
 * cal
 *
 *  performs the CAL operation as outlined in the homework
 */
void cal(){

    stack[sp + 1] = 5; // space to return value (FV)
    stack[sp + 2] = base(bp, ir[i].l); // static link (SL)
    stack[sp + 3] = bp; // dynamic link (DL)
    stack[sp + 4] = pc+1; // return address (RA)
    stack[sp + 7] = -999;//new end index
    bp = sp + 1;
    pc = ir[i].m;


}



/*
 *
 *
 * inc
 *
 *  performs the INC operation as outlined in the homework
 */
void inc(){

    sp = sp + ir[i].m;

}



/*
 *
 *
 * jmp
 *
 *  performs the JMP operation as outlined in the homework
 */
void jmp(){
    pc = ir[i].m;
}


/*
 *
 *
 * jpc
 *
 *  performs the JPC operation as outlined in the homework
 */
void jpc(){

    if (stack[sp] == 0){
        pc = ir[i].m;
    }else
        pc++;

    sp = sp - 1;

}



/*
 *
 *
 * sio
 *
 *  performs the SIO operation as outlined in the homework
 */
void sio(){

    if(ir[i].m == 0){
        printf("%d\n", stack[sp]);
        sp = sp - 1;
    }

    if(ir[i].m == 1){
        sp = sp + 1;
        scanf("\n\n\t Please enter value: %d", &stack[sp]);

    }

    if(ir[i].m == 2){
        exit(1);
    }

}// end sio



/*
 *
 *
 * opname
 *
 *  returns a string of the operation name based on it's integer representation.
 */
char *opname(instruction op){

    switch (op.op){
        case 1:
            return "LIT";
            break;

        case 2:
            switch (op.l) {
            	case 0:
					return "RET";
					break;

				case 1:
					return "NEG";
					break;

				case 2:
					return "ADD";
					break;

				case 3:
					return "SUB";
					break;

				case 4:
					return "MUT";
					break;

				case 5:
					return "DIV";
					break;

				case 6:
					return "ODD";
					break;

				case 7:
					return "MOD";
					break;

				case 8:
					return "EQL";
					break;

				case 9:
					return "NEQ";
					break;

				case 10:
					return "LLS";
					break;

				case 11:
					return "LEQ";
					break;

				case 12:
					return "GTR";
					break;

				case 13:
					return "GEQ";
					break;

				default:
					return "HLT";
					break;
            }
            break;

        case 3:
            return "LOD";
            break;

        case 4:
            return "STO";
            break;

        case 5:
            return "CAL";
            break;

        case 6:
            return "INC";
            break;

        case 7:
            return "JMP";
            break;

        case 8:
            return "JPC";
            break;

        case 9:
            return "OUT";
            break;

        case 10:
            return "SIO";
            break;

        case 11:
            return "HLT";
        	break;

        default:
            return "HLT";
            break;
    }
}
