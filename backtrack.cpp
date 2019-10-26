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
	heapsort((int)cards.size());

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
	int ret = 0, maxProfit = 0;
	maxProfit = greedy_two(maxSize);



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
