#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <fstream>

#include <sstream>
#include <stack>
#include <list>

using namespace std;

struct element{
	char *compoundName, *compoundExp, *unknownName;
	element *next;
};

class log {
	public:
		ofstream *fout;
		ifstream *fin;
		char ***a;
		int N, M;

		stack<string> steck;
		list<char**> listCompoundNotPossible;

		stringstream *flowA;

		log(char *strFout = (char*)"out.txt", char *strFin = (char*)"in.txt"){
			fout = new ofstream(strFout);
			fin = new ifstream(strFin);

			(*fin) >> N;
			(*fin) >> M;
			fin->ignore(255, '\n');

			a = new char** [N+M];
			for(int i = 0; i < N+M; i++)
				a[i] = new char* [5];

			for(int i = 0; i < N; i++){
				a[i][0] = new char [100];
				a[i][1] = new char [1];
				(*fin) >> a[i][0];
				(*fin) >> a[i][1];
			}
			for(int i = N; i < N+M; i++){
				a[i][0] = new char [100];
				a[i][1] = new char [100];
				a[i][2] = new char [100];
				a[i][3] = new char [100];
				a[i][4] = new char [100];
				(*fin) >> a[i][0];
				fin->ignore(255, '=');
				fin->ignore(255, ' ');
				fin->getline(a[i][2], 100);
			}
		}
		~log(){
			fout->close(); // закрываем файл
			fin->close();
			for(int i = 0; i < N+M; i++){
				delete []a[i][0];
				delete []a[i][1];
				delete []a[i][2];
				delete []a[i][3];

				delete []a[i][4];

				delete []a[i];
			}
			delete []a;
		}

		void writeMas(){
			cout << N << " " << M << endl;
			for(int i = 0; i < N+M; i++){
				if(i >= N) cout << a[i][0] << " " << a[i][2] << endl;
				else cout << a[i][0] << " " << a[i][1] << endl;
			}
		}

		void VM(){
			stringstream simpleA, compoundA;
			char **copyA;

			for(int i = N; i < N+M; i++){
				compoundA << a[i][2];
				logDecision(compoundA, i, false);
				compoundA.clear();

				a[i][1][0] = stackSum();

				if(!listCompoundNotPossible.empty()){
					//for(int i = 0; i < listCompoundNotPossible[0].size(); i++){
						//copyA = listCompoundNotPossible[0].front();
						//compoundA << copyA[2];

						//cout << endl << "*****************" << *copyA[1] << "******************" << listCompoundNotPossible[0].size() << endl;
						//copyA[1] = (char*)"X";
						//listCompoundNotPossible[0].pop_front();
						//compoundA.clear();
					//}
					for(list<char**>::iterator begin = listCompoundNotPossible.begin(), end = listCompoundNotPossible.end(); begin != end; ++begin){

						char**& element = *begin;

						//cout << a[i][0] << "  " << element[3] << endl;

						//if(a[i][0] == element[3]){
						if(!strcmp(a[i][0], element[3])){
							cout  << "*****************" << a[i][0] << "  " << element[3] << endl;
							compoundA << element[2];
							stringstream ss;
							ss << element[4];
							int j;
							ss >> j;
							logDecision(compoundA, i+1, true);
							compoundA.clear();
							a[j][1][0] = stackSum();
							//cout  << "*****************" << a[i][0] << "  " << element[3] << endl;
							//listCompoundNotPossible.erase(begin);
							ss.clear();
						}
						//cout << element[3] << " " << listCompoundNotPossible.size() << endl;
					}
					//cout << endl;
				}
			}

		}


		char logDecision(stringstream &compoundA, int i, bool p){
			string rez;
			char rezInt1, rezInt2;
			string wordBuff;
				
			while(compoundA >> wordBuff){
				if(wordBuff == "&"){
					compoundA >> wordBuff;

					rezInt1 = nameSearch(steck.top(), i, p);
					rezInt2 = nameSearch(wordBuff, i, p);

					if(rezInt1 != '?' && rezInt2 != '?')
						rez = (int)rezInt1 & (int)rezInt2;
					else{
						rez = notPossible(rezInt1, '&', rezInt2);

						//////////////////////////////////
						//rez = "?";
						//////////////////////////////////
					}

					steck.pop();
					steck.push(rez);
				}else if(wordBuff == "|"){
					compoundA >> wordBuff;
					rezInt2 = nameSearch(wordBuff, i, p);
					rez = rezInt2;
					steck.push(rez);
				//}else if(wordBuff == "?"){

				}else{
					rezInt2 = nameSearch(wordBuff, i, p);
					rez = rezInt2;
					steck.push(rez);
				}
			}
		}


		char stackSum(){
			char sum = '0';
			string rez;
			while(!steck.empty()){
				rez = steck.top();
				if(rez != "?" && sum != '?'){
					sum = (int)sum | (int)*rez.c_str();
					steck.pop();
				}else{
					sum = (int)notPossible(sum, '|', rez[0]);
					steck.pop();

					//////////////////////////////////

					//////////////////////////////////
				}
			}
			return sum;
		}
		char nameSearch(string nameS, int i, bool p){
			string rez;
			char rezInt;
			char *copyNameS;
			for(int j = 0; j < i; j++){
				if(nameS == "0" || nameS == "1"){
					rez = nameS;
					rezInt = *rez.c_str();
					break;
				}else if(a[j][0] == nameS){
					rezInt = a[j][1][0];
					break;
				}else if(j == N-1){
					rezInt = '?';

					//a[i][3] = &nameS[0u];
					if(nameS != "?" && !p){
						stringstream ss;
						ss << i;
						a[i][3] = strdup(nameS.c_str());
						a[i][4] = strdup(ss.str().c_str());
						listCompoundNotPossible.push_front(a[i]);
						//listNameNotPossible.push_front(&nameS[0u])
					}
				}
			}
			return rezInt; 
		}
		char notPossible(char operand1, char log, char operand2){
			if(log == '&'){
				if(operand1 == '0' || operand2 == '0')
					return '0';
				else
					return '?';
			}else{
				if(operand1 == '1' || operand2 == '1')
					return '1';
				else
					return '?';
			}
		}
		char notPossibleCompound(){

		}

		void printA(){
			for(int i = N; i < N+M; i++)
				cout << a[i][0] << " " << a[i][1] << "  " << a[i][2] << "  ||" << a[i][3] << "  //" << a[i][4] << endl;;
		}
};


int main(){
	//system("reset");
	log *a;
	char *strFout = new char(20);
	char *strFin = new char(20);
	//cin >> strFout;
	//strFout = (char*)"hoho.txt";
	//strFin = (char*)"hoh.txt";
	//cin >> strFin;
	a = new log();

	a->writeMas();

	cout << "_______________" << endl;

	a->VM();

	cout << "_______________" << endl;

	a->printA();

	return 0;
}