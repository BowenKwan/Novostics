#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <chrono>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iterator>
#include <iostream>
#include "utils.h"
using namespace std;


void print_vector(vector<int> a)
{
    for (auto i : a)
        printf("%d ",i);
    printf("\n");
}

void print_2dvector(vector<d2> a)
{
    for (auto i : a)
        printf("%d %d\n",i.pos, i.delta);
    printf("\n");
}

void print_10dvector(vector<d10> a)
{
    for (auto i : a)
        printf("%d %d %d %f %d %d %d %d %d %d \n",i.pos, i.end, i.lambda, i.p_val, i.up_min, i.up_med, i.up_max, i.down_min, i.down_med, i.down_max);
    printf("\n");
}


vector<int> sort_copy(vector<int>& original)
{
   auto copy = original;
   sort(copy.begin(), copy.end());
   return copy;
}

// Method 1 of getting median (sort copy whole vector)
//int get_median_m1(vector<int>& a)
int get_median_m1(vector<int>& a, vector<int>& a_index, int n, int val)

{   a.push_back(val);
    a_index.push_back(n);
    vector<int> sorted(a.size());
    sorted=sort_copy(a);
    return sorted[a.size()/2];
}

// Method 3 of getting median (sort value and keep index)
int get_median_m3(vector<int>& a, vector<int>& a_index, int n, int val)
{
    auto pos = lower_bound(a.begin(), a.end(), val);
    int shared_pos=pos- a.begin();
    a.insert(pos, val);
    a_index.insert(shared_pos+a_index.begin(),n);
    return a[a.size()/2];
}

// erase vector item by specific value from 1 array, and corresponding entry of the other array
void erase_temp_vector(vector<int>& a, vector<int>& a_index, int n)
{
    auto position = std::find(a_index.begin(), a_index.end(), n);
    int shared_position=position- a_index.begin();
    a.erase(a.begin()+shared_position);
    a_index.erase(position);
}


// update median (choose which method to use)
int update_median(vector<int>& a, vector<int>& a_index, int n, int val)
{
//    return get_median_m1(a, a_index, n, val);
    return get_median_m3(a, a_index, n, val);
}

// erase element when out of range
void erase_median(vector<int>& a, vector<int>& a_index, int n,int range, int start_pos)
{
    if(n>=(range+start_pos)){
        erase_temp_vector(a, a_index, n-range);
    }
}

// compute poisson value (p= [lambda^c e^(-lambda)]/c! )
// when lambda = 0, p = 0
// when c = 0, p = e^(-lambda)
// use log method to avoid computing huge factorial as c increaes
double poisson_value(int lambda, int end_count)
{   double temp=0;

    if(lambda==0){
        return 0;
    }
    else if(end_count==0){
        return exp(-lambda);
    }
    else{
        for(int i=1;i<=end_count;i++){
            temp=temp+log(i);
        }
        return exp(end_count*log(lambda)-lambda-temp);
    }
}

// update result to vector
void update_result(vector<d10>& result, int pos, int end_count, int lambda, double p_val, vector<int> upstream, int upstream_median)
{
    //printf("update result %d %d %d\n", *upstream.begin(),upstream_median,*(upstream.end()-1));
    result.push_back({pos, end_count,0,0,*upstream.begin(),upstream_median,*(upstream.end()-1),0,0,0});
}

// update downstream data
// downstream data of element[0] = upstream data of element[range]
// lambda and p-val only update when downstream data is ready
void update_downstream(vector<d10>& result, int pos, int lambda, double p_val,int range, int start_pos)
{
    if(pos>=(2*range+start_pos)){
    result[pos-range-start_pos]={result[pos-range-start_pos].pos, result[pos-range-start_pos].end,lambda, p_val,result[pos-range-start_pos].up_min,result[pos-range-start_pos].up_med, result[pos-range-start_pos].up_max, result[pos-start_pos].up_min, result[pos-start_pos].up_med, result[pos-start_pos].up_max};
    }
    else{
    result[pos-range-start_pos]={result[pos-range-start_pos].pos, result[pos-range-start_pos].end,0, 0,result[pos-range-start_pos].up_min,result[pos-range-start_pos].up_med, result[pos-range-start_pos].up_max, result[pos-start_pos].up_min, result[pos-start_pos].up_med, result[pos-start_pos].up_max};
    }
}
bool compf(d2 a, int pos){ return a.pos< pos; }
// left end of the read (rising edge)
// 1 end at pos x = element[x] + 1
// rising edge is only updated when no more read start at pos x
// update all counts at once
void coverage_start(vector< d2>& coverage_profile, int pos,int count)
{
    auto position = find_if( coverage_profile.begin( ), coverage_profile.end( ),
                             [=]( d2 item )
                             {
                                 return item.pos == pos;
                             } );

    if(position==coverage_profile.end( )){
        d2 temp={pos, numeric_limits<int>::min()};
        //auto pos_lower = distance(coverage_profile.begin(), lower_bound(coverage_profile.begin(),coverage_profile.end(), {pos, numeric_limits<int>::min()}) );
        auto pos_lower1 = lower_bound(coverage_profile.begin(),coverage_profile.end(), pos, compf);
        //printf("pow_lower %td\n",pos_lower);
        //print_2dvector(coverage_profile);
        coverage_profile.insert(pos_lower1,{pos, count});
        //print_2dvector(coverage_profile);
    }
    else {
        int pos_mod=position- coverage_profile.begin();

        coverage_profile[pos_mod]={coverage_profile[pos_mod].pos,coverage_profile[pos_mod].delta + count };
    }

}

// right end of the read (falling edge)
// 1 end at pos x = element[x] - 1
void coverage_end(vector< d2>& coverage_profile, int pos)
{
    auto position = find_if( coverage_profile.begin( ), coverage_profile.end( ),
                             [=]( d2 item )
                             {
                                 return item.pos == pos;
                             } );

    if(position==coverage_profile.end( )){
        d2 temp={pos, numeric_limits<int>::min()};
        //auto pos_lower = distance(coverage_profile.begin(), lower_bound(coverage_profile.begin(),coverage_profile.end(), {pos, numeric_limits<int>::min()}) );
        auto pos_lower1 = lower_bound(coverage_profile.begin(),coverage_profile.end(), pos,compf);
        //printf("pow_lower %td\n",pos_lower);
        //print_2dvector(coverage_profile);
        coverage_profile.insert(pos_lower1,{pos, -1});
        //print_2dvector(coverage_profile);
    }
    else {
        int pos_mod=position- coverage_profile.begin();

        coverage_profile[pos_mod]={coverage_profile[pos_mod].pos,coverage_profile[pos_mod].delta -1 };
    }
}

// update all vectors
// median, median_index, upstream, upstream_index
// write the result to result
void update_vectors(vector<d10>& result, int pos, int end_count, vector<int>& median, vector<int>& median_index, vector<int>& upstream, vector<int>& upstream_index, int range, int start_pos,vector< d2>& wave)
{

    int lambda=update_median(median, median_index, pos, end_count);

    int upstream_median=update_median(upstream, upstream_index, pos, end_count);

    double p_val=poisson_value(lambda, end_count);

    //update_result(result, pos, end_count, lambda, p_val, upstream, upstream_median);
    result.push_back({pos, end_count,0,0,*upstream.begin(),upstream_median,*(upstream.end()-1),0,0,0});
    if(pos>=(range+start_pos)){
        update_downstream(result, pos, lambda, p_val,range, start_pos);
    }

    erase_median(median, median_index, pos, range*2, start_pos);

    erase_median(upstream, upstream_index, pos, range, start_pos);
}

// count end and check end property
void count_end(vector<d10>& result, int pos, int length, vector<int>& end_count, int& count,int& end_count_pt,vector<int>& median, vector<int>& median_index,vector<int>& upstream, vector<int>& upstream_index, int range, int start_pos, vector< d2>& wave)
{
    //printf("%d\n",end_count_pt);
    //printf("%d\n",pos);
    //printf("%d\n",end_count_pt<pos);

    if(end_count_pt==pos){
        count++;
        coverage_end(wave,end_count_pt+length);
    }
    else if(end_count_pt<pos){
        end_count.push_back(count);
        coverage_start(wave,end_count_pt,count);
        coverage_end(wave,pos+length);
        update_vectors(result, end_count_pt, count, median, median_index, upstream, upstream_index, range, start_pos, wave);

        count=1;
        end_count_pt++;
    }
    else {
        printf("Error, READ NOT aligned properly\n");
        printf("current pos: %d", end_count_pt);
        printf("incoming pos: %d", pos);
    }
    while(end_count_pt<pos){
        end_count.push_back(0);
        update_vectors(result, end_count_pt, 0, median, median_index, upstream, upstream_index, range, start_pos,wave);
        end_count_pt++;
    }
}

// update downstream only
// access upstream array for downstream data
// last few downstream data =/= upstream is earlier position as upstream array is not full towards the end
void update_downstream_end(vector<d10>& result, int pos, int lambda, double p_val, vector<int> upstream, int upstream_median,int range, int start_pos)
{
    result[pos-range-start_pos]={result[pos-range-start_pos].pos, result[pos-range-start_pos].end,lambda, p_val,result[pos-range-start_pos].up_min,result[pos-range-start_pos].up_med, result[pos-range-start_pos].up_max, *upstream.begin(),upstream_median,*(upstream.end()-1)};
}

// update downstream for last-"range" data
void end_wrap(vector<d10>& result,int pos,vector<int>& median, vector<int>& median_index,vector<int>& upstream, vector<int>& upstream_index, int range, int start_pos)
{
    for (int i =0;i<range;i++){
        int lambda=median[median.size()/2];

        int upstream_median=upstream[upstream.size()/2];

        double p_val=poisson_value(lambda, 0);

        update_downstream_end(result, pos+i, lambda, p_val, upstream, upstream_median, range, start_pos);

        erase_median(median, median_index, pos+i, range*2, start_pos);

        erase_median(upstream, upstream_index, pos+i, range, start_pos);
    }
}



void vec_to_file(string filename, vector<int> v)
{
    fstream file;
    file.open(filename,ios_base::out);

    ostream_iterator<int> out_itr(file, "\n");
    copy(v.begin(), v.end(), out_itr);

    file.close();

}


ostream& operator<<(std::ostream& os, const d2& i)
{
    os << "pos "<<i.pos<< " " << i.delta<<endl;;
    return os;
}

void vec2d_to_file(string filename, vector<d2> v)
{
    fstream file;
    file.open(filename,ios_base::out);
    printf("before iter");
    ostream_iterator<d2> out_itr(file, "\n");
    copy(v.begin(), v.end(), out_itr);
    printf("before close");
    file.close();

}


ostream& operator<<(std::ostream& os, const d10& i)
{
    //os << get<0>(i) << get<1>(i) << get<2>(i) << get<3>(i) <<  get<4>(i) << get<5>(i) <<  get<6>(i) <<  get<7>(i) << get<8>(i) << get<9>(i);
    os << "pos " << i.pos << " end count " << i.end << " lambda "<<i.lambda <<" p-val " << i.p_val << " upstream min "<< i.up_min <<" upstream med "<< i.up_med << " upstream max "<<i.up_max <<" downstream min "<< i.down_min<<" downstream med " << i.down_med <<" downstream max "<< i.down_max<<endl;
    return os;
}

void vec10d_to_file(string filename, vector<d10> v)
{
    fstream file;
    file.open(filename,ios_base::out);
    ostream_iterator<d10> out_itr(file, "\n");
    copy(v.begin(), v.end(), out_itr);

    file.close();

}
