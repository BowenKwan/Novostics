#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <chrono>
#include <numeric>
#include <algorithm> 
#include "utils.h"
#include <cmath>
using namespace std;

//int main(int argc, char *argv[])
int main()
{

    double temp=0;
    int end_count=2;
    int lambda=1;
    for(int i=1;i<=end_count;i++){
        printf("temp %d %f\n",i,temp);
        temp=temp+log(i);
    }
    double p = exp(end_count*log(lambda)-lambda-temp);


  //double p=poisson_value(2,2);
  printf("testing");
  printf("%f",p);
return 0;
}