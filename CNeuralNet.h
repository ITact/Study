#ifndef CSshenjingNET_H
#define CSshenjingNET_H

#include <vector>
#include <math.h>

#include "utils.h"
#include "CParams.h"

using namespace std;



//-------------------------------------------------------------------
//	������ϸ���ṹ
//-------------------------------------------------------------------
struct Sshenjing
{
	//���뵽��ϸ��������
	int				      m_NumInputs;

	//ÿ�������Ȩ��
	vector<double>	m_vecWeight;

	Sshenjing(int NumInputs);
};


//---------------------------------------------------------------------
//	��ϸ���� �ṹ
//---------------------------------------------------------------------

struct SshenjingLayer
{
	//���ϸ������
	int					      m_NumSshenjing;

	//��������ϸ��
	vector<Sshenjing>		m_vecSshenjing;

	SshenjingLayer(int NumSshenjing, 
				       int NumInputsPerShenjing); //�ڶ���������һ����Ԫ������������Ŀ
};


//----------------------------------------------------------------------
//	��������
//----------------------------------------------------------------------

class CSshenjingNet
{
	
private:
	
	int m_NumInputs; //�������

	int m_NumOutputs; //�������

	int	m_NumHiddenLayers;  //���ز��������������㶼�����ز�

	int m_SshenjingPerHiddenLyr; //ÿ�����ز��������Ԫ����

	//�洢ÿһ�����ϸ������������㣬����������������
	vector<SshenjingLayer>	m_vecLayers;

public:

	CSshenjingNet();

	void CreateNet(); //������������

	//���������ж�ȡȨ��
	vector<double>	GetQuanzhong()const;

	//����������Ȩ�ص�����
	int				      GetNumberOfWeights()const;

	//���ĵ�Ȩ���滻ԭ�е�Ȩ�� Ϊʲô��
	void			      PutQuanzhong(vector<double> &weights);

	//��������������
	vector<double>	Update(vector<double> &inputs);

	//�߼�˹������
	inline double	  Sigmoid(double activation, double response);

	vector<int>     CalculateSplitPoints() const;
};
			

#endif