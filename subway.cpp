// subway.cpp : 定义控制台应用程序的入口点。
//

#include <iostream>
#include "stdafx.h"
#include <fstream>
#include <string.h>
#include <string>
using namespace std;

class Station
{
public:
	Station(string n, int l,int i);
	Station();
	~Station();
	void appendNeight(int s);
	void appendLine(int l);
	string getName();
	int getNeiNum();
	int getNei(int i);
	int getLine(int i);
	int getLineSum();
	bool inLine(int l);

private:
	string name;
	int lineNum[3];
	int neight[10];
	int neiNum;
	int lineSum;
	int id;
};

int main(int argc, char* argv[])
{
	int line = 0;
	int id = 0;
	int mark = 0;
	string s;
	Station station[500];
	int head[20][50];
	int headmark = 0;
	string lineName[20];
	int i_line = 1;
	int per_id;
	int i;
	ifstream file("beijing-subway.txt",ios::in);
	if (file.is_open()) {
		while (!file.eof()) {
			file >> s;
			if (mark == 1) {
				mark = 2;
				headmark = 1;
				lineName[line++] = s;
				head[line - 1][0] = 0;
			}
			else if (s == "Line") {
				mark = 1;
				i_line = 1;
			}
			else if (mark == 2) {
				for (i = 0; i < id; i++) {
					if (s == station[i].getName()) {
						station[i].appendLine(line-1);
						break;
					}
				}
				head[line - 1][i_line++] = i;
				head[line - 1][0]++;
				if (headmark == 1) {
					headmark = 0;
				}
				if (i == id) {
					station[id] = Station(s, line-1, id);
					id++;
					if (id > 1) {
						int sum = station[per_id].getLineSum();
						for (int j = 0; j < sum; j++) {
							if (station[per_id].getLine(j)+1 == line) {
								station[id - 1].appendNeight(per_id);
								station[per_id].appendNeight(id - 1);
								break;
							}
						}
					}
				}
				//连续多个换乘会出错，不是上一个
				else {
					int sum = station[per_id].getLineSum();
					for (int j = 0; j < sum; j++) {
						if (station[per_id].getLine(j)+1 == line) {
							station[i].appendNeight(per_id);
							station[per_id].appendNeight(i);
							break;
						}
					}
					station[i].appendLine(line - 1);
				}
				per_id = i;
			}
		}
	}
	file.close();
	
	/*
	for (int j = 0; j < id; j++) {
		cout << j << " "<< station[j].getLineSum() << station[j].getName() << station[j].getNeiNum()<< " ";
		for (int k = 0; k < station[j].getNeiNum(); k++) {
			cout << station[j].getNei(k) << " ";
		}
		cout << "\n";
	}
	*/
	/*
	for (int j = 0; j < line; j++) {
		cout << station[head[j]].getName() << endl;
	}*/
	if (argc > 0) {
		if (argv[1][0] == '-') {
			if (argv[1][1] == 'b') {
				//最短路程
				string start(argv[2]);
				string end(argv[3]);
				int end_id, start_id;
				int queue[500];
				int per[500];
				int mark[500] = { 0 };
				int top = -1;
				int last = -1;
				int distance = 0;
				for (int i = 0; i < id; i++) {
					if (station[i].getName() == start) {
						queue[++top] = i;
						last++;
						start_id = i;
						mark[i] = 1;
						break;
					}
				}
				for (int i = 0; i < id; i++) {
					if (station[i].getName() == end) {
						end_id = i;
						break;
					}
				}
				//cout << start_id << " " << end_id << endl;
				if (start_id == end_id) {
					cout << "1" << endl;
					cout << station[start_id].getName() << endl;
				}
				while (last>=top) {
					top++;
					int id_head = queue[top - 1];
					//cout << station[id_head].getNeiNum() << endl;
					for (int k = 0; k < station[id_head].getNeiNum(); k++) {
						int l = station[id_head].getNei(k);
						if (mark[l] != 1) {
							mark[l] = 1;
							queue[++last] = l;
							per[l] = id_head;
						}
						if (l == end_id) {
							break;
						}
					}
					if (queue[last] == end_id)
						break;
				}
				if (top <= last) {
					int top_result = -1;
					int result[300];
					int per_line = -1;
					int now_line = -1;
					while (1) {
						result[++top_result] = end_id;
						if (end_id == start_id)
							break;
						end_id = per[end_id];
					}
					cout << top_result+1 << endl;
					for (int j1 = 0; j1 < station[result[top_result]].getLineSum(); j1++) {
						for (int j2 = 0; j2 < station[result[top_result - 1]].getLineSum(); j2++) {
							if (station[result[top_result]].getLine(j1) == station[result[top_result - 1]].getLine(j2))
								now_line = station[result[top_result]].getLine(j1);
						}
					}
					if (top_result >= 2 && ((result[top_result] == 21 && result[top_result-1] == 22) || (result[top_result] == 22 && result[top_result-1] == 21))) {
						now_line = station[result[top_result - 2]].getLine(0);
					}
					for (; top_result >= 0; top_result--) {
						int j_line;
						for (j_line = 0; j_line < station[result[top_result]].getLineSum(); j_line++) {
							if (per_line == station[result[top_result]].getLine(j_line))
								break;
						}
						if (j_line == station[result[top_result]].getLineSum() && per_line != -1) {
							if (station[result[top_result]].getLineSum() == 1) {
								now_line = station[result[top_result]].getLine(0);
								cout << "换乘" << lineName[station[result[top_result]].getLine(0)] << endl;
							}
							else {
								for (int j1 = 0; j1 < station[result[top_result]].getLineSum(); j1++) {
									for (int j2 = 0; j2 < station[result[top_result + 1]].getLineSum(); j2++) {
										if (station[result[top_result]].getLine(j1) == station[result[top_result + 1]].getLine(j2))
											now_line = station[result[top_result]].getLine(j1);
									}
								}
								cout << "换乘" << lineName[now_line] << endl;
							}
						}
						else if (j_line != station[result[top_result]].getLineSum()) {
							cout << "\n";
						}
						cout << station[result[top_result]].getName();
						per_line = now_line;
					}
				}
			}

			else if (argv[1][1] == 'c') {
				string start(argv[2]);
				string end(argv[3]);
				int end_id, start_id;
				int exchange[100];
				int sum_exchange = 0;
				int top = 0;
				int last = -1;
				int mark_line[30] = { 0 };
				int per[100][10];
				//0 distance ,1 sum
				for (int i = 0; i < 100; i++) {
					per[i][0] = 999;
					per[i][1] = 0;
				}
				int queue[100];
				for (int i = 0; i < id; i++) {
					if (station[i].getName() == start) {
						start_id = i;
					}
					else if (station[i].getName() == end) {
						end_id = i;
					}
					if (station[i].getLineSum() > 1) {
						exchange[sum_exchange++] = i;
					}
				}
				for (int i = 0; i < station[start_id].getLineSum(); i++) {
					for (int j = 0; j < sum_exchange; j++) {
						if (station[exchange[j]].inLine(station[start_id].getLine(i)) && mark_line[station[start_id].getLine(i)]!=1) {
							mark_line[station[start_id].getLine(i)] = 1;
							queue[++last] = j;
							per[j][0] = 0;
						}
					}
				}
				while (top <= last) {
					top++;
					int id_head = queue[top - 1];
					for (int i = 0; i < station[id_head].getLineSum(); i++) {
						for (int j = 0; j < sum_exchange; j++) {
							if (station[exchange[j]].inLine(station[start_id].getLine(i)) ) {
								mark_line[station[start_id].getLine(i)] = 1;
								queue[++last] = j;
								if ((per[id_head][0] + 1) < per[j][0]) {
									per[j][per[j][1] + 2] = id_head;
									per[j][i]++;
								}
							}
						}
					}
					int line_mark;
					for (line_mark = 0; line_mark < station[end_id].getLineSum(); line_mark++) {
						if (mark_line[station[end_id].getLine(line_mark)] == 1)
							break;
					}
					if (line_mark != station[end_id].getLineSum()) {
						break;
					}
				}

			}
		}
	}



	while (1) {
		cin >> s;
		for (int j = 0; j < 20; j++) {
			if (s == lineName[j]) {
				for (int k = 1; k <= head[j][0]; k++)
					cout << station[head[j][k]].getName() << endl;
			}
		}
	}
    return 0;
}


Station::Station(string n, int l,int i)
{
	name = n;
	lineNum[0] = l;
	neiNum = 0;
	lineSum = 1;
	id = i;
}

Station::Station() {

}

Station::~Station()
{
}

string Station::getName() {
	return name;
}

int Station::getNeiNum() {
	return neiNum;
}

void Station::appendNeight(int s)
{
	neight[neiNum++] = s;
}

void Station::appendLine(int l)
{
	if(lineSum>0&& l != lineNum[lineSum - 1])
		lineNum[lineSum++] = l;
	else if(lineSum == 0)
		lineNum[lineSum++] = l;
}

int Station::getNei(int i) {
	return neight[i];
}

int Station::getLine(int i) {
	return lineNum[i];
}

int Station::getLineSum() {
	return lineSum;
}

bool Station::inLine(int l) {
	for (int i = 0; i < lineSum; i++) {
		if (lineNum[i] == l)
			return true;
	}
	return false;
}


