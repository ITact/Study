#include "CNeuralNet.h"

Sshenjing::Sshenjing(int NumInputs): m_NumInputs(NumInputs+1)
											
{
	//需要附加一个偏移量
	for (int i=0; i<NumInputs+1; ++i)
	{
		//把所有权重设置为随机值，后面会慢慢优化
		m_vecWeight.push_back(RandomClamped());
	}
}

//神经网络层
SshenjingLayer::SshenjingLayer(int NumSshenjing, 
                           int NumInputsPerNeuron):	m_NumSshenjing(NumSshenjing)
{
	for (int i=0; i<NumSshenjing; ++i)

		m_vecSshenjing.push_back(Sshenjing(NumInputsPerNeuron)); //m_vecSshenjing存放了NumSshenjing个神经元，每个神经元包含NumInputsPerNeuron个输入
}

//初始化神经网络，默认值在param.ini里面
CSshenjingNet::CSshenjingNet() 
{
	//从参数列表中获取各个参数
	m_NumInputs = CParams::iNumInputs;
	m_NumOutputs = CParams::iNumOutputs;
	m_NumHiddenLayers =	CParams::iNumHidden;
	m_SshenjingPerHiddenLyr = CParams::iSshenjingPerHiddenLayer;

	CreateNet();
}

//创建神经网络层，w为-1到1
void CSshenjingNet::CreateNet()
{
	//创建网络的每一个层
	if (m_NumHiddenLayers > 0)
	{
		//创建第一个隐藏层
	  m_vecLayers.push_back(SshenjingLayer(m_SshenjingPerHiddenLyr, m_NumInputs));
    
    for (int i=0; i<m_NumHiddenLayers-1; ++i)
    {

			m_vecLayers.push_back(SshenjingLayer(m_SshenjingPerHiddenLyr,
                                         m_SshenjingPerHiddenLyr));
    }

    //创建输出层
	  m_vecLayers.push_back(SshenjingLayer(m_NumOutputs, m_SshenjingPerHiddenLyr));
	}

  else
  {
	  //如果没有隐藏层直接创建输出层
	  m_vecLayers.push_back(SshenjingLayer(m_NumOutputs, m_NumInputs));
  }
}

//	返回权重值
vector<double> CSshenjingNet::GetQuanzhong() const
{
	vector<double> weights;
	
	//循环每一层
	for (int i=0; i<m_NumHiddenLayers + 1; ++i)
	{

		//循环每一个神经细胞
		for (int j=0; j<m_vecLayers[i].m_NumSshenjing; ++j)
		{
			//循环每一个神经细胞的权重
			for (int k=0; k<m_vecLayers[i].m_vecSshenjing[j].m_NumInputs; ++k)
			{
				weights.push_back(m_vecLayers[i].m_vecSshenjing[j].m_vecWeight[k]);
			}
		}
	}

	return weights;
}

//输入权重
void CSshenjingNet::PutQuanzhong(vector<double> &weights)
{
	int cWeight = 0;
	
	//循环每一层
	for (int i=0; i<m_NumHiddenLayers + 1; ++i)
	{

		//循环每一个神经细胞
		for (int j=0; j<m_vecLayers[i].m_NumSshenjing; ++j)
		{
			//循环每一个权重
			for (int k=0; k<m_vecLayers[i].m_vecSshenjing[j].m_NumInputs; ++k)
			{
				m_vecLayers[i].m_vecSshenjing[j].m_vecWeight[k] = weights[cWeight++];
			}
		}
	}

	return;
}

//  返回一个网络里面的权重总数量
int CSshenjingNet::GetNumberOfWeights() const
{

	int weights = 0;
	
	//循环每一层
	for (int i=0; i<m_NumHiddenLayers + 1; ++i)
	{

		//循环每一个神经细胞
		for (int j=0; j<m_vecLayers[i].m_NumSshenjing; ++j)
		{
			//循环每一个权重
			for (int k=0; k<m_vecLayers[i].m_vecSshenjing[j].m_NumInputs; ++k)
			
				weights++;			
		}
	}

	return weights;
}

//  根据每次输入计算输出
vector<double> CSshenjingNet::Update(vector<double> &inputs)
{
	//储存每一层的输出结果
	vector<double> outputs;

	int cWeight = 0;
	
	//首先检查输入的个数正确吗
	if (inputs.size() != m_NumInputs)
  {
		// 如果错误，返回一个空的向量
		return outputs;
  }
	
	//NumHiddenLayers + 1; ++i)
	{
		
		if ( i > 0 )
    {
			inputs = outputs;//下一层的输入就是上一层的输出
    }

		outputs.clear(); //输出再清零，用于存储下一层的输出
		
		cWeight = 0; 

		for (int j=0; j<m_vecLayers[i].m_NumSshenjing; ++j)
		{
			double netinput = 0;

			int	NumInputs = m_vecLayers[i].m_vecSshenjing[j].m_NumInputs;
			
			//循环每一个权重
			for (int k=0; k<NumInputs - 1; ++k)
			{
				//计算权重*输入
				netinput += m_vecLayers[i].m_vecSshenjing[j].m_vecWeight[k] * 
                    inputs[cWeight++];
			}

			//增加偏移量
			netinput += m_vecLayers[i].m_vecSshenjing[j].m_vecWeight[NumInputs-1] * 
                  CParams::dBias;

			//每一层输出都要保存起来
			//激励值需要先得到S形函数的过滤
			outputs.push_back(Sigmoid(netinput,
                                CParams::dActivationResponse));

			cWeight = 0;
		}
	}

	return outputs; 
}

//  S形函数
double CSshenjingNet::Sigmoid(double netinput, double response)
{
	return ( 1 / ( 1 + exp(-netinput / response)));
}



vector<int> CSshenjingNet::CalculateSplitPoints() const
{
	vector<int> SplitPoints;

  int WeightCounter = 0;
	
	//循环每一层
	for (int i=0; i<m_NumHiddenLayers + 1; ++i)
	{
		//循环每一个细胞
		for (int j=0; j<m_vecLayers[i].m_NumSshenjing; ++j)
		{
			//循环每一个权重
			for (int k=0; k<m_vecLayers[i].m_vecSshenjing[j].m_NumInputs; ++k)
      {
				++WeightCounter;			
      }

      SplitPoints.push_back(WeightCounter - 1);
		}
	}

	return SplitPoints;
}