#include "layeredDP.h"
#include "Dictionary.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <algorithm> 

using namespace std;

layeredDP::layeredDP(){}



float layeredDP::arb_order_score(const vector<string>& req_vect,const vector< pair<string,int> >& name, vector< vector<float> >&scores){
	
	//Using maximal matching
	vector<bool> ifused_query(req_vect.size(),false);//if a word in query term is used for matching, its corresponding bool value is true, and it won't be
													//used for other matches later
	vector<bool> ifused_meta(name.size(),false);
	vector<scoreVert> score_vertexes;//scoreVert is defined as a structure in layeredDP.h 
	for(size_t i=0;i<req_vect.size();i++){
		for(size_t j=0;j<name.size();j++){
			scoreVert sv;
			//indicating the vertexes and edge weights in a graph
			//vertexes are the indexes in the requested querey term and in the name from metathesaurus
			sv.score = scores[i][name[j].second];
			sv.vertex_query = i;
			sv.vertex_meta = j;
			
			score_vertexes.push_back(sv);
		}
	}
	sort(score_vertexes.begin(),score_vertexes.end(),score_vertexes_sortfunction);//sort the vector into a decreasing order
	size_t query_size = req_vect.size();
	size_t meta_size = name.size();
	float arb_order_score = 0.0;
	for(size_t i=0;i<score_vertexes.size();i++){
		if((query_size==0)||(meta_size==0)){
			break;
		}//if all words in query term or in name from metathesaurus have been used, break
		if((!ifused_query[score_vertexes[i].vertex_query])&&(!ifused_meta[score_vertexes[i].vertex_meta])){
			arb_order_score += score_vertexes[i].score;
			ifused_query[score_vertexes[i].vertex_query] = true;
			ifused_meta[score_vertexes[i].vertex_meta] = true;
			query_size--;
			meta_size--;
		}//if one of the two vertexes is used, continue to the next
	}
	arb_order_score = arb_order_score*2.0/((float)(req_vect.size()+name.size()));
	return arb_order_score;
}

bool layeredDP::score_vertexes_sortfunction(const scoreVert& sv1, const scoreVert& sv2){
	return (sv1.score>sv2.score);
}

float layeredDP::ConceptSimilarityScore(const vector<string>& req, vector< pair<string,int> >& name, vector< vector<float> >& scores, vector< vector<float> >& s, const int& leng){
	unsigned int req_size = req.size();
	unsigned int name_size = name.size();
	float match_value;
	
	for(unsigned int i=0;i<s.size();i++){
		s.at(i).resize(name_size+1,0);
	}
	
	for(unsigned int i=1;i<req_size+1;i++){
		for(unsigned int j=leng+1;j<name_size+1;j++){
			float temp_score = scores.at(i-1).at(name.at(j-1).second);
			if((s.at(i-1).at(j-1)+temp_score)>max(s.at(i).at(j-1),s.at(i-1).at(j))){
				s.at(i).at(j)=s.at(i-1).at(j-1)+temp_score;
			}else{
				s.at(i).at(j)=max(s.at(i).at(j-1),s.at(i-1).at(j));
			}
		}
	}
	match_value = s.at(req_size).at(name_size)*2/(float)(req_size+name_size);
	return match_value;
}

vector<char> layeredDP::convert(const string& x){
	vector<char> v(x.begin(), x.end());
	//v.insert(v.begin(),' ');
	return v;
}

float layeredDP::WordSimilarity_caseSensitive(const vector<char>& x_vect, const vector<char>& y_vect, vector <vector <float> >& c, const int& leng){
	unsigned int x_size = x_vect.size();
	unsigned int y_size = y_vect.size();

	for(unsigned int i=0;i<c.size();i++){
		c[i].resize(y_size+1,0);
	}
	
	for(unsigned int i=1;i<x_size+1;i++){
		for(unsigned int j=leng+1;j<y_size+1;j++){
			if(x_vect.at(i-1)==y_vect.at(j-1)){
				c.at(i).at(j) = c.at(i-1).at(j-1)+1;
			}else{
				c.at(i).at(j) = max(c.at(i-1).at(j),c.at(i).at(j-1));
			}
		}
	}

	float word_score = c.at(x_size).at(y_size)/((float)(x_size+y_size)/2);
	return word_score;
}


float layeredDP::WordSimilarity_caseInsensitive(const vector<char>& x_vect, const vector<char>& y_vect, vector <vector <float> >& c, const int& leng){
	unsigned int x_size = x_vect.size();
	unsigned int y_size = y_vect.size();

	for(unsigned int i=0;i<c.size();i++){
		c[i].resize(y_size+1,0);
	}
	
	for(unsigned int i=1;i<x_size+1;i++){
		for(unsigned int j=leng+1;j<y_size+1;j++){
			if(x_vect.at(i-1)==y_vect.at(j-1) || (is_letter(x_vect[i-1]) && is_letter(y_vect[j-1]) && abs(x_vect[i-1] - y_vect[j-1]) == 32)){
				c.at(i).at(j) = c.at(i-1).at(j-1)+1;
			}else{
				c.at(i).at(j) = max(c.at(i-1).at(j),c.at(i).at(j-1));
			}
		}
	}

	float word_score = c.at(x_size).at(y_size)/((float)(x_size+y_size)/2);
	return word_score;
}

bool layeredDP::is_letter(const char &x){
	return ((x <= 122 && x >= 97)||(x <= 90 && x >= 65));
}

