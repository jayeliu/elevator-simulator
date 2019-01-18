#ifndef FUNCTION_H
#define FUNCTION_H
#include"elevator.h"
#include<fstream>
const double t = 0.1;
int standardTime = 0;
//从文件中读入数据
bool input(line &arr) {
	passenger *tmp;
	int num = 1, wf = 0, gf = 0, wt = 0, tt = 0;
	ifstream file("data.txt");
	if (!file.is_open()) { cout << "无法打开数据文件！" << endl; return false; }
	while (file.peek() != EOF) {
		file >> wf >> gf >> wt >> tt;
		tmp = new passenger(num, wf, gf, wt, tt);
		if (tmp->waitFloor == 0)break;
		arr.Insert(tmp);
		num++;
	}
	return true;
}
//电梯运行主函数
void run(line &arr, evelator eve) {
	int *btn = eve.getBtn(), floor = 0, f = 0, tag = 0, time;//计数器;
	passenger *p;
	while (!arr.IsEmpty() || !eve.IsEmpty()) {//队列非空或电梯非空
		arr.Remove(standardTime, btn);
		floor = eve.getCurrentFloor();
		if (eve.noCall() && eve.IsEmpty()) {//无乘客响应；电梯为空
			tag = 0;
			if (arr.maxWait(standardTime)) {
				eve.setCall(1);
				if (floor != 1)tag = -1;
				eve.cancelCall(floor);//
				standardTime += 300;
				cout << "时刻：" << standardTime << endl;
				cout << "电梯在" << floor << "层处于空闲状态，向第1层驶去" << endl;
			}
			else if (!arr.maxWait(standardTime)) {
				f = arr.getCall(time, tag);
				if (time > standardTime)standardTime = time;
				eve.setCall(f);
				if (f > floor)tag = 1;
				if (f < floor)tag = -1;
				if (floor != f)eve.cancelCall(floor);//
				cout << "----电梯" << floor << "层等待状态时，得到其他乘客响应----" << endl;
			}
			continue;
		}
		else {
			f = eve.getNextMove(tag);
			eve.setCall(f);
			if (f > floor) {//上行
				tag = 1;
				eve.cancelCall(floor);
				arr.Remove(standardTime, btn);
				eve.cancelCall(floor);
				cout << "时刻：" << standardTime << endl;
				cout << "电梯" << floor << "层,上行" << endl;
				standardTime += 51;
				eve.setCurrentFloor(++floor);
			}
			else if (f < floor) {//下行
				tag = -1;
				eve.cancelCall(floor);
				arr.Remove(standardTime, btn);
				eve.cancelCall(floor);
				cout << "时刻：" << standardTime << endl;
				cout << "电梯" << floor << "层,下行" << endl;
				standardTime += 61;
				eve.setCurrentFloor(--floor);
			}
			else {//开门
				//上行过程中开门
				if (tag == 1) {
					cout << "时刻：" << standardTime << endl;
					cout << "电梯到达" << floor << "层，开始减速" << endl;
					standardTime += 14;
					cout << "时刻：" << standardTime << endl;
					cout << "电梯" << floor << "层,正在开门" << endl;
				}//下行过程中开门
				if (tag == -1) {
					cout << "时刻：" << standardTime << endl;
					cout << "电梯到达" << floor << "层，开始减速" << endl;
					standardTime += 23;
					cout << "时刻：" << standardTime << endl;
					cout << "电梯" << floor << "层,正在开门" << endl;
				}
				standardTime += 20;
				cout << "时刻：" << standardTime << endl;
				cout << "电梯" << floor << "层,已开门" << endl;
				//出电梯
				while (eve.Remove(floor)) {
					standardTime += 25;
				}
				arr.Remove(standardTime, btn);
				//电梯有空间
				while (!eve.IsFull()) {
					p = arr.pop(standardTime, 40 + 20, floor);
					//在完全关门前有人进入
					if (p == NULL) { standardTime += 40; break; }
					eve.Insert(p);
					standardTime = (p->waitTime < standardTime) ? (standardTime + 25) : (p->waitTime + 25);
				}
				cout << "时刻：" << standardTime << endl;
				cout << "电梯" << floor << "层,开始关门" << endl;
				standardTime += 20;
				cout << "时刻：" << standardTime << endl;
				cout << "电梯" << floor << "层,已关门" << endl;
				eve.cancelCall(floor);
			}
			continue;
		}
	}
}
#endif // !FUNCTION_H
