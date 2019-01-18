#ifndef PASSENGER_H
#define PASSENGER_H
#include<iostream>
using namespace std;
struct passenger {
	passenger();
	passenger(int num, int wf, int gf, int wt, int tt);//构造函数
	int ID;//关键码
	int waitFloor;//等待层数
	int goalFloor;//目标层数
	int waitTime;//起始等待时间
	int TolerateTime;//容忍时间
	passenger *next;//形成链式队列的指针
};
//无参构造函数
passenger::passenger() {
	ID = 0;
	waitFloor = 0;
	goalFloor = 0;
	waitTime = 0;
	TolerateTime = 0;
	next = NULL;
}
//含参构造函数
passenger::passenger(int num, int wf, int gf, int wt, int tt) {
	ID = num;
	waitFloor = wf;
	goalFloor = gf;
	waitTime = wt;
	TolerateTime = tt;
	next = NULL;
}
//乘客对列
class line {
public:
	line();//构造函数
	~line();//析构函数
	void Insert(passenger *p);//插入乘客进入等待队列
	void Remove(int time, int *btn);//删除无法容忍而离开的乘客
	int getCall(int &time, int &tag);//获得队列中最早按下按钮的乘客信息（所在楼层数和起始等待时间）
	passenger* pop(int stime, int timearea, int wf);//弹出即将进入电梯的乘客
	bool IsEmpty();//判断等待队列是否为空
	bool maxWait(int nowtime);//电梯“空闲等待时”是否有乘客响应
private:
	passenger *waitfloor[10];//waitfloor[0]不使用,waitfloor[1]~[9]为1~9层的乘客队列头指针;
};
//构造函数
line::line() {
	for (int i = 0; i < 10; i++)
	{
		waitfloor[i] = NULL;
	}
}
//析构函数
line::~line()
{
	passenger *tmp;
	for (int i = 0; i < 10; i++)
	{
		while (waitfloor[i] != NULL) {
			tmp = waitfloor[i];
			waitfloor[i] = waitfloor[i]->next;
			delete tmp;
		}
	}
}
//
void line::Insert(passenger *p)
{//将乘客按时间顺序插入到所在层的等待队列中
	int i = p->waitFloor; passenger *tmp = waitfloor[i];
	//队列为空
	if (tmp == NULL) { waitfloor[i] = p; return; }
	//插入点为首元位置
	if (tmp->waitTime > p->waitTime) { p->next = waitfloor[i]->next; waitfloor[i] = p; return; }

	while (tmp->next != NULL && p->waitTime > tmp->next->waitTime)tmp = tmp->next;
	//插入点不在队尾
	if (tmp->next != NULL) {
		p->next = tmp->next;
		tmp->next = p;
		return;
	}//插入点在队尾
	else {
		tmp->next = p;
	}
}
void line::Remove(int time, int *btn)//?离开的乘客一定按下了按钮
{//删除所有楼层中在time之前超出容忍时间的乘客
	passenger *tmp;
	for (int i = 1; i < 10; i++)
	{
		while (waitfloor[i] != NULL && (waitfloor[i]->waitTime + waitfloor[i]->TolerateTime) <= time) {
			tmp = waitfloor[i];
			waitfloor[i] = waitfloor[i]->next;
			cout << "ID:" << tmp->ID << "的乘客已离开" << tmp->waitFloor << "层" << endl;
			btn[i] = 1;
			delete tmp;
		}
	}
}
int line::getCall(int & time, int & tag)
{//在所有队列中找起始等待时间最小的乘客
	int t = 100000000, f = 0, gf = 0;
	passenger *tmp;
	for (int i = 1; i < 10; i++)
	{
		tmp = waitfloor[i];
		while (tmp != NULL)
		{
			if (tmp->waitTime < t) {
				t = tmp->waitTime;
				gf = tmp->goalFloor;
				f = i;
			}
			tmp = tmp->next;
		}
	}
	time = t;
	//预期判断：假设当电梯在f层时
	if (gf < f)tag = -1;//电梯关门后下行
	if (gf > f)tag = 1;//电梯关门后上行
	return f;
}
//
passenger* line::pop(int stime, int timearea, int wf)
{//将在电梯开启时间段（stime~timearea）仍在等待的乘客弹出等待队列，准备进入电梯
	passenger *tmp;
	if (waitfloor[wf] != NULL && (stime + timearea) >= waitfloor[wf]->waitTime &&stime <= (waitfloor[wf]->waitTime + waitfloor[wf]->TolerateTime - timearea)) {
		tmp = waitfloor[wf];
		waitfloor[wf] = waitfloor[wf]->next;
		tmp->next = NULL;
		return tmp;
	}
	return NULL;
}
//判断所有队列是否为空
bool line::IsEmpty()
{
	for (int i = 1; i < 10; i++)
	{
		if (waitfloor[i] != NULL)return false;
	}
	return true;
}
//空闲等待时扫描是否有乘客响应
bool line::maxWait(int nowtime)
{
	nowtime += 300;
	for (int i = 1; i < 10; i++)
	{
		passenger *tmp = waitfloor[i];
		while (tmp != NULL) {
			if (tmp->waitTime < nowtime)return false;
			tmp = tmp->next;
		}
	}
	return false;
}
#endif // !PASSENGER_H
