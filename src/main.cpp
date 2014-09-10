#include "Dictionary.h"
#include "layeredDP.h"

#include <string>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

#define DL ";"

using namespace std;



static void usage() {
	cout << "\nUsage:\n"
		"	UMLSLDP [-h] [-c] [-t1 threshold1] [-t2 threshold2] filename\n"
		"Description:\n"
		"	-h		Print the help message.\n"
		"	-c		Case sensitive.\n"
		"	threshold1	The default is 0.85\n"
		"	threshold2	The default is 0.35\n"
		"	filename	The source file\n"
		<< endl;
}

int main(int argc, char* argv[]) {
	string required_name;
	int match_num;
	string input_str;
	
	char* inputFileName;
	float threshold1 = 0.85;//set the default threshold value to 0.85
	float threshold2 = 0.35;//set the default threshold value to 0.35
	bool case_sensitive = false;
	if(argc==1){
		usage();
		return 1;
	}
	int input_para_counter = 1;
	while(input_para_counter < argc){
		if(strcmp("-h", argv[input_para_counter]) == 0){
			usage();
			return 1;
		}
		if(strcmp("-c", argv[input_para_counter]) == 0){
			input_para_counter++;
			case_sensitive = true;
		}else if(strcmp("-t1", argv[input_para_counter]) == 0){
			input_para_counter++;
			threshold1 = ::atof(argv[input_para_counter++]);
		}else if(strcmp("-t2", argv[input_para_counter]) == 0){
			input_para_counter++;
			threshold2 = ::atof(argv[input_para_counter++]);
		}else{
			inputFileName = argv[input_para_counter++];
		}
	}

	Dictionary dict(threshold1, threshold2, case_sensitive);
	if(!dict.read(inputFileName)){
		usage();
		return 1;
	}else{
		cout<<"----------------------------------UMLSLDP-------------------------------------"<<endl;
	}
	while(true){
		cout<<"\nQuery Name:\n"<<endl;
		getline(cin,required_name);
		cout<<"\n\nTop Match Number:\n"<<endl;
		getline(cin,input_str);
		istringstream (input_str) >>match_num;
		if(required_name.empty()||match_num<=0){
			cout<<"\nPlease make sure the name is not empty or match number is larger than 0!\n"<<endl;
		}else{
			dict.match(required_name,match_num);
		}
	}
	return 0;
}

