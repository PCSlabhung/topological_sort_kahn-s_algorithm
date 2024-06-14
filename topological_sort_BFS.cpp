#include <iostream>
#include <vector>
#include <queue>
using namespace std;
// leetcode 207 
// private function use kahn's algorithm (BFS) to check if there's cycle in graph
// public function implement algorithm and establish graph
// not only implement kahn's algorithm & topological sort
// 2024/6/14
class Solution{
	public:
	vector<int> findOrder(int numCourse, vector<vector<int>>& prerequisities){
		vector<vector<int>> CourseDict(numCourse);
		vector<int> indegree(numCourse);
		for(int i = 0 ; i < prerequisities.size();i++){
			int prev = prerequisities[i][1];
			int curr = prerequisities[i][0];
			CourseDict[prev].push_back(curr);
			indegree[curr]++;
		}
		queue<int> start;
		int dequeueNum = 0; // for cycle check
		// if there's no cycle , dequeueNum = numCourse
		for(int i = 0; i < numCourse; i++){
			if(indegree[i] == 0){
				start.push(i);
			}
		}
		vector<int> order;
		while(start.empty() != true){
			int node_index = start.front();
			order.push_back(node_index);
			dequeueNum++;
			start.pop();
			for(int i = 0; i < CourseDict[node_index].size();i++){
				int des_node = CourseDict[node_index][i];
				indegree[des_node] --;
				if(indegree[des_node] == 0){
					start.push(des_node);
				}
			}
		}
		if(dequeueNum != numCourse){
			vector<int> empty_array;
			return empty_array;
		}
		else{
			return order;
			
		}
	}
	private:
	bool canFinish(int numCourse, vector<vector<int>>& prerequisities){
		vector<vector<int>> CourseDict(numCourse);
		vector<int> indegree(numCourse);
		for(int i = 0 ; i < prerequisities.size();i++){
			int prev = prerequisities[i][1];
			int curr = prerequisities[i][0];
			CourseDict[prev].push_back(curr);
			indegree[curr]++;
		}
		
		queue<int> start;
		int dequeueNum = 0; // for cycle check
		// if there's no cycle , dequeueNum = numCourse
		for(int i = 0; i < numCourse; i++){
			if(indegree[i] == 0){
				start.push(i);
			}
		}
		
		while(start.empty() != true){
			int node_index = start.front();
			dequeueNum++;
			start.pop();
			for(int i = 0; i < CourseDict[node_index].size();i++){
				int des_node = CourseDict[node_index][i];
				indegree[des_node] --;
				if(indegree[des_node] == 0){
					start.push(des_node);
				}
			}
		}
		return dequeueNum == numCourse;
		
	}
	
};
int main(){
	Solution s1;
	vector<vector<int>> prerequire;
	prerequire = {{1,0},{2,0},{3,1},{3,2}};
	vector<int> result = s1.findOrder(4,prerequire);
	for(int i = 0 ;i < result.size();i++){
		cout<<result[i]<<" ";
	}
	return 0;
}