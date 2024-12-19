#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <chrono>
#include <numeric>
#include <algorithm>

using namespace std;

struct d2
{int pos;
int delta;
};

struct d10
{int pos;
int end;
int lambda;
double p_val;
int up_min;
int up_med;
int up_max;
int down_min;
int down_med;
int down_max;
};


vector<int> sort_copy(vector<int>& original);

void print_vector(vector<int> a);

void print_2dvector(vector<d2> a);

void print_10dvector(vector<d10> a);



int get_median_m3(vector<int>& a, vector<int>& a_index, int n, int val);

void erase_temp_vector(vector<int>& a, vector<int>& a_index, int n);
int update_median(vector<int>& a, vector<int>& a_index, int n, int val);
void erase_median(vector<int>& a, vector<int>& a_index, int n, int range, int start_pos);
void update_result(vector<d10>& result, int pos, int end_count, int length, int lambda, double p_val, vector<int> upstream, int upstream_median);
void update_vectors(vector<d10>& result, int pos, int end_count, vector<int>& median, vector<int>& median_index, vector<int>& upstream, vector<int>& upstream_index, int range, int start_pos, vector< d2>& wave);
void count_end(vector<d10>& result, int pos, int length, vector<int>& end_count, int& count,int& end_count_pt,vector<int>& median, vector<int>& median_index,vector<int>& upstream, vector<int>& upstream_index, int range, int start_pos, vector< d2>& wave);
double poisson_value(int lambda, int end_count);

void update_downstream(vector<d10>& result, int pos, int lambda, double p_val, vector<int> upstream, int upstream_median, int range, int start_pos);

void update_downstream_end(vector<d10>& result, int pos, int lambda, double p_val, vector<int> upstream, int upstream_median,int range, int start_pos);

void end_wrap(vector<d10>& result,int pos,vector<int>& median, vector<int>& median_index,vector<int>& upstream, vector<int>& upstream_index, int range, int start_pos);
void coverage_start(vector< d2>& coverage_profile, int pos,int count);
void coverage_end(vector< d2>& coverage_profile, int pos);

void vec_to_file(string filename, vector<int> v);
void vec2d_to_file(string filename, vector<d2> v);
void vec10d_to_file(string filename, vector<d10> v);


