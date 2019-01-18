#ifndef FUNCTION_H
#define FUNCTION_H
#include"elevator.h"
#include<fstream>
const double t = 0.1;
int standardTime = 0;
//���ļ��ж�������
bool input(line &arr) {
	passenger *tmp;
	int num = 1, wf = 0, gf = 0, wt = 0, tt = 0;
	ifstream file("data.txt");
	if (!file.is_open()) { cout << "�޷��������ļ���" << endl; return false; }
	while (file.peek() != EOF) {
		file >> wf >> gf >> wt >> tt;
		tmp = new passenger(num, wf, gf, wt, tt);
		if (tmp->waitFloor == 0)break;
		arr.Insert(tmp);
		num++;
	}
	return true;
}
//��������������
void run(line &arr, evelator eve) {
	int *btn = eve.getBtn(), floor = 0, f = 0, tag = 0, time;//������;
	passenger *p;
	while (!arr.IsEmpty() || !eve.IsEmpty()) {//���зǿջ���ݷǿ�
		arr.Remove(standardTime, btn);
		floor = eve.getCurrentFloor();
		if (eve.noCall() && eve.IsEmpty()) {//�޳˿���Ӧ������Ϊ��
			tag = 0;
			if (arr.maxWait(standardTime)) {
				eve.setCall(1);
				if (floor != 1)tag = -1;
				eve.cancelCall(floor);//
				standardTime += 300;
				cout << "ʱ�̣�" << standardTime << endl;
				cout << "������" << floor << "�㴦�ڿ���״̬�����1��ʻȥ" << endl;
			}
			else if (!arr.maxWait(standardTime)) {
				f = arr.getCall(time, tag);
				if (time > standardTime)standardTime = time;
				eve.setCall(f);
				if (f > floor)tag = 1;
				if (f < floor)tag = -1;
				if (floor != f)eve.cancelCall(floor);//
				cout << "----����" << floor << "��ȴ�״̬ʱ���õ������˿���Ӧ----" << endl;
			}
			continue;
		}
		else {
			f = eve.getNextMove(tag);
			eve.setCall(f);
			if (f > floor) {//����
				tag = 1;
				eve.cancelCall(floor);
				arr.Remove(standardTime, btn);
				eve.cancelCall(floor);
				cout << "ʱ�̣�" << standardTime << endl;
				cout << "����" << floor << "��,����" << endl;
				standardTime += 51;
				eve.setCurrentFloor(++floor);
			}
			else if (f < floor) {//����
				tag = -1;
				eve.cancelCall(floor);
				arr.Remove(standardTime, btn);
				eve.cancelCall(floor);
				cout << "ʱ�̣�" << standardTime << endl;
				cout << "����" << floor << "��,����" << endl;
				standardTime += 61;
				eve.setCurrentFloor(--floor);
			}
			else {//����
				//���й����п���
				if (tag == 1) {
					cout << "ʱ�̣�" << standardTime << endl;
					cout << "���ݵ���" << floor << "�㣬��ʼ����" << endl;
					standardTime += 14;
					cout << "ʱ�̣�" << standardTime << endl;
					cout << "����" << floor << "��,���ڿ���" << endl;
				}//���й����п���
				if (tag == -1) {
					cout << "ʱ�̣�" << standardTime << endl;
					cout << "���ݵ���" << floor << "�㣬��ʼ����" << endl;
					standardTime += 23;
					cout << "ʱ�̣�" << standardTime << endl;
					cout << "����" << floor << "��,���ڿ���" << endl;
				}
				standardTime += 20;
				cout << "ʱ�̣�" << standardTime << endl;
				cout << "����" << floor << "��,�ѿ���" << endl;
				//������
				while (eve.Remove(floor)) {
					standardTime += 25;
				}
				arr.Remove(standardTime, btn);
				//�����пռ�
				while (!eve.IsFull()) {
					p = arr.pop(standardTime, 40 + 20, floor);
					//����ȫ����ǰ���˽���
					if (p == NULL) { standardTime += 40; break; }
					eve.Insert(p);
					standardTime = (p->waitTime < standardTime) ? (standardTime + 25) : (p->waitTime + 25);
				}
				cout << "ʱ�̣�" << standardTime << endl;
				cout << "����" << floor << "��,��ʼ����" << endl;
				standardTime += 20;
				cout << "ʱ�̣�" << standardTime << endl;
				cout << "����" << floor << "��,�ѹ���" << endl;
				eve.cancelCall(floor);
			}
			continue;
		}
	}
}
#endif // !FUNCTION_H
