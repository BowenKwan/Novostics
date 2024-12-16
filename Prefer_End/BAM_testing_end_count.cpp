#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <chrono>
#include <numeric>
#include <algorithm> 

using namespace std;

void print_vector(vector<int> a){
    for (auto i : a)
        printf("%d ",i);
    printf("\n");
}


//int main(int argc, char *argv[])
int main()
{
    int pos[18] = {1,2,2,3,7,8,8,9,10,11,11,11,11,12,17,18,19,20};

    vector<int> end_count;

    int end_count_pt=0;

    int count=0;

    for (int i:pos)
{
    printf("%d\n",i);
    if(end_count_pt==i){
        count++;
    }
    else if(end_count_pt<i){
        end_count.push_back(count);
        count=1;
        end_count_pt++;
    }
    else {
        printf("Error, READ NOT aligned properly\n");
        printf("current pos: %d", end_count_pt);
        printf("incoming pos: %d", i);
    }
    while(end_count_pt<i){
        end_count.push_back(0);
        end_count_pt++;
        print_vector(end_count);
    }
}

print_vector(end_count);

return 0;
}