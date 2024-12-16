
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

vector<int> sort_copy(vector<int>& original)
{
   auto copy = original;
   sort(copy.begin(), copy.end());
   return copy;
}

//int main(int argc, char *argv[])
int main()
{
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    //vector<int> a = {1,2,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
    int N = 101;
    vector<int> a(N);
 
    iota(a.begin(), a.end(), 1);
    //reverse(a.begin(), a.end());
    random_shuffle(a.begin(), a.end());
    
    int Median;

    print_vector(a);
    printf("\n");

    //Method 1 Pass by value and sort 
    printf("Method 1 Pass by Value\n");

    vector<int> a_sorted1(a.size());
    auto t1 = high_resolution_clock::now();
    a.push_back(9);
    a.erase(a.begin());
    a_sorted1=sort_copy(a);
    Median=a_sorted1[a.size()/2];

    auto t2 = high_resolution_clock::now();

    /* Getting number of milliseconds as an integer. */
    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    /* Getting number of milliseconds as a double. */
    duration<double, std::milli> ms_double = t2 - t1;

    printf("%lld ms\n",ms_int.count());
    printf("%f ms\n",ms_double.count());
    
    
    //Method 2 partial_sort_copy
    //a = {1,2,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};


    printf("Method 2 Partial Sort Copy\n");
    
    iota(a.begin(), a.end(), 1);
    //reverse(a.begin(), a.end());
    random_shuffle(a.begin(), a.end());
    
   
    vector<int> a_sorted2(a.size());
    t1 = high_resolution_clock::now();
    a.push_back(9);
    a.erase(a.begin());
    //print_vector(a);
    //print_vector(a_sorted2);

    partial_sort_copy(begin(a), end(a), begin(a_sorted2), end(a_sorted2));
    Median=a_sorted2[a.size()/2];
    t2 = high_resolution_clock::now();
    /* Getting number of milliseconds as an integer. */
    ms_int = duration_cast<milliseconds>(t2 - t1);

    /* Getting number of milliseconds as a double. */
    ms_double = t2 - t1;
    //print_vector(a);
    //print_vector(a_sorted2);
    //printf("Median: %d\n", Median);

    printf("%lld ms\n",ms_int.count());
    printf("%f ms\n",ms_double.count());


    //Method 3 Sort Index
    printf("Method 3 Sort Index\n");

    iota(a.begin(), a.end(), 1);
    
    vector<int> a_index(a.size());
    iota(a_index.begin(), a_index.end(), 1);
    random_shuffle(a_index.begin(), a_index.end());
    //print_vector(a_index);

    t1 = high_resolution_clock::now();
    auto position = std::find(a_index.begin(), a_index.end(), 1);
    int shared_position=position- a_index.begin();
    a.erase(a.begin()+shared_position);
    a_index.erase(position);
    
    auto pos = lower_bound(a.begin(), a.end(), 9);
    int shared_pos=pos- a.begin();
    a.insert(pos, 9);
    a_index.insert(shared_pos+a_index.begin(),N+1);

    Median=a[a.size()/2];
    t2 = high_resolution_clock::now();
  	// Insert element at correct position

    /* Getting number of milliseconds as an integer. */
    ms_int = duration_cast<milliseconds>(t2 - t1);

    /* Getting number of milliseconds as a double. */
    ms_double = t2 - t1;

    printf("%lld ms\n",ms_int.count());
    printf("%f ms\n",ms_double.count());


    return 0;


}