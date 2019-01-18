#ifndef ELEVATOR_H
#define ELEVATOR_H
#include<cmath>
#include"passenger.h"
//
class evelator {
public:
	evelator(int sz = 10);//构造函数
	~evelator();//析构函数
	int getCurrentSize() { return currentSize; }//获得当前电梯内人数
	int getMaxSize() { return maxSize; }//获得最大承载量
	int getCurrentFloor() { return currentFloor; }//获得当前层数
	int getNextMove(int tag);//important获得行驶方向中最近的动作楼层
	int *getBtn() { return button; }//获得电梯各层按钮的状态
	bool setCall(int f) { button[f] = 1; return true; }//设置某层的电梯响应
	bool setCurrentFloor(int f) { currentFloor = f; return true; }//更改电梯当前所在楼层数
	bool cancelCall(int f) { button[f] = 0; return true; }//取消某层的电梯响应
	bool IsFull();//判断电梯是否满载
	bool IsEmpty();//电梯是否空载
	bool noCall();//是否无乘客响应
	bool Insert(passenger *p);//乘客进入电梯
	bool Remove(int floor);//乘客从电梯离开
private:
	int currentFloor;//当前楼层
	int maxSize;//电梯的容量
	int currentSize;//当前电梯的人数
	int button[10];//0:无响应1:有响应
	passenger *eveFloor[10];//eveFloor[0]不使用，目的楼层为i,乘客插入进eveFloor[i]队列中
};
//构造函数（电梯起始状态为等待状态，所在楼层数为1）
evelator::evelator(int sz) :maxSize(sz), currentSize(0), currentFloor(1) {
	for (int i = 0; i < 10; i++)
	{
		eveFloor[i] = NULL;
		button[i] = 0;
	}
}
//析构函数
evelator::~evelator()
{
	for (int i = 1; i < 10; i++)
	{
		passenger *tmp;
		while (eveFloor[i] != NULL) {
			tmp = eveFloor[i];
			eveFloor[i] = eveFloor[i]->next;
			delete tmp;
		}
	}
}
//插入函数
bool evelator::Insert(passenger * p)
{
	if (currentSize == maxSize) { delete p; return false; }
	cout << "乘客" << p->ID << "进入电梯";
	int f = p->goalFloor;
	passenger *tmp = eveFloor[f];
	if (tmp == NULL)eveFloor[f] = p;//队空
	else {//队非空
		while (tmp->next != NULL)tmp = tmp->next;
		tmp->next = p;
	}
	p->next = NULL;
	currentSize++;
	return true;
}
//删除函数
bool evelator::Remove(int floor)
{
	passenger *tmp;
	if (eveFloor[floor] == NULL) {
		return false;
	}
	else {
		tmp = eveFloor[floor];
		eveFloor[floor] = eveFloor[floor]->next;
		cout << "乘客" << tmp->ID << "离开电梯" << endl;
		delete tmp;
		currentSize--;
		return true;
	}
}
//判断函数
bool evelator::IsFull()
{
	if (currentSize == maxSize)return true;
	else return false;
}
//
bool evelator::IsEmpty()
{
	if (currentSize == 0)return true;
	else return false;
}
//响应判断函数
bool evelator::noCall()
{
	for (int i = 1; i < 10; i++)
	{
		if (button[i] != 0)return false;
	}
	return true;
}
//核心函数
int evelator::getNextMove(int tag)
{
	int f = getCurrentFloor();
	if (IsEmpty() && !noCall()) {//电梯空且有乘客响应
		if (tag == 1) {//上行
			for (int i = f; i < 10; i++)
			{
				if (button[i] == 1)return i;
			}//乘客响应在下方楼层
			tag = -1; getNextMove(tag);//状态改变
		}
		else if (tag == -1) {//下行
			for (int i = f; i > 0; i--)
			{
				if (button[i] == 1) return i;
			}//乘客响应在上方楼层
			tag = 1; getNextMove(tag);//状态改变
		}
	}
	if (!IsEmpty() && noCall()) {//电梯非空且无乘客响应
		if (tag == 1) {//上行
			for (int i = f; i < 10; i++)
			{
				if (eveFloor[i] != NULL)return i;
			}//乘客目的楼层在下方
			tag = -1; getNextMove(tag);//状态改变
		}
		else if (tag == -1) {//下行
			for (int i = f; i > 0; i--)
			{
				if (eveFloor[i] != NULL) return i;
			}//乘客目的楼层在下方
			tag = 1; getNextMove(tag);//状态改变
		}
	}
	if (!IsEmpty() && !noCall()) {//电梯非空且有乘客响应
		if (tag == 1) {//上行
			for (int i = f; i < 10; i++)
			{
				if (eveFloor[i] != NULL || button[i] == 1)return i;
			}//目的楼层或乘客响应在下方
			tag = -1; getNextMove(tag);//状态改变
		}
		if (tag == -1) {//下行
			for (int i = f; i > 0; i--)
			{
				if (eveFloor[i] != NULL || button[i] == 1)return i;
			}//目的楼层或乘客响应在上方
			tag = 1; getNextMove(tag);//状态改变
		}
	}
}
#endif // !ELEVATOR_H
