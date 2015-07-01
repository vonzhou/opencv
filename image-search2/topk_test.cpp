#include <iostream>
#include <queue>
#include <vector>

using namespace std;

int main(){

	std::vector<double> test = {0.4, 0.3, 0.88, -0.66, 0.1};
	priority_queue<double> q;

	for(int i=0; i<test.size(); i++)
		q.push(test[i]);

	int k=3;
	for(int i=0;i<k;i++){
		double t = q.top();
		cout << t << endl;
		q.pop();
	}

	return 0;
}