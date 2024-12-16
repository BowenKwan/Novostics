#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <chrono>
#include <numeric>
#include <algorithm> 

#include <htslib/sam.h>

#include "utils.h"

using namespace std;
//using namespace std::chrono_literals;

void print_10dvector1(vector<tuple<int,int,int,double,int,int,int,int,int,int>> a){
    for (auto i : a)
        printf("%d %d %d %f %d %d %d %d %d %d \n",get<0>(i), get<1>(i),get<2>(i),get<3>(i), get<4>(i),get<5>(i), get<6>(i), get<7>(i), get<8>(i), get<9>(i));
    printf("\n");
}


//int main(int argc, char *argv[])
int main()
{
/*
    vector<tuple<int,int>> a = {tuple<int,int>{1,2},tuple<int,int>{3,5},tuple<int,int>{11,12}};
    print_2dvector(a);
    printf("%d %d\n",get<0>(a[1]),get<1>(a[1]));
    a[1]={get<0>(a[1]),7};
    printf("%d %d\n",get<0>(a[1]),get<1>(a[1]));

    printf("\n");
*/
    //data struct (end count, length, lambda, P-val, Up (min, median, max), Down (min, median, max))
    vector<tuple<int,int,int,double,int,int,int,int,int,int>> result;
    vector<int> lambda_temp;
    vector<int> stream_temp;
    tuple<int,int,int,double,int,int,int,int,int,int> temp;

    vector<int> pos = {0,1,1,4,5,6,7};
    vector<int> length = {3,3,2,4,5,6,6};
    //for(int i =0;i<pos.size();i++)
    
    vector <tuple<int,int>> wave;

    vector<int> end_count;

    int end_count_pt=0;

    int count=0;

    for (int i=0;i<7;i++)
    {
    printf("i %d\n",i);

    printf("pos leng %d %d\n",pos[i],length[i]);

    if(end_count_pt==pos[i]){
        printf("in ==\n");

        printf("%d\n",count);
        count++;
        printf("%d\n",count);
        coverage_end(wave,end_count_pt+length[i]);
    }
    else if(end_count_pt<pos[i]){
        printf("in <\n");
        end_count.push_back(count);
        printf("end count\n");
        print_vector(end_count);
        coverage_start(wave,end_count_pt,count);
        print_2dvector(wave);
        coverage_end(wave,pos[i]+length[i]);
        print_2dvector(wave);
        count=1;
        end_count_pt++;
    }
    else {
        printf("Error, READ NOT aligned properly\n");
        printf("current pos: %d", end_count_pt);
        printf("incoming pos: %d", i);
    }
    while(end_count_pt<pos[i]){
        printf("in while %d",i);
        end_count.push_back(0);
        end_count_pt++;
        print_vector(end_count);
    }
}
    printf("in <\n");
    end_count.push_back(count);
    printf("end count\n");
    print_vector(end_count);
    coverage_start(wave,end_count_pt,count);
    //coverage_end(wave,end_count_pt+length[6]);
    count=1;
    end_count_pt++;
    
print_2dvector(wave);    
return 0;


}