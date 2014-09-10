#include "Dictionary.h"
#include "layeredDP.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <algorithm>

#define DL " "

using namespace std;

Dictionary::Dictionary(const float &thd1, const float &thd2, const bool &cs){
	threshold1 = thd1;
	threshold2 = thd2;
	case_sensitive = cs;
}

Entry Dictionary::makeEntry(vector<string>& entry_vect){
	Entry e;
	string sub;
	e.CUI = entry_vect.at(0);
	e.for_cont = false;
	if(entry_vect.at(6)=="Y"){
		e.preferred = true;
	}else{
			e.preferred = false;
		}
		
	e.cont_name = layeredDP::convert(entry_vect.at(14));
	while(entry_vect.at(14).find(DL)!=string::npos){
		sub = entry_vect.at(14).substr(0, entry_vect.at(14).find(DL));//get the substring without counting in the DL
		
		//if a substring is followed by a ',', remove the ','
		if(sub.substr(sub.length()-1,1)==","){
			sub = sub.substr(0,sub.length()-1);
		}
		
		
		e.name.push_back(make_pair(sub,-1));
		entry_vect.at(14).erase(0, entry_vect.at(14).find(DL)+1);
	}
	e.name.push_back(make_pair(entry_vect.at(14),-1));
	//e.name.insert(e.name.begin(),make_pair(" ",-1));
	insert_set(e.name);
	return e;
}

void Dictionary::insert_set(const vector< pair<string,int> >& Name){
	for(unsigned int i=0;i<Name.size();i++){
		word_set.insert(Name.at(i).first);
	}
}

void Dictionary::addEntry(Entry& e){
	dict.push_back(e);
}

bool Dictionary::read(const char* inputFileName){
	vector<string> entryVec;
	string last_CUI;
	string last_name;
	ifstream fin(inputFileName);
	if(!fin.is_open()){
		cout<<"Error opening source file!"<<endl;
		return false;
	}
	int x=0;
	cout<<"\nReading..."<<endl;

	while(fin){
		string s;
		getline(fin,s);
		//line_count++;
		istringstream ss(s);
		while(ss){
			string str;
			if(!getline(ss,str,'|')) break;
			entryVec.push_back(str);
			x++;
			if(x==18){
				//if(!getline(ss,str,'|')) break;
				getline(ss,str,'|');
				x=0;
				if((entryVec.at(1).compare("ENG")==0)&&((last_CUI.compare(entryVec.at(0))!=0)||(last_name.compare(entryVec.at(14))!=0))){
					//entry_count++;
					last_CUI = entryVec.at(0);
					last_name = entryVec.at(14);
					Entry e = makeEntry(entryVec);
					addEntry(e);
				}
				entryVec.clear();
			}
		}
	}
	fin.close();
	LDPMap_Preprocessing();
	return true;
}

void Dictionary::LDPMap_Preprocessing(){
	make_word_vector();
	keep_name_leng();//keep the kept lenght of each name in dictionary into a vector of int
	//update_entry();
	set_word_index();
}

 //words are inserted into word_set and word_vector while entries are made
//after first iteration, need to give each word in entries an index in word_vector
void Dictionary::set_word_index(){
	for(unsigned int i=0;i<dict.size();i++){
		int iter;
		for(unsigned int j=0;j<dict.at(i).name.size();j++){
			iter = search_word_index(0,word_vector.size()-1,dict.at(i).name.at(j).first);
			dict.at(i).name.at(j).second = iter;
		}
	}
	
}


  //recursive binary search_word_index
 //to give each word in dictionary the index in word_vector
int Dictionary::search_word_index(int first, int last, string& word){
	int mid = (first+last)/2;
	if(word.compare(word_vector.at(mid))<0){
		return search_word_index(first,mid-1,word);
	}else if(word.compare(word_vector.at(mid))>0){
		return search_word_index(mid+1,last,word);
	}else{
		return mid;
	}
}


//keep the kept lenght of each name in dictionary into a vector of int
void Dictionary::keep_name_leng(){
	for(unsigned int i=1;i<dict.size();i++){
		kept_leng_name.push_back(name_kept_leng(dict.at(i-1).name, dict.at(i).name));
		kept_leng_contname.push_back(word_kept_leng(dict.at(i-1).cont_name,dict.at(i).cont_name));
	}
	kept_leng_name.insert(kept_leng_name.begin(),0);//treat the first name as keeping lenght of 0
	kept_leng_contname.insert(kept_leng_contname.begin(),0);
	
}


//get the kept length of each name in dictionary
int Dictionary::name_kept_leng(const vector< pair<string,int> >& pre_name, const vector< pair<string,int> >& cur_name){
	int leng = 0;
	for(unsigned int i=0;i<pre_name.size();i++){
		if(i<cur_name.size()){
			if(pre_name.at(i).first.compare(cur_name.at(i).first)!=0){
				break;
			}else{
				leng++;
			}
		}else{
			break;
		}
	}
	return leng;
}

void Dictionary::make_word_vector(){
	//cout<<"Making word_vector..."<<endl;
	//cout<<"\n-----------------------------------------------------------------------"<<endl;
	set<string>::iterator it;
	for(it=word_set.begin();it!=word_set.end();++it){
		word_vector.push_back(*it);
	}
	//convert each word in word_vector into vector of char
	for(unsigned int i=0;i<word_vector.size();i++){
		word_char_vector.push_back(layeredDP::convert(word_vector.at(i)));
	}
	
	//keep the kept length of each word into a vector of int
	for(unsigned int i=1;i<word_vector.size();i++){
		kept_leng_word.push_back(word_kept_leng(word_char_vector.at(i-1),word_char_vector.at(i)));
	}
	kept_leng_word.insert(kept_leng_word.begin(),0);//the first word is treated as keeping one vector
												   //each word starts with a char ' ', for dinamic programming 
}


//get the kept length of each word in word vector
int Dictionary::word_kept_leng(const vector<char>& pre_word, const vector<char>& cur_word){
	int leng = 0;
	for(unsigned int i=0;i<pre_word.size();i++){
		if(i<cur_word.size()){
			if(pre_word.at(i)!=cur_word.at(i)){
				break;
			}else{
				leng++;
			}
		}else{
			break;
		}
	}
	return leng;
}

void Dictionary::sort_result(vector< pair<float,int> >& match_scores){
	for(unsigned int i=match_scores.size()-1;i>0;i--){
		if(match_scores.at(i).first>match_scores.at(i-1).first){
			swap(match_scores.at(i),match_scores.at(i-1));
		}else{
			break;
		}
	}
}


void Dictionary::cut_tail(vector< pair<float,int> >& result){
	float thd = 0.1;
	for(unsigned int i=0;i<result.size();i++){
		if(result.at(i).first<thd){
			result.erase(result.begin()+i,result.end());
			break;
		}
	}
}

void Dictionary::output_result(const vector< pair<float,int> >& result, const int& match_num){
	
	int result_size = result.size();
	int op_num = match_num;
	if(result_size==0){
		cout<<"\nNo match!\n";
		return;
	}else if(result_size<match_num){
		cout<<"\n Matching number less than required!\n";
		op_num = result_size;
	}
	
	cout<<"\nThe most match:\n";
	cout<<"\n"<<dict.at(result.at(0).second).CUI<<"|";
	for(unsigned int x=0;x<dict.at(result.at(0).second).name.size();x++){
		cout<<dict.at(result.at(0).second).name.at(x).first<<" ";
	}
	cout<<" Score: "<<result.at(0).first<<endl;

	if((result.size()>1)&&(match_num>1)){
		cout<<"\n\n====================================";
		cout<<"\n\nAlternative matches:";
		cout<<"\n\n====================================";
	}
	for(int x=1;x<op_num;x++){
		cout<<"\n\n"<<dict.at(result.at(x).second).CUI<<"|";
		for(unsigned int y=0;y<dict.at(result.at(x).second).name.size();y++){
			cout<<dict.at(result.at(x).second).name.at(y).first<<" ";
		}
		//cout<<result.at(x).second<<" ,index"<<endl;
		cout<<" Score: "<<result.at(x).first<<endl;
	}
	cout<<"\n\n----------------------------------UMLSLDP-------------------------------------\n";	
}

void Dictionary::remove_duplicated_results(vector< pair<float,int> >& result){
	unsigned int dic_size = dict.size();
	//unsigned int result_size = result.size();
	vector<bool> if_exist(dic_size, false);
	for(unsigned int i = 0;i<result.size();i++){
		if(if_exist.at(result.at(i).second)==false){
			if_exist.at(result.at(i).second) = true;
		}else{
			result.erase(result.begin()+i);
			i--;
		}
	}
}

void Dictionary::match (string& required_name, int& match_num){
	vector<string> req_vect;//required name is converted into vector of string
	vector<char> req_char_vect;//each word in the required name is then converted into vector of char
	//convert requred name string into vector of string
	string sub;
	string req_name = required_name;
	while(required_name.find(DL)!=string::npos){
		sub = required_name.substr(0, required_name.find(DL));
		req_vect.push_back(sub);
		required_name.erase(0, required_name.find(DL)+1);
	}
	req_vect.push_back(required_name);
	//req_vect.insert(req_vect.begin()," ");
	unsigned int words_size = word_char_vector.size();
	unsigned int req_size = req_vect.size();
	vector< vector<float> > scores(req_size,vector<float>(words_size,0));//score vectors that keep LCS scores
//	cout<<"Making score vectors..."<<endl;
	/*
	for(unsigned int i=0;i<kept_leng_word.size();i++){
		cout<<kept_leng_word[i]<<", ";
	}
	*/
	for(unsigned int i=0;i<req_size; i++){
		req_char_vect = layeredDP::convert(req_vect.at(i));
		/*
		for(unsigned int xx=0;xx<req_char_vect.size();xx++){
			cout<<req_char_vect[xx];
		}
		cout<<req_char_vect.size()<<"rcs!"<<endl;
		*/
		vector< vector<float> > WordSimilarityMatrix (req_char_vect.size()+1, vector<float> (0,0) );
		//cout<<c.size()<<"c.size!"<<endl;
		if(case_sensitive){
			for(unsigned int j = 0; j < words_size; j++){
				scores.at(i).at(j) = layeredDP::WordSimilarity_caseSensitive(req_char_vect, word_char_vector.at(j), WordSimilarityMatrix,kept_leng_word.at(j));
			}
		}else{
			for(unsigned int j = 0; j < words_size; j++){
				scores.at(i).at(j) = layeredDP::WordSimilarity_caseInsensitive(req_char_vect, word_char_vector.at(j), WordSimilarityMatrix,kept_leng_word.at(j));
			}
		}
	}
	/*
	for(unsigned int i=0;i<req_size;i++){
		scores[i][0] = 0;
	}
	*/

	vector<string> rn1 = req_vect;
	vector< pair<float,int> > result1 = LDPMap_Query(rn1, match_num,scores);//lcsMatch

	if(result1.at(0).first>threshold1){
		cut_tail(result1);
		output_result(result1,match_num);
		for(unsigned int i=0;i<dict.size();i++){
			dict.at(i).for_cont = false;
		}
		return;
	}
	vector< pair<float,int> > result = result1;
	vector<string> rn2 = req_vect;
	vector< pair<float,int> > result2 = arbitrary_order_match(rn2, match_num,scores);
	//cout<<result2[0].first<<"  arbScore!"<<endl;
	//vector< pair<float,int> > result22 = result2;
	
	for(unsigned int i=0;i<result2.size();i++){
		result.push_back(result2.at(i));
		sort_result(result);
	}
	/*
	if(result22.at(0).first>threshold){
		remove_duplicated_results(result);
		cut_tail(result);
		output_result(result,match_num);
		return;
	}
	*/
	
	//preprossing for continuous match
	vector<char> required_name_vect = layeredDP::convert(req_name);
	vector< vector<float> > continuousWordSimilarityMatrix (required_name_vect.size()+1, vector<float> (0,0) );
	
	vector< pair<float,int> > result3 = continuous_match(required_name_vect, match_num,continuousWordSimilarityMatrix);

	for(unsigned int i=0;i<result3.size();i++){
		result.push_back(result3.at(i));
		sort_result(result);
	}
	remove_duplicated_results(result);
	cut_tail(result);
	output_result(result,match_num);
	for(unsigned int i=0;i<dict.size();i++){
		dict.at(i).for_cont = false;
	}
	return;
}

vector< pair<float,int> > Dictionary::LDPMap_Query(vector<string>& req_vect, int& match_num,vector< vector<float> >& scores){
	cout<<"\nLDPMap_Query..."<<endl;
//	cout<<"Searching..."<<endl;
	vector< pair<float,int> >most_matches(match_num, make_pair(0,0));
	vector< vector<float> > s (req_vect.size()+1, vector<float> (0,0));
	for(unsigned int i=0;i<dict.size();i++){
		float match_score = layeredDP::ConceptSimilarityScore(req_vect,dict.at(i).name,scores,s,kept_leng_name.at(i));
		if(match_score>threshold2){
			dict.at(i).for_cont = true;
		}
		if(match_score>most_matches.at(match_num-1).first){
			most_matches.at(match_num-1).first = match_score;
			most_matches.at(match_num-1).second = i;
			if(match_num>1){
				sort_result(most_matches);
			}
		}
	}
	return most_matches;
}

vector< pair<float,int> > Dictionary::arbitrary_order_match(vector<string>& req_vect, int& match_num,vector< vector<float> >& scores){
	cout<<"\nArbitrary_Order_Match..."<<endl;	
//	cout<<"Searching..."<<endl;
	vector< pair<float,int> >most_matches(match_num, make_pair(0,0));
	for(unsigned int i=0;i<dict.size();i++){
		float arb_match_score = layeredDP::arb_order_score(req_vect,dict.at(i).name,scores);
		if(arb_match_score>most_matches.at(match_num-1).first){
			most_matches.at(match_num-1).first = arb_match_score;
			most_matches.at(match_num-1).second = i;
			if(match_num>1){
				sort_result(most_matches);
			}
		}
	}
	return most_matches;
}

//operated only on concepts having scores > t2
vector< pair<float,int> > Dictionary::continuous_match(vector<char>& required_name_vect, int& match_num, vector< vector<float> >& continuousWordSimilarityMatrix){
	cout<<"continuousMatch..."<<endl;
	//vector<char> required_name_vect = layeredDP::convert(required_name);
	//vector< vector<float> > WordSimilarityMatrix (required_name_vect.size(), vector<float> (1,0) );
	float score;
	vector< pair<float,int> >most_matches(match_num, make_pair(0,0));
	if(case_sensitive){
		for(unsigned int i=0;i<dict.size();i++){
			if(dict.at(i).for_cont==true){
				int start = 0;
				score = layeredDP::WordSimilarity_caseSensitive(required_name_vect, dict.at(i).cont_name,continuousWordSimilarityMatrix,start);
				if(score>most_matches.at(match_num-1).first){
					most_matches.at(match_num-1).first = score;
					most_matches.at(match_num-1).second = i;
					if(match_num>1){
						sort_result(most_matches);
					}
				}
			}
		}
	}else{
		for(unsigned int i=0;i<dict.size();i++){
			if(dict.at(i).for_cont==true){
				int start = 0;
				score = layeredDP::WordSimilarity_caseInsensitive(required_name_vect, dict.at(i).cont_name,continuousWordSimilarityMatrix,start);
				if(score>most_matches.at(match_num-1).first){
					most_matches.at(match_num-1).first = score;
					most_matches.at(match_num-1).second = i;
					if(match_num>1){
						sort_result(most_matches);
					}
				}
			}
		}
	}
	return most_matches;
}


