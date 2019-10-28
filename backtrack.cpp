#include <stddef.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <chrono>

using namespace std;

//cards.first -> profit and weight pair
//cards.second -> profit/weight
vector<pair<pair<int,int>, double>> cards;

int greedy_one(int maxSize);
int greedy_two(int maxSize);
int backtracking(int maxSize);
void track(int maxSize, int &maxProfit, int node, int tNode, bool addNode, vector<pair<pair<int,int>, int>> &tracking);
int kwf(int maxSize);
void minheapify(int i, int n);
void buildminheap(int n);
void heapsort(int n);

int main(int argc, char**argv){

	if(argc != 4){
		cout << "Run as ./submission <input> <output> <0/1/2>.\n";
		return 1;
	}

	string in = argv[1], out = argv[2];
	int type = atoi(argv[3]);
	if(type < 0 || type > 2){
		cout << "Run with values 0, 1, or 2 only.\n";
		return 1;
	}

	if(!ifstream(in)){
		cout << "The given input file does not exist.\n";
		return 1;
	}
	if(!ofstream(out)){
		cout << "The given output file could not be opened.\n";
		return 1;
	}

	ifstream input(in);
	ofstream output(out);

	int item = 0, cap = 0, maxProfit = 0;
	while(!input.eof()){
		if(input.eof())
			break;
		input >> item >> cap;
		for(int i = 0; i < item; i++){
			pair<int,int> add(-1,-1);
			pair<pair<int,int>, double> add2(add,0.0);
			cards.push_back(add2);
		}
		for(int j = 0; j < item; j++){
			input >> cards[j].first.second >> cards[j].first.first;
			cards[j].second = (double)cards[j].first.first/cards[j].first.second;
		}

		auto start = chrono::high_resolution_clock::now();
		heapsort((int)cards.size());
		if(type == 0){
			maxProfit = greedy_one(cap);
		}
		else if(type == 1){
			maxProfit = greedy_two(cap);
		}
		else{
			maxProfit = backtracking(cap);
		}

		auto stop = chrono::high_resolution_clock::now();

		chrono::duration<double> elapsed = stop - start;
		output << item << " " << maxProfit << " " << elapsed.count() << "\n";
		cards.clear();
	}

	return 0;
}

int greedy_one(int maxSize){
	int ret = 0, size = maxSize;

	for(int i = 0; i < (int)cards.size(); i++){
		int w = cards[i].first.second;
		if(size-w >= 0){
		int p = cards[i].first.first;
		ret += p;
		size -=w;
		}
		if(size == 0) break;
	}

	return ret;
}

int greedy_two(int maxSize){
	int ret = 0, greedyMax = 0, pMax = 0;
	greedyMax = greedy_one(maxSize);

	for(int i = 0; i < (int)cards.size(); i++){
		int w = cards[i].first.second;
		int p = cards[i].first.first;
		if(maxSize-w >= 0 && p > pMax){
			pMax = p;
		}
	}

	if(pMax > greedyMax)
		ret = pMax;
	else
		ret = greedyMax;

	return ret;
}



int backtracking(int maxSize){
	int maxProfit = greedy_two(maxSize);
	int tSize = cards.size()*cards.size();

	//tracking.first -> profit and weight
	//tracking.second -> bound
	vector<pair<pair<int,int>, int>> tracking(tSize);
	tracking[0].first.first = 0;
	tracking[0].first.second = 0;
	tracking[0].second = kwf(maxSize);

	track(maxSize,maxProfit,0,0,true,tracking); //left side of the backtracking tree
	track(maxSize,maxProfit,0,0,false,tracking); //right side of the backtracking tree
	return maxProfit;
}

/*
 * maxSize -> maximum weight
 * maxProfit -> used to hold the found maximum profit, no change if greedy_two is max
 * node -> index number of the card to add to the total profit
 * tNode -> index number of the tracking array
 * addNode -> whether the node is added or not (left child is true, right child is false)
 * tracking -> array that holds the backtracking tree
 */

void track(int maxSize, int &maxProfit, int node, int tNode, bool addNode, vector<pair<pair<int,int>, int>> &tracking){
	if(node < cards.size()){
		int nextNode = 0;
		if(addNode){
			nextNode = tNode*2 + 1;
			tracking[nextNode].first.first = tracking[tNode].first.first + cards[node].first.first;
			tracking[nextNode].first.second = tracking[tNode].first.second + cards[node].first.second;
			tracking[nextNode].second = tracking[tNode].second;
			if(tracking[nextNode].first.second <= maxSize && tracking[nextNode].first.first > maxProfit)
				maxProfit = tracking[nextNode].first.first; //new optimal solution
		}
		else{
			nextNode = tNode*2 + 2;
			tracking[nextNode].first.first = tracking[tNode].first.first;
			tracking[nextNode].first.second = tracking[tNode].first.second;
		}

		//if the node is promising, create children
		if(tracking[nextNode].first.second < maxSize && tracking[nextNode].second > maxProfit){
			track(maxSize,maxProfit,node+1,nextNode,true,tracking); //left side of the backtracking tree
			track(maxSize,maxProfit,node+1,nextNode,false,tracking); //right side of the backtracking tree
		}
	}


}

int kwf(int maxSize){
	int ret = 0, size = maxSize;

	for(int i = 0; i < (int)cards.size(); i++){
		int w = cards[i].first.second;
		if(size-w >= 0){
		int p = cards[i].first.first;
		ret += p;
		size -=w;
		}
		else{
			int p = (cards[i].first.first*size)/w;
			ret += p;
			size = 0;
		}
		if(size == 0) break;
	}

	return ret;
}

void minheapify(int i, int n){
	int smallest = -1;
	int left = 2*i+1;
	int right = 2*i+2;
	if(left<n && cards[left].second < cards[i].second)
		smallest = left;
	else
		smallest = i;
	if(right<n && cards[right].second < cards[smallest].second)
		smallest = right;
	if(smallest != i){
		int f = cards[i].first.first, s = cards[i].first.second;
		double sec = cards[i].second;
		cards[i].first.first = cards[smallest].first.first;
		cards[i].first.second = cards[smallest].first.second;
		cards[i].second = cards[smallest].second;
		cards[smallest].first.first = f;
		cards[smallest].first.second = s;
		cards[smallest].second = sec;
		minheapify(smallest,n);
	}
}

void buildminheap(int n){
	for(int i = n/2; i >= 0; i--){
		minheapify(i,n);
	}
}

void heapsort(int n){
	buildminheap(n);
	for(int i = n; i > 0; i--){
		int f = cards[0].first.first, s = cards[0].first.second;
		double sec = cards[0].second;
		cards[0].first.first = cards[i-1].first.first;
		cards[0].first.second = cards[i-1].first.second;
		cards[0].second = cards[i-1].second;
		cards[i-1].first.first = f;
		cards[i-1].first.second = s;
		cards[i-1].second = sec;
		minheapify(0,i-1);
	}
}
