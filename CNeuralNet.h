#ifndef CSshenjingNET_H
#define CSshenjingNET_H

#include <vector>
#include <math.h>

#include "utils.h"
#include "CParams.h"

using namespace std;



//-------------------------------------------------------------------
//	定义神经细胞结构
//-------------------------------------------------------------------
struct Sshenjing
{
	//输入到神经细胞的数量
	int				      m_NumInputs;

	//每个输入的权重
	vector<double>	m_vecWeight;

	Sshenjing(int NumInputs);
};


//---------------------------------------------------------------------
//	神经细胞层 结构
//---------------------------------------------------------------------

struct SshenjingLayer
{
	//层的细胞数量
	int					      m_NumSshenjing;

	//这个层的神经细胞
	vector<Sshenjing>		m_vecSshenjing;

	SshenjingLayer(int NumSshenjing, 
				       int NumInputsPerShenjing); //第二个参数是一个神经元所含的输入数目
};


//----------------------------------------------------------------------
//	神经网络类
//----------------------------------------------------------------------

class CSshenjingNet
{
	
private:
	
	int m_NumInputs; //输入个数

	int m_NumOutputs; //输出个数

	int	m_NumHiddenLayers;  //隐藏层个数，除了输出层都是隐藏层

	int m_SshenjingPerHiddenLyr; //每个隐藏层包含的神经元个数

	//存储每一层的神经细胞，包括输出层，构成了整个神经网络
	vector<SshenjingLayer>	m_vecLayers;

public:

	CSshenjingNet();

	void CreateNet(); //创建整个网络

	//从神经网络中读取权重
	vector<double>	GetQuanzhong()const;

	//返回网络中权重的总数
	int				      GetNumberOfWeights()const;

	//用心的权重替换原有的权重 为什么？
	void			      PutQuanzhong(vector<double> &weights);

	//根据输入计算输出
	vector<double>	Update(vector<double> &inputs);

	//逻辑斯蒂曲线
	inline double	  Sigmoid(double activation, double response);

	vector<int>     CalculateSplitPoints() const;
};
			

#endif