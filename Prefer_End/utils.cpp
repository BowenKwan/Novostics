#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <chrono>
#include <numeric>
#include <algorithm> 

using namespace std;


void print_vector(vector<int> a)
{
    for (auto i : a)
        printf("%d ",i);
    printf("\n");
}

void print_2dvector(vector<tuple<int,int>> a)
{
    for (auto i : a)
        printf("%d %d\n",get<0>(i), get<1>(i));
    printf("\n");
}

void print_10dvector(vector<tuple<int,int,int,double,int,int,int,int,int,int>> a)
{
    for (auto i : a)
        printf("%d %d %d %f %d %d %d %d %d %d \n",get<0>(i), get<1>(i),get<2>(i),get<3>(i), get<4>(i),get<5>(i), get<6>(i), get<7>(i), get<8>(i), get<9>(i));
    printf("\n");
}


vector<int> sort_copy(vector<int>& original)
{
   auto copy = original;
   sort(copy.begin(), copy.end());
   return copy;
}

int get_median_m1(vector<int>& a)
{
    vector<int> sorted(a.size());
    sorted=sort_copy(a);
    return sorted[a.size()/2];
}
int get_median_m3(vector<int>& a, vector<int>& a_index, int n, int val)
{
    auto pos = lower_bound(a.begin(), a.end(), val);
    int shared_pos=pos- a.begin();
    a.insert(pos, val);
    a_index.insert(shared_pos+a_index.begin(),n);
    return a[a.size()/2];
}

void erase_temp_vector(vector<int>& a, vector<int>& a_index, int n)
{
    auto position = std::find(a_index.begin(), a_index.end(), n);
    int shared_position=position- a_index.begin();
    a.erase(a.begin()+shared_position);
    a_index.erase(position);
}



int update_median(vector<int>& a, vector<int>& a_index, int n, int val)
{
    return get_median_m3(a, a_index, n, val);
}

void erase_median(vector<int>& a, vector<int>& a_index, int n,int range, int start_pos)
{
    if(n>=(range+start_pos)){
        erase_temp_vector(a, a_index, n-range);
    }
}


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

void update_result(vector<tuple<int,int,int,double,int,int,int,int,int,int>>& result, int pos, int end_count, int lambda, double p_val, vector<int> upstream, int upstream_median)
{ 
    //printf("update result %d %d %d\n", *upstream.begin(),upstream_median,*(upstream.end()-1));
    result.push_back({pos, end_count,0,0,*upstream.begin(),upstream_median,*(upstream.end()-1),0,0,0});
}
void update_downstream(vector<tuple<int,int,int,double,int,int,int,int,int,int>>& result, int pos, int lambda, double p_val,int range, int start_pos)
{ 
    if(pos>=(2*range+start_pos)){
    result[pos-range-start_pos]={get<0>(result[pos-range-start_pos]), get<1>(result[pos-range-start_pos]),lambda, p_val,get<4>(result[pos-range-start_pos]),get<5>(result[pos-range-start_pos]), get<6>(result[pos-range-start_pos]), get<4>(result[pos-start_pos]), get<5>(result[pos-start_pos]), get<6>(result[pos-start_pos])};
    }
    else{
    result[pos-range-start_pos]={get<0>(result[pos-range-start_pos]), get<1>(result[pos-range-start_pos]),0,0,get<4>(result[pos-range-start_pos]),get<5>(result[pos-range-start_pos]), get<6>(result[pos-range-start_pos]), get<4>(result[pos-start_pos]), get<5>(result[pos-start_pos]), get<6>(result[pos-start_pos])};    
    }
}


void coverage_start(vector< tuple<int, int>>& coverage_profile, int pos,int count)
{
    auto position = find_if( coverage_profile.begin( ), coverage_profile.end( ),
                             [=]( tuple<int, int> item )
                             {
                                 return get< 0 >( item ) == pos;
                             } );
                             
    if(position==coverage_profile.end( )){

        auto pos_lower = distance(coverage_profile.begin(), lower_bound(coverage_profile.begin(),coverage_profile.end(), make_tuple(pos, numeric_limits<int>::min())) );
        auto pos_lower1 = lower_bound(coverage_profile.begin(),coverage_profile.end(), make_tuple(pos, numeric_limits<int>::min()));
        //printf("pow_lower %td\n",pos_lower);
        //print_2dvector(coverage_profile);
        coverage_profile.insert(pos_lower1,{pos, count});
        //print_2dvector(coverage_profile);
    }
    else {
        int pos_mod=position- coverage_profile.begin();
    
        coverage_profile[pos_mod]={get< 0 >( coverage_profile[pos_mod]),get< 1 >( coverage_profile[pos_mod]) + count };
    }
    
}


void coverage_end(vector< tuple<int, int>>& coverage_profile, int pos)
{
    auto position = find_if( coverage_profile.begin( ), coverage_profile.end( ),
                             [=]( tuple<int, int> item )
                             {
                                 return get< 0 >( item ) == pos;
                             } );
                             
    if(position==coverage_profile.end( )){

        auto pos_lower = distance(coverage_profile.begin(), lower_bound(coverage_profile.begin(),coverage_profile.end(), make_tuple(pos, numeric_limits<int>::min())) );
        auto pos_lower1 = lower_bound(coverage_profile.begin(),coverage_profile.end(), make_tuple(pos, numeric_limits<int>::min()));
        //printf("pow_lower %td\n",pos_lower);
        //print_2dvector(coverage_profile);
        coverage_profile.insert(pos_lower1,{pos, -1});
        //print_2dvector(coverage_profile);
    }
    else {
        int pos_mod=position- coverage_profile.begin();
    
        coverage_profile[pos_mod]={get< 0 >( coverage_profile[pos_mod]),get< 1 >( coverage_profile[pos_mod]) -1 };
    }
}


void update_vectors(vector<tuple<int,int,int,double,int,int,int,int,int,int>>& result, int pos, int end_count, vector<int>& median, vector<int>& median_index, vector<int>& upstream, vector<int>& upstream_index, int range, int start_pos,vector< tuple<int, int>>& wave)
{
    
    int lambda=update_median(median, median_index, pos, end_count);

    int upstream_median=update_median(upstream, upstream_index, pos, end_count);

    double p_val=poisson_value(lambda, end_count);

    update_result(result, pos, end_count, lambda, p_val, upstream, upstream_median);
    
    if(pos>=(range+start_pos)){
        update_downstream(result, pos, lambda, p_val,range, start_pos);
    }   

    erase_median(median, median_index, pos, range*2, start_pos);

    erase_median(upstream, upstream_index, pos, range, start_pos);
}


void count_end(vector<tuple<int,int,int,double,int,int,int,int,int,int>>& result, int pos, int length, vector<int>& end_count, int& count,int& end_count_pt,vector<int>& median, vector<int>& median_index,vector<int>& upstream, vector<int>& upstream_index, int range, int start_pos, vector< tuple<int, int>>& wave)
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

void update_downstream_end(vector<tuple<int,int,int,double,int,int,int,int,int,int>>& result, int pos, int lambda, double p_val, vector<int> upstream, int upstream_median,int range, int start_pos)
{ 
    result[pos-range-start_pos]={get<0>(result[pos-range-start_pos]), get<1>(result[pos-range-start_pos]),lambda, p_val,get<4>(result[pos-range-start_pos]),get<5>(result[pos-range-start_pos]), get<6>(result[pos-range-start_pos]), *upstream.begin(),upstream_median,*(upstream.end()-1)};
}

void end_wrap(vector<tuple<int,int,int,double,int,int,int,int,int,int>>& result,int pos,vector<int>& median, vector<int>& median_index,vector<int>& upstream, vector<int>& upstream_index, int range, int start_pos)
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



