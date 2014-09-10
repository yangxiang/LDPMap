#ifndef _DICTIONARY_H
#define _DICTIONARY_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <set>
#include "layeredDP.h"

using namespace std;

struct Entry{
string CUI;
bool preferred;
vector< pair<string,int> > name;
vector<char> cont_name;
bool for_cont;
};

class Dictionary {
	private:
		float threshold1;
		float threshold2;
		bool case_sensitive;
		set<string> word_set;
		vector<Entry> dict;
		vector<string> word_vector;
		vector<int> kept_leng_name;
		vector<int> kept_leng_word;
		vector<int> kept_leng_contname;
		vector< vector<char> >word_char_vector;
		//vector<Entry> cont_d;
		//layeredDP layeredDP;
		Entry makeEntry(vector<string>& entry_vect);
		void addEntry(Entry& e);
		void LDPMap_Preprocessing();
		void insert_set(const vector< pair<string,int> >& Name);		
		int search_word_index(int first, int last, string& word);//int search(int first, int last, string& word);
		void make_word_vector();
		void set_word_index();//void update_entry();
		void keep_name_leng();
		int word_kept_leng(const vector<char>& pre_word, const vector<char>& cur_word);
		int name_kept_leng(const vector< pair<string,int> >& pre_name, const vector< pair<string,int> >& cur_name);
		void sort_result(vector< pair<float,int> >& match_scores);//sort_function
		void cut_tail(vector< pair<float,int> >& result);
		void output_result(const vector< pair<float,int> >& result, const int& match_num);//output
		void remove_duplicated_results(vector< pair<float,int> >& result);//remove_dup
		
	public:
		Dictionary(const float &threshold1, const float &threshold2, const bool &case_sensitive);
		bool read(const char* inputFileName);
		//void exactMatch(string& required_name);
		vector< pair<float,int> > LDPMap_Query(vector<string>& req_vect, int& match_num,vector< vector<float> >& scores);//lcsMatch
		vector< pair<float,int> > arbitrary_order_match(vector<string>& req_vect, int& match_num, vector< vector<float> >& scores);
		vector< pair<float,int> > continuous_match(vector<char>& required_name_vect, int& match_num, vector< vector<float> >& continuousWordSimilarityMatrix);
		void match(string& required_name, int& match_num);
		
};	

#endif
