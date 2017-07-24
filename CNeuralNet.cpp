#include "CNeuralNet.h"

Sshenjing::Sshenjing(int NumInputs): m_NumInputs(NumInputs+1)
											
{
	//��Ҫ����һ��ƫ����
	for (int i=0; i<NumInputs+1; ++i)
	{
		//������Ȩ������Ϊ���ֵ������������Ż�
		m_vecWeight.push_back(RandomClamped());
	}
}

//�������
SshenjingLayer::SshenjingLayer(int NumSshenjing, 
                           int NumInputsPerNeuron):	m_NumSshenjing(NumSshenjing)
{
	for (int i=0; i<NumSshenjing; ++i)

		m_vecSshenjing.push_back(Sshenjing(NumInputsPerNeuron)); //m_vecSshenjing�����NumSshenjing����Ԫ��ÿ����Ԫ����NumInputsPerNeuron������
}

//��ʼ�������磬Ĭ��ֵ��param.ini����
CSshenjingNet::CSshenjingNet() 
{
	//�Ӳ����б��л�ȡ��������
	m_NumInputs = CParams::iNumInputs;
	m_NumOutputs = CParams::iNumOutputs;
	m_NumHiddenLayers =	CParams::iNumHidden;
	m_SshenjingPerHiddenLyr = CParams::iSshenjingPerHiddenLayer;

	CreateNet();
}

//����������㣬wΪ-1��1
void CSshenjingNet::CreateNet()
{
	//���������ÿһ����
	if (m_NumHiddenLayers > 0)
	{
		//������һ�����ز�
	  m_vecLayers.push_back(SshenjingLayer(m_SshenjingPerHiddenLyr, m_NumInputs));
    
    for (int i=0; i<m_NumHiddenLayers-1; ++i)
    {

			m_vecLayers.push_back(SshenjingLayer(m_SshenjingPerHiddenLyr,
                                         m_SshenjingPerHiddenLyr));
    }

    //���������
	  m_vecLayers.push_back(SshenjingLayer(m_NumOutputs, m_SshenjingPerHiddenLyr));
	}

  else
  {
	  //���û�����ز�ֱ�Ӵ��������
	  m_vecLayers.push_back(SshenjingLayer(m_NumOutputs, m_NumInputs));
  }
}

//	����Ȩ��ֵ
vector<double> CSshenjingNet::GetQuanzhong() const
{
	vector<double> weights;
	
	//ѭ��ÿһ��
	for (int i=0; i<m_NumHiddenLayers + 1; ++i)
	{

		//ѭ��ÿһ����ϸ��
		for (int j=0; j<m_vecLayers[i].m_NumSshenjing; ++j)
		{
			//ѭ��ÿһ����ϸ����Ȩ��
			for (int k=0; k<m_vecLayers[i].m_vecSshenjing[j].m_NumInputs; ++k)
			{
				weights.push_back(m_vecLayers[i].m_vecSshenjing[j].m_vecWeight[k]);
			}
		}
	}

	return weights;
}

//����Ȩ��
void CSshenjingNet::PutQuanzhong(vector<double> &weights)
{
	int cWeight = 0;
	
	//ѭ��ÿһ��
	for (int i=0; i<m_NumHiddenLayers + 1; ++i)
	{

		//ѭ��ÿһ����ϸ��
		for (int j=0; j<m_vecLayers[i].m_NumSshenjing; ++j)
		{
			//ѭ��ÿһ��Ȩ��
			for (int k=0; k<m_vecLayers[i].m_vecSshenjing[j].m_NumInputs; ++k)
			{
				m_vecLayers[i].m_vecSshenjing[j].m_vecWeight[k] = weights[cWeight++];
			}
		}
	}

	return;
}

//  ����һ�����������Ȩ��������
int CSshenjingNet::GetNumberOfWeights() const
{

	int weights = 0;
	
	//ѭ��ÿһ��
	for (int i=0; i<m_NumHiddenLayers + 1; ++i)
	{

		//ѭ��ÿһ����ϸ��
		for (int j=0; j<m_vecLayers[i].m_NumSshenjing; ++j)
		{
			//ѭ��ÿһ��Ȩ��
			for (int k=0; k<m_vecLayers[i].m_vecSshenjing[j].m_NumInputs; ++k)
			
				weights++;			
		}
	}

	return weights;
}

//  ����ÿ������������
vector<double> CSshenjingNet::Update(vector<double> &inputs)
{
	//����ÿһ���������
	vector<double> outputs;

	int cWeight = 0;
	
	//���ȼ������ĸ�����ȷ��
	if (inputs.size() != m_NumInputs)
  {
		// ������󣬷���һ���յ�����
		return outputs;
  }
	
	//NumHiddenLayers + 1; ++i)
	{
		
		if ( i > 0 )
    {
			inputs = outputs;//��һ������������һ������
    }

		outputs.clear(); //��������㣬���ڴ洢��һ������
		
		cWeight = 0; 

		for (int j=0; j<m_vecLayers[i].m_NumSshenjing; ++j)
		{
			double netinput = 0;

			int	NumInputs = m_vecLayers[i].m_vecSshenjing[j].m_NumInputs;
			
			//ѭ��ÿһ��Ȩ��
			for (int k=0; k<NumInputs - 1; ++k)
			{
				//����Ȩ��*����
				netinput += m_vecLayers[i].m_vecSshenjing[j].m_vecWeight[k] * 
                    inputs[cWeight++];
			}

			//����ƫ����
			netinput += m_vecLayers[i].m_vecSshenjing[j].m_vecWeight[NumInputs-1] * 
                  CParams::dBias;

			//ÿһ�������Ҫ��������
			//����ֵ��Ҫ�ȵõ�S�κ����Ĺ���
			outputs.push_back(Sigmoid(netinput,
                                CParams::dActivationResponse));

			cWeight = 0;
		}
	}

	return outputs; 
}

//  S�κ���
double CSshenjingNet::Sigmoid(double netinput, double response)
{
	return ( 1 / ( 1 + exp(-netinput / response)));
}



vector<int> CSshenjingNet::CalculateSplitPoints() const
{
	vector<int> SplitPoints;

  int WeightCounter = 0;
	
	//ѭ��ÿһ��
	for (int i=0; i<m_NumHiddenLayers + 1; ++i)
	{
		//ѭ��ÿһ��ϸ��
		for (int j=0; j<m_vecLayers[i].m_NumSshenjing; ++j)
		{
			//ѭ��ÿһ��Ȩ��
			for (int k=0; k<m_vecLayers[i].m_vecSshenjing[j].m_NumInputs; ++k)
      {
				++WeightCounter;			
      }

      SplitPoints.push_back(WeightCounter - 1);
		}
	}

	return SplitPoints;
}