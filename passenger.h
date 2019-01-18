#ifndef PASSENGER_H
#define PASSENGER_H
#include<iostream>
using namespace std;
struct passenger {
	passenger();
	passenger(int num, int wf, int gf, int wt, int tt);//���캯��
	int ID;//�ؼ���
	int waitFloor;//�ȴ�����
	int goalFloor;//Ŀ�����
	int waitTime;//��ʼ�ȴ�ʱ��
	int TolerateTime;//����ʱ��
	passenger *next;//�γ���ʽ���е�ָ��
};
//�޲ι��캯��
passenger::passenger() {
	ID = 0;
	waitFloor = 0;
	goalFloor = 0;
	waitTime = 0;
	TolerateTime = 0;
	next = NULL;
}
//���ι��캯��
passenger::passenger(int num, int wf, int gf, int wt, int tt) {
	ID = num;
	waitFloor = wf;
	goalFloor = gf;
	waitTime = wt;
	TolerateTime = tt;
	next = NULL;
}
//�˿Ͷ���
class line {
public:
	line();//���캯��
	~line();//��������
	void Insert(passenger *p);//����˿ͽ���ȴ�����
	void Remove(int time, int *btn);//ɾ���޷����̶��뿪�ĳ˿�
	int getCall(int &time, int &tag);//��ö��������簴�°�ť�ĳ˿���Ϣ������¥��������ʼ�ȴ�ʱ�䣩
	passenger* pop(int stime, int timearea, int wf);//��������������ݵĳ˿�
	bool IsEmpty();//�жϵȴ������Ƿ�Ϊ��
	bool maxWait(int nowtime);//���ݡ����еȴ�ʱ���Ƿ��г˿���Ӧ
private:
	passenger *waitfloor[10];//waitfloor[0]��ʹ��,waitfloor[1]~[9]Ϊ1~9��ĳ˿Ͷ���ͷָ��;
};
//���캯��
line::line() {
	for (int i = 0; i < 10; i++)
	{
		waitfloor[i] = NULL;
	}
}
//��������
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
{//���˿Ͱ�ʱ��˳����뵽���ڲ�ĵȴ�������
	int i = p->waitFloor; passenger *tmp = waitfloor[i];
	//����Ϊ��
	if (tmp == NULL) { waitfloor[i] = p; return; }
	//�����Ϊ��Ԫλ��
	if (tmp->waitTime > p->waitTime) { p->next = waitfloor[i]->next; waitfloor[i] = p; return; }

	while (tmp->next != NULL && p->waitTime > tmp->next->waitTime)tmp = tmp->next;
	//����㲻�ڶ�β
	if (tmp->next != NULL) {
		p->next = tmp->next;
		tmp->next = p;
		return;
	}//������ڶ�β
	else {
		tmp->next = p;
	}
}
void line::Remove(int time, int *btn)//?�뿪�ĳ˿�һ�������˰�ť
{//ɾ������¥������time֮ǰ��������ʱ��ĳ˿�
	passenger *tmp;
	for (int i = 1; i < 10; i++)
	{
		while (waitfloor[i] != NULL && (waitfloor[i]->waitTime + waitfloor[i]->TolerateTime) <= time) {
			tmp = waitfloor[i];
			waitfloor[i] = waitfloor[i]->next;
			cout << "ID:" << tmp->ID << "�ĳ˿����뿪" << tmp->waitFloor << "��" << endl;
			btn[i] = 1;
			delete tmp;
		}
	}
}
int line::getCall(int & time, int & tag)
{//�����ж���������ʼ�ȴ�ʱ����С�ĳ˿�
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
	//Ԥ���жϣ����赱������f��ʱ
	if (gf < f)tag = -1;//���ݹ��ź�����
	if (gf > f)tag = 1;//���ݹ��ź�����
	return f;
}
//
passenger* line::pop(int stime, int timearea, int wf)
{//���ڵ��ݿ���ʱ��Σ�stime~timearea�����ڵȴ��ĳ˿͵����ȴ����У�׼���������
	passenger *tmp;
	if (waitfloor[wf] != NULL && (stime + timearea) >= waitfloor[wf]->waitTime &&stime <= (waitfloor[wf]->waitTime + waitfloor[wf]->TolerateTime - timearea)) {
		tmp = waitfloor[wf];
		waitfloor[wf] = waitfloor[wf]->next;
		tmp->next = NULL;
		return tmp;
	}
	return NULL;
}
//�ж����ж����Ƿ�Ϊ��
bool line::IsEmpty()
{
	for (int i = 1; i < 10; i++)
	{
		if (waitfloor[i] != NULL)return false;
	}
	return true;
}
//���еȴ�ʱɨ���Ƿ��г˿���Ӧ
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
