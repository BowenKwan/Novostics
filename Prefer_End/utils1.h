#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <chrono>
#include <numeric>
#include <algorithm> 

using namespace std;


vector<int> sort_copy(vector<int>& original);

void print_vector(vector<int> a);

void print_2dvector(vector<tuple<int,int>> a);

void print_10dvector(vector<tuple<int,int,int,double,int,int,int,int,int,int>> a);



int get_median_m3(vector<int>& a, vector<int>& a_index, int n, int val);

void erase_temp_vector(vector<int>& a, vector<int>& a_index, int n);
int update_median(vector<int>& a, vector<int>& a_index, int n, int val);
void erase_median(vector<int>& a, vector<int>& a_index, int n, int range, int start_pos);
void update_result(vector<tuple<int,int,int,double,int,int,int,int,int,int>>& result, int pos, int end_count, int length, int lambda, double p_val, vector<int> upstream, int upstream_median);
void update_vectors(vector<tuple<int,int,int,double,int,int,int,int,int,int>>& result, int pos, int end_count, vector<int>& median, vector<int>& median_index, vector<int>& upstream, vector<int>& upstream_index, int range, int start_pos, vector< tuple<int, int>>& wave);
void count_end(vector<tuple<int,int,int,double,int,int,int,int,int,int>>& result, int pos, int length, vector<int>& end_count, int& count,int& end_count_pt,vector<int>& median, vector<int>& median_index,vector<int>& upstream, vector<int>& upstream_index, int range, int start_pos, vector< tuple<int, int>>& wave);
double poisson_value(int lambda, int end_count);

void update_downstream(vector<tuple<int,int,int,double,int,int,int,int,int,int>>& result, int pos, int lambda, double p_val, vector<int> upstream, int upstream_median, int range, int start_pos);

void update_downstream_end(vector<tuple<int,int,int,double,int,int,int,int,int,int>>& result, int pos, int lambda, double p_val, vector<int> upstream, int upstream_median,int range, int start_pos); 

void end_wrap(vector<tuple<int,int,int,double,int,int,int,int,int,int>>& result,int pos,vector<int>& median, vector<int>& median_index,vector<int>& upstream, vector<int>& upstream_index, int range, int start_pos);
void coverage_start(vector< tuple<int, int>>& coverage_profile, int pos,int count);
void coverage_end(vector< tuple<int, int>>& coverage_profile, int pos);
void vec_to_file(string filename, vector<int> v);
void 2dvec_to_file(string filename, vector<tuple<int,int>> v);
void 10dvec_to_file(string filename, vector<tuple<int,int,int,double,int,int,int,int,int,int>> v);



