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
	int neight[8];
	int neiNum;
	int lineSum;
	int id;
};

Station station[300];
int main(int argc, char* argv[])
{
	int line = 0;
	int id = 0;
	int mark = 0;
	string s;
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
		cout << j << " "<< station[j].getLine(0) << station[j].getName() << station[j].getNeiNum()<< " ";
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
			int end_id = -1, start_id = -1;
			int queue[300] = { -1 };
			if (argv[1][1] == 'b') {
				//最短路程
				string start(argv[2]);
				string end(argv[3]);
				int per[300];
				int mark[300] = { 0 };
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
				if (start_id == end_id && start_id!=-1) {
					cout << "1" << endl;
					cout << station[start_id].getName() << endl;
				}
				else {
					while (last >= top) {
						top++;
						int id_head = 0;
						if (top >= 1) {
							id_head = queue[top - 1];
						}
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
						if (last>=0 && queue[last] == end_id)
							break;
					}
					if (top <= last) {
						int top_result = -1;
						int result[100] = {-1};
						int per_line = -1;
						int now_line = -1;
						while (end_id != -1) {
							result[++top_result] = end_id;
							if (end_id == start_id)
								break;
							end_id = per[end_id];
						}
						cout << top_result + 1 << endl;
						if (top_result != -1) {
							for (int j1 = 0; j1 < station[result[top_result]].getLineSum(); j1++) {
								for (int j2 = 0; j2 < station[result[top_result - 1]].getLineSum(); j2++) {
									if (station[result[top_result]].getLine(j1) == station[result[top_result - 1]].getLine(j2))
										now_line = station[result[top_result]].getLine(j1);
								}
							}
						}
						if (top_result >= 2 && ((result[top_result] == 21 && result[top_result - 1] == 22) || (result[top_result] == 22 && result[top_result - 1] == 21))) {
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

			}

			else if (argv[1][1] == 'c') {
				//向两边延伸，标记当前线路所有节点，再从换乘车站向两边延伸
				string start(argv[2]);
				string end(argv[3]);
				int queue_exchange[100];
				int exchange_sum = 0;
				int top_exchange = -1;
				int last_exchange = -1;
				int top = -1;
				int last = -1;
				//int mark[300] = { 0 };
				int per[300][3];
				int now_line;
				int mark_line[20] = { 0 };
				for (int i = 0; i < id; i++) {
					if (station[i].getLineSum() > 1)
						exchange_sum++;
				}
				//0 per,1 distance ,2 exchangetimes
				for (int i = 0; i < 300; i++) {
					per[i][0] = -1;
					per[i][1] = 999;
					per[i][2] = 999;
				}
				for (int i = 0; i < id; i++) {
					if (station[i].getName() == start) {
						start_id = i;
					}
					else if (station[i].getName() == end) {
						end_id = i;
					}
				}
				if (start_id == end_id && start_id != -1) {
					cout << 1 << endl;
					cout << station[start_id].getName() << endl;
				}
				else if(start_id!=-1){
					queue[++top] = start_id;
					last++;
					per[start_id][1] = 0;
					per[start_id][2] = 0;
					//mark[start_id]++;
					if (station[start_id].getLineSum() > 1) {
						queue_exchange[++top_exchange] = start_id;
						last_exchange++;
					}
					while (top_exchange <= last_exchange) {
						int head_id;
						if (top_exchange == -1) {
							head_id = start_id;
						}
						else {
							top_exchange++;
							head_id = queue_exchange[top_exchange - 1];
						}
						//cout << head_id<<" " << station[head_id].getName() << " ";
						queue[top] = head_id;
						last = top;
						//cout << station[head_id].getName() << endl;
						
						for (int i = 0; i < station[head_id].getLineSum(); i++) {
							now_line = station[head_id].getLine(i);
							if (mark_line[now_line] == 0) {
								//cout << now_line << " " << lineName[now_line] << endl;
								int j;
								for (j = 0; j <= head[now_line][0]; j++) {
									if (head[now_line][j] == head_id)
										break;
								}
								int distance = 0;
								for (int k = j - 1; k >= 1; k--, distance++) {
									//if (now_line == 4) {
										//cout << per[head[now_line][j]][2] << "and"<< per[head[now_line][k]][2] << endl;
									//}
									if (per[head[now_line][j]][1] + distance < per[head[now_line][k]][1] && per[head[now_line][j]][2] <= per[head[now_line][k]][2]) {
										if (station[head[now_line][k]].getLineSum() > 1 && per[head[now_line][k]][0] == -1) {
											queue_exchange[++last_exchange] = head[now_line][k];
											per[head[now_line][k]][2] = per[head[now_line][j]][2]+1;
											if (top_exchange == -1)
												top_exchange++;
										}
										per[head[now_line][k]][0] = head[now_line][k + 1];
										per[head[now_line][k]][1] = per[head[now_line][j]][1] + distance;
										if(per[head[now_line][k]][2] == 999)
											per[head[now_line][k]][2] = per[head[now_line][j]][2];
									}
								}
								distance = 0;
								for (int k = j + 1; k <= head[now_line][0]; k++, distance++) {
									//if (now_line == 4) {
										//cout << "" << endl;
									//}
									if (per[head[now_line][j]][1] + distance < per[head[now_line][k]][1] && per[head[now_line][j]][2] <= per[head[now_line][k]][2]) {
										if (station[head[now_line][k]].getLineSum() > 1 && per[head[now_line][k]][0] == -1) {
											queue_exchange[++last_exchange] = head[now_line][k];
											per[head[now_line][k]][2] = per[head[now_line][j]][2] + 1;
											if (top_exchange == -1)
												top_exchange++;
										}
										per[head[now_line][k]][0] = head[now_line][k - 1];
										per[head[now_line][k]][1] = per[head[now_line][j]][1] + distance;
										if (per[head[now_line][k]][2] == 999)
											per[head[now_line][k]][2] = per[head[now_line][j]][2];
									}
								}
							}
								
							/*	while (top <= last) {
									top++;
									int head_id_2 = queue[top - 1];
									for (int j = 0; j < station[head_id_2].getNeiNum(); j++) {
										if (station[station[head_id_2].getNei(j)].inLine(now_line) && (per[head_id_2][1] + 1) < per[station[head_id_2].getNei(j)][1]) {
											if(now_line == 4)
												cout << station[head_id_2].getName() << " "<<station[station[head_id_2].getNei(j)].getName() << endl;
											if (station[station[head_id_2].getNei(j)].getLineSum() > 1 && per[station[head_id_2].getNei(j)][0] == -1) {
												queue_exchange[++last_exchange] = station[head_id_2].getNei(j);
												if (top_exchange == -1) {
													top_exchange++;
												}
											}
											per[station[head_id_2].getNei(j)][1] = per[head_id_2][1] + 1;
											per[station[head_id_2].getNei(j)][0] = head_id_2;
											queue[++last] = station[head_id_2].getNei(j);
										}
										else if (station[station[head_id_2].getNei(j)].getLineSum() > 1 && per[station[head_id_2].getNei(j)][1]!=-1) {
											if (top >= 1) {
												top--;
											}
										}
									}
								}*/
							
							mark_line[now_line] = 1;
						}
					}


					int top_result = -1;
					int result[100] = {-1};
					int per_line = -1;
					now_line = -1;
					while (end_id>=0) {
						result[++top_result] = end_id;
						if (end_id == start_id)
							break;
						end_id = per[end_id][0];
					}
					cout << top_result + 1 << endl;
					for (int j1 = 0; j1 < station[result[top_result]].getLineSum(); j1++) {
						for (int j2 = 0; j2 < station[result[top_result - 1]].getLineSum(); j2++) {
							if (station[result[top_result]].getLine(j1) == station[result[top_result - 1]].getLine(j2))
								now_line = station[result[top_result]].getLine(j1);
						}
					}
					if (top_result >= 2 && ((result[top_result] == 21 && result[top_result - 1] == 22) || (result[top_result] == 22 && result[top_result - 1] == 21))) {
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


