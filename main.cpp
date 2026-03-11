#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <climits>
#include "mathfunc.h"
#include <unistd.h>

void intChecker(int &a){ //проверка целочисленного типа
    if((a > INT_MAX || a < INT_MIN))
    {
        printf("Number is out of int");
        exit(0);
    }
}

void parse(struct data &calc_Data, int argc, char *argv[]);
void check(struct data &calc_Data);
void calculate(struct data &calc_Data);
void run(struct data &calc_Data, int argc, char *argv[]);
void print(struct data &calc_Data);

void math::sum(int &a, int &b, int &c);
void math::sub(int &a, int &b, int &c);
void math::mul(int &a, int &b, int &c);
void math::div(int &a, int &b, int &c);
void math::max(int &a, int &b, int &c);
int math::fact(int a);

struct data
{
    int firstNum;
    int secondNum;
    char operation; // +(43) -(45) *(97) /(47) ^(94) !(33)
    int result;
};

int main(int argc, char *argv[])
{
    data DataCalc;
    //help
    if((argc == 1)||(strcmp(argv[1], "-h")==0))
    {
        printf("This program use for calculation\n");
        printf("How to use:\n");
        printf("Can addition(+), subtraction(-), multiplication(*), division(/), exponentiation(^), factorial(!)\n");
        printf("If you need like addition you type 2 numbers and expression\n");
        printf(" -f 5 -o + -s 5\n");
        exit(0);
    }
    //check
    if(argc != 8)
    {
        printf("You insert wrong expression");
        exit(0);
    }
    //start
    run(DataCalc, argc, argv);
}

void parse(struct data &d, int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "f:o:s:")) != -1) {
        switch (opt) {
            case 'f':
                d.firstNum = atol(optarg);
                break;

            case 'o':
                d.operation = optarg[0];
                break;

            case 's':
                d.secondNum = atol(optarg);
                break;

            default:
                exit(0);
        }
    }
}

void check(struct data &calcData)
{
    intChecker(calcData.firstNum);
    intChecker(calcData.secondNum);
}

void calculate(struct data &calcData)
{
    try{
    switch((int)calcData.operation) {
        case 43: 
            math::sum(calcData.firstNum, calcData.secondNum, calcData.result);
            break;
        case 45:
            math::sub(calcData.firstNum, calcData.secondNum, calcData.result);
            break;
        case 97:
            math::mul(calcData.firstNum, calcData.secondNum, calcData.result);
            break;
        case 47:
            math::div(calcData.firstNum, calcData.secondNum, calcData.result);
            break;
        case 94:
            math::max(calcData.firstNum, calcData.secondNum, calcData.result);
            break;
        case 33:
            calcData.result = math::fact(calcData.firstNum);
            break;
    }
    }catch(int a){
        printf("Int out of range, exit from program \n");
        exit(0);
    }
    catch(double a){
        printf("divide on zero \n");
        exit(0);
    }
    
}

void print(struct data &calcData)
{
    printf("Result of calculating is:\n");
    printf("%d" ,calcData.result);
}

void run(struct data &calcData, int argc, char *argv[]){
    parse(calcData, argc, argv);
    check(calcData);
    calculate(calcData);
    print(calcData);
}