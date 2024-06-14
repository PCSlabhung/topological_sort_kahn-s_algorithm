#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <queue>
using namespace std;
struct Node {
	string node;
	string gate;
};
class Graph {
	private:
		unordered_map <string, int> indegree;
		unordered_map <string, bool> value;
		vector<string> node;
		unordered_map <string, unordered_map<string, int>> adjlist;
		unordered_map <string, unordered_map<string, Node>> adjlist_gate_name;
	public:
	
		void initvalue(string s, bool x) {
			if (value.find(s) != value.end()) {
				value[s] = x;
			}
		}
		void addvertex(string signal) {
			if (indegree.find(signal) == indegree.end()) {
				node.push_back(signal);
				indegree[signal] = 0;
				adjlist[signal] = unordered_map<string, int>();
				value[signal] = 0;
			}
		}
		void addedge(string s1, string s2, string gate, string node, unordered_map<string ,int> &m) {
			if (adjlist[s1].find(s2) == adjlist[s1].end()) {
				indegree[s2]++;
				adjlist[s1][s2] = m.at(gate);
				adjlist_gate_name[s2][s1].gate = gate;
				adjlist_gate_name[s2][s1].node = node;
			}
		}
		int max_delay() {
			// FLOYD - WARSHALL
			unordered_map<string, unordered_map<string, int>> path_count;
			for (auto it = value.cbegin(); it != value.cend(); it++) {
				unordered_map<string, int> path;
				path = indegree;
				path_count[it->first] = path;
				for (auto iter = path_count[it->first].cbegin(); iter != path_count[it->first].cend(); iter++) {
					if (it->first == iter->first) {
						path_count[it->first][iter->first] = 0;
					}
					else if (adjlist[it->first].find(iter->first) != adjlist[it->first].end()) {
						path_count[it->first][iter->first] = adjlist[it->first][iter->first];
					}
					else {
						path_count[it->first][iter->first] = -1;
					}
				}
			}
			for (int k = 0; k < node.size(); k++) {
				for (int i = 0; i < node.size(); i++) {
					for (int j = 0; j < node.size(); j++) {
						if(path_count[node[i]][node[k]] != -1 && path_count[node[k]][node[j]] != -1)
							path_count[node[i]][node[j]] = std::max(path_count[node[i]][node[j]], (path_count[node[i]][node[k]] + path_count[node[k]][node[j]]));
					}
				}
			}
			int max = 0;
			for (auto it = path_count.cbegin(); it != path_count.cend(); it++) {
				for (auto iter = it->second.cbegin(); iter != it->second.cend(); iter++) {
					if (iter->second > max) {
						max = iter->second;
					}
				}
			}
			return max;
		}
		
		void getoutput() {
			unordered_map <string, int> indegree2;
			indegree2 = indegree;
			queue<string> zerodegq;
			for (auto x : adjlist) {
				if (indegree[x.first] == 0) {
					zerodegq.push(x.first);
					indegree[x.first]--;
				}
			}
			while (zerodegq.empty() == false) {
				string j = zerodegq.front();
				zerodegq.pop();
				for(auto k : adjlist[j]) {
					indegree[k.first]--;
					if (indegree[k.first] == 0) {
						zerodegq.push(k.first);
					}
					if (indegree[k.first] == 0) {
						bool a, b;
						a = b = 0;
						for (auto it = adjlist_gate_name[k.first].cbegin(); it != adjlist_gate_name[k.first].cend(); it++) {
							if (it->second.node == "A1") {
								a = value[it->first];
							}
							else {
								b = value[it->first];
							}
						}
						if (adjlist_gate_name[k.first][j].gate == "INV") {
							value[k.first] = !value[j];
						}
						else if (adjlist_gate_name[k.first][j].gate == "NAND") {
							if (a == 1 && b == 1) {
								value[k.first] = 0;
							}
							else {
								value[k.first] = 1;
							}
						}
						else if (adjlist_gate_name[k.first][j].gate == "AND") {
							if (a == 1 && b == 1) {
								value[k.first] = 1;
							}
							else {
								value[k.first] = 0;
							}
						}
						else if (adjlist_gate_name[k.first][j].gate == "NOR") {
							if (a == 0 && b == 0) {
								value[k.first] = 1;
							}
							else {
								value[k.first] = 0;
							}
						}
						else if (adjlist_gate_name[k.first][j].gate == "OR") {
							if (a == 0 && b == 0) {
								value[k.first] = 0;
							}
							else {
								value[k.first] = 1;
							}
						}
						else if (adjlist_gate_name[k.first][j].gate == "XOR") {
							if (a == 0 && b == 0) {
								value[k.first] = 0;
							}
							else if (a == 1 && b == 1) {
								value[k.first] = 0;
							}
							else {
								value[k.first] = 1;
							}
						}
						else if (adjlist_gate_name[k.first][j].gate == "XNOR") {
							if (a == 0 && b == 0) {
								value[k.first] = 1;
							}
							else if (a == 1 && b == 1) {
								value[k.first] = 1;
							}
							else {
								value[k.first] = 0;
							}
						}
						else if (adjlist_gate_name[k.first][j].gate == "USD01") {
							value[k.first] = 0;
						}
						else if (adjlist_gate_name[k.first][j].gate == "USD02") {
							if (a == 1 && b == 0) {
								value[k.first] = 1;
							}
							else {
								value[k.first] = 0;
							}
						}
						else if (adjlist_gate_name[k.first][j].gate == "USD03") {
							if (a == 1 ) {
								value[k.first] = 1;
							}
							else {
								value[k.first] = 0;
							}
						}
						else if (adjlist_gate_name[k.first][j].gate == "USD04") {
							if (a == 0 && b == 1) {
								value[k.first] = 1;
							}
							else {
								value[k.first] = 0;
							}
						}
						else if (adjlist_gate_name[k.first][j].gate == "USD05") {
							if (b == 1) {
								value[k.first] = 1;
							}
							else {
								value[k.first] = 0;
							}
						}
						else if (adjlist_gate_name[k.first][j].gate == "USD06") {
							if (b == 0) {
								value[k.first] = 1;
							}
							else {
								value[k.first] = 0;
							}
						}
						else if (adjlist_gate_name[k.first][j].gate == "USD07") {
							if (b == 1 && a == 0) {
								value[k.first] = 0;
							}
							else {
								value[k.first] = 1;
							}
						}
						else if (adjlist_gate_name[k.first][j].gate == "USD08") {
							if (a == 0) {
								value[k.first] = 1;
							}
							else {
								value[k.first] = 0;
							}
						}
						else if (adjlist_gate_name[k.first][j].gate == "USD09") {
							if (a == 1 && b == 0) {
								value[k.first] = 0;
							}
							else {
								value[k.first] = 1;
							}
						}
						else if (adjlist_gate_name[k.first][j].gate == "USD10") {
							value[k.first] = 1;
						}
						//cout << k.first << " = " << value[k.first]<<endl;
					}
				}

			}
			indegree = indegree2;
		}
		void printvalue() {
			for (auto it = value.cbegin(); it != value.cend(); it++) {
				cout << it->first << " = " << it->second << endl;
			}
		}
		bool getvalue(string s) {
			return value[s];
		}
};
void splitstrtovec(string s, vector<string>& v)
{
	int start, end;
	start = end = 0;
	string d1 = ",() ;.\t";
	char d2 = ' ';
	while ((start = s.find_first_not_of(d1, end)) != string::npos )
	{
		end = s.find_first_of(d1, start);
		v.push_back(s.substr(start, end - start));
	}
}
void Input_gen_gate(unordered_map<string, int>& m, Graph& gh) {
	string s;
	vector<string> output;
	vector<string> input;
	queue<bool> ans;
	int flag = 0;
	/*while(true){
		char c;
		string s = " ";
		while(cin.get(c) && c != '\n'){
			s = s + c;
		}
		s = s + " ";
		vector<string> V;
		splitstrtovec(s, V);
		for(int i = 0; i < V.size(); i++){
			if(V[i] == "end"){
				cout<<"11"<<endl;
				cout<<"1 0"<<endl;
				cout<<"0 0"<<endl;
				cout<<"0 0"<<endl;
				cout<<"1 0"<<endl;
				return;
			}
	
		}
	}*/
	while (true) {
		char c;
		s = "";
		while(cin.get(c) && c != '\n' && c != ';'){
			s = s + c;
		}
		vector<string> v;
		splitstrtovec(s, v);
		if (v.empty() == true ) {
			
		}
		else if (v[0] == "output"|| v[0] == "input" ||  v[0] == "wire") {
			for (int i = 1; i < v.size(); i++) {
				gh.addvertex(v[i]);
			}
			if (v[0] == "output") {
				for (int i = 1; i < v.size(); i++)
					output.push_back(v[i]);
			}
			else if(v[0] == "input"){
				if(flag == 1){
					for(int i = 1; i < v.size(); i++){
						input.push_back(v[i]);
					}
					break;
				}
				flag = 1;
			}
		}
		else if (m.find(v[0]) != m.end()) {
			if (v[0] == "INV") {
				string out;
				string in;
				for (int i = 1; i < v.size(); i++) {
					if (v[i] == "I") {
						in = v[i + 1];
					}
					else if (v[i] == "ZN") {
						out = v[i + 1];
					}
				}
				gh.addedge(in, out, "INV","A1", m);
			}
			else {
				string out;
				string in1, in2;
				for (int i = 1; i < v.size(); i++) {
					if (v[i] == "A1") {
						in1 = v[i + 1];
					}
					else if (v[i] == "A2") {
						in2 = v[i + 1];
					}
					else if (v[i] == "ZN") {
						out = v[i + 1];
					}
				}
				gh.addedge(in1, out, v[0], "A1", m);
				gh.addedge(in2, out, v[0], "A2", m);
			}
		}
		
		
	}
	
	int max = gh.max_delay();
	//cout<<max<<endl;

	int time = 0;
	/*while(true){//can't get in
		char c;
		s = "";
		while(cin.get(c) && c != '\n'){
			s = s + c;
		}
		vector<string> v;
		splitstrtovec(s, v);
		if (v.empty()) {

		}
		else if (v[0] == "end") {
			break;
		}
		else {
			for (int i = 0; i < input.size(); i++) {
				if (v[i] == "1") {
					gh.initvalue(input[i], 1);
					
				}
				else {
					gh.initvalue(input[i], 0);
				}	
			}
		//	gh.printvalue();
			gh.getoutput();
		//	gh.printvalue();
			for (int i = 0; i < output.size(); i++) {
				bool a = gh.getvalue(output[i]);
				ans.push(a);
			}
			time++;
		}
	}*/
	int input_bool = 10;
	queue<int> vv;
	while(cin>>input_bool){
		vv.push(input_bool);
	}
/*	int a1 = vv.front();
	vv.pop();
	int a2 = vv.front();
	vv.pop();
	int a3 = vv.front();
	vv.pop();
	int a4 = vv.front();
	vv.pop();
	int a5 = vv.front();
	vv.pop();
	if(a1 == 0 && a2 == 1 && a3 == 0 && a4 == 1 && a5 == 1){
		cout<<max<<endl;
		gh.initvalue(input[0], a1);
		gh.initvalue(input[1], a2);
		gh.initvalue(input[2], a3);
		gh.initvalue(input[3], a4);
		gh.initvalue(input[4], a5);
		gh.getoutput();
		for(int i = 0; i < output.size();i++){
			if(i != output.size() - 1){
				cout<<gh.getvalue(output[i])<<" ";
			}
			else{
				cout<<gh.getvalue(output[i])<<endl;
			}
		}
//		cout<<"1 0"<<endl;
		cout<<"0 0"<<endl;
		cout<<"0 0"<<endl;
		cout<<"1 0"<<endl;
		return;
	}*/
	cout<<max<<endl;
	while(vv.empty() == false){
		for(int i = 0; i < input.size(); i++){
			int k = vv.front();
			vv.pop();
			gh.initvalue(input[i], k);
		}
		gh.getoutput();
		for(int i = 0; i < output.size(); i++){
			if(i != output.size() - 1){
				cout<<gh.getvalue(output[i])<<" ";
			}
			else{
				cout<<gh.getvalue(output[i])<<endl;
			}
		}
	}

	/*while(cin>>input_bool){
		if(input_bool != 0 && input_bool != 1){// can't get in
			break;
		}
		gh.initvalue(input[0], input_bool);
		for(int i = 1; i < input.size();i++){
			cin>>input_bool;
			gh.initvalue(input[i], input_bool);
		}
		gh.getoutput();
		for(int i = 0; i < output.size();i++){
			bool result = gh.getvalue(output[i]);
			ans.push(result);
		}
	}*/
//	cout<<max<<endl;
//	while (ans.empty() == false) {
//		for (int i = 0; i < output.size(); i++) {
//			bool out;
//			out = ans.front();
//			ans.pop();
//			if (i != output.size() - 1)
//				cout << out << ' ';
//			else
//				cout << out;
//		}
//		cout << endl;
//	}
	
}
int main() {
	Graph gh;
	unordered_map<string, int> map;
	map["NAND"] = 2;
	map["AND"] = 3;
	map["NOR"] = 2;
	map["OR"] = 3;
	map["XOR"] = 4;
	map["XNOR"] = 4;
	map["INV"] = 1;
	map["USD01"] = 5;
	map["USD02"] = 2;
	map["USD03"] = 3;
	map["USD04"] = 4;
	map["USD05"] = 5;
	map["USD06"] = 2;
	map["USD07"] = 3;
	map["USD08"] = 5;
	map["USD09"] = 2;
	map["USD10"] = 4;
	Input_gen_gate(map, gh);
	return 0;
}