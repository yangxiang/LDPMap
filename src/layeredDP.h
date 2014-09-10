#ifndef _layeredDP_H
#define _layeredDP_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include <cmath>
#include <cstring>
#include <cstdlib>

using namespace std;

struct scoreVert{
	float score;
	int vertex_query;
	int vertex_meta;
};

class layeredDP{
	private:
		//static void erase_row_column(vector< vector<float> >& word_similarity_matrix, const int& query_index, const int& meta_index);
		static bool score_vertexes_sortfunction(const scoreVert& sv1, const scoreVert& sv2);
		static bool is_letter(const char &x);
	public:
		layeredDP();
		static vector<char> convert(const string& x);
		static float continuous_match_WordSimilarity(vector<char> x,vector<char> y);
		//static float getMav(vector<string>req,vector<string>entryName);
		static float WordSimilarity_caseSensitive(const vector<char>& x_vect, const vector<char>& y_vect, vector <vector <float> >& c, const int& leng);
		static float WordSimilarity_caseInsensitive(const vector<char>& x_vect, const vector<char>& y_vect, vector <vector <float> >& c, const int& leng);
		static float ConceptSimilarityScore(const vector<string>& req,vector< pair<string,int> >& name,vector< vector<float> >& scores, vector< vector<float> >& s, const int& leng);
		static float arb_order_score(const vector<string>& req_vect,const vector< pair<string,int> >& name, vector< vector<float> >&scores);
};


#endif