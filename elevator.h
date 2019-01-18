#ifndef ELEVATOR_H
#define ELEVATOR_H
#include<cmath>
#include"passenger.h"
//
class evelator {
public:
	evelator(int sz = 10);//���캯��
	~evelator();//��������
	int getCurrentSize() { return currentSize; }//��õ�ǰ����������
	int getMaxSize() { return maxSize; }//�����������
	int getCurrentFloor() { return currentFloor; }//��õ�ǰ����
	int getNextMove(int tag);//important�����ʻ����������Ķ���¥��
	int *getBtn() { return button; }//��õ��ݸ��㰴ť��״̬
	bool setCall(int f) { button[f] = 1; return true; }//����ĳ��ĵ�����Ӧ
	bool setCurrentFloor(int f) { currentFloor = f; return true; }//���ĵ��ݵ�ǰ����¥����
	bool cancelCall(int f) { button[f] = 0; return true; }//ȡ��ĳ��ĵ�����Ӧ
	bool IsFull();//�жϵ����Ƿ�����
	bool IsEmpty();//�����Ƿ����
	bool noCall();//�Ƿ��޳˿���Ӧ
	bool Insert(passenger *p);//�˿ͽ������
	bool Remove(int floor);//�˿ʹӵ����뿪
private:
	int currentFloor;//��ǰ¥��
	int maxSize;//���ݵ�����
	int currentSize;//��ǰ���ݵ�����
	int button[10];//0:����Ӧ1:����Ӧ
	passenger *eveFloor[10];//eveFloor[0]��ʹ�ã�Ŀ��¥��Ϊi,�˿Ͳ����eveFloor[i]������
};
//���캯����������ʼ״̬Ϊ�ȴ�״̬������¥����Ϊ1��
evelator::evelator(int sz) :maxSize(sz), currentSize(0), currentFloor(1) {
	for (int i = 0; i < 10; i++)
	{
		eveFloor[i] = NULL;
		button[i] = 0;
	}
}
//��������
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
//���뺯��
bool evelator::Insert(passenger * p)
{
	if (currentSize == maxSize) { delete p; return false; }
	cout << "�˿�" << p->ID << "�������";
	int f = p->goalFloor;
	passenger *tmp = eveFloor[f];
	if (tmp == NULL)eveFloor[f] = p;//�ӿ�
	else {//�ӷǿ�
		while (tmp->next != NULL)tmp = tmp->next;
		tmp->next = p;
	}
	p->next = NULL;
	currentSize++;
	return true;
}
//ɾ������
bool evelator::Remove(int floor)
{
	passenger *tmp;
	if (eveFloor[floor] == NULL) {
		return false;
	}
	else {
		tmp = eveFloor[floor];
		eveFloor[floor] = eveFloor[floor]->next;
		cout << "�˿�" << tmp->ID << "�뿪����" << endl;
		delete tmp;
		currentSize--;
		return true;
	}
}
//�жϺ���
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
//��Ӧ�жϺ���
bool evelator::noCall()
{
	for (int i = 1; i < 10; i++)
	{
		if (button[i] != 0)return false;
	}
	return true;
}
//���ĺ���
int evelator::getNextMove(int tag)
{
	int f = getCurrentFloor();
	if (IsEmpty() && !noCall()) {//���ݿ����г˿���Ӧ
		if (tag == 1) {//����
			for (int i = f; i < 10; i++)
			{
				if (button[i] == 1)return i;
			}//�˿���Ӧ���·�¥��
			tag = -1; getNextMove(tag);//״̬�ı�
		}
		else if (tag == -1) {//����
			for (int i = f; i > 0; i--)
			{
				if (button[i] == 1) return i;
			}//�˿���Ӧ���Ϸ�¥��
			tag = 1; getNextMove(tag);//״̬�ı�
		}
	}
	if (!IsEmpty() && noCall()) {//���ݷǿ����޳˿���Ӧ
		if (tag == 1) {//����
			for (int i = f; i < 10; i++)
			{
				if (eveFloor[i] != NULL)return i;
			}//�˿�Ŀ��¥�����·�
			tag = -1; getNextMove(tag);//״̬�ı�
		}
		else if (tag == -1) {//����
			for (int i = f; i > 0; i--)
			{
				if (eveFloor[i] != NULL) return i;
			}//�˿�Ŀ��¥�����·�
			tag = 1; getNextMove(tag);//״̬�ı�
		}
	}
	if (!IsEmpty() && !noCall()) {//���ݷǿ����г˿���Ӧ
		if (tag == 1) {//����
			for (int i = f; i < 10; i++)
			{
				if (eveFloor[i] != NULL || button[i] == 1)return i;
			}//Ŀ��¥���˿���Ӧ���·�
			tag = -1; getNextMove(tag);//״̬�ı�
		}
		if (tag == -1) {//����
			for (int i = f; i > 0; i--)
			{
				if (eveFloor[i] != NULL || button[i] == 1)return i;
			}//Ŀ��¥���˿���Ӧ���Ϸ�
			tag = 1; getNextMove(tag);//״̬�ı�
		}
	}
}
#endif // !ELEVATOR_H
