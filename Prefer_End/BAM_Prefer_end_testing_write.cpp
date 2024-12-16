#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <chrono>
#include <numeric>
#include <algorithm> 

#include <htslib/sam.h>
using namespace std;
//using namespace std::chrono_literals;
void print_vector(vector<int> a){
    for (auto i : a)
        printf("%d ",i);
    printf("\n");
}
void print_2dvector(vector<tuple<int,int>> a){
    for (auto i : a)
        printf("%d %d\n",get<0>(i), get<1>(i));
    printf("\n");
}

void print_10dvector(vector<tuple<int,int,int,double,int,int,int,int,int,int>> a){
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

    vector<int> pos = {1,2,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
    vector<int> length = {11,12,12,13,14,15,16,17,18,19,110,111,112,113,114,115,116,117,118,119,120};

    //for(int i =0;i<pos.size();i++)
    for(int i =0;i<2;i++)
    
    {
        printf("%d iteration \n",i);
        //print_10dvector(result);
        temp={pos[i],length[i],0,0,0,0,0,0,0,0};
        //printf("%d %d %d %f %d %d %d %d %d %d \n",get<0>(temp), get<1>(temp),get<2>(temp),get<3>(temp), get<4>(temp),get<5>(temp), get<6>(temp), get<7>(temp), get<8>(temp), get<9>(temp));


        //print_10dvector(result);
        lambda_temp.push_back(pos[i]);
        stream_temp.push_back(pos[i]);


        //printf("testing%d \n",*stream_temp.begin());

        temp={pos[i],length[i],0,0,*stream_temp.begin(),0,*stream_temp.end(),0,0,0};
        
        if(i>=4){
            stream_temp.erase(stream_temp.begin());
            print_vector(stream_temp);
        }
        if(i>=8){
            lambda_temp.erase(lambda_temp.begin());
            print_vector(lambda_temp);
        }
        
        
        result.push_back(temp);
    }
    print_10dvector(result);
    

    
    return 0;


}