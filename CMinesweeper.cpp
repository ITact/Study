#include "CMinesweeper.h"

CMinesweeper::CMinesweeper():
                             m_dRotation(RandFloat()*CParams::dTwoPi),
                             m_lTrack(0.16),
                             m_rTrack(0.16),
                             m_dFitness(CParams::dStartEnergy),
							               m_dScale(CParams::iSweeperScale),
                             m_iClosestMine(0)
			 
{
	//创建一个随机开始位置
	m_vPosition = SVector2D((RandFloat() * CParams::WindowWidth), 
					                (RandFloat() * CParams::WindowHeight));
  
}


void CMinesweeper::Reset()
{
	//充值扫雷及位置
	m_vPosition = SVector2D((RandFloat() * CParams::WindowWidth), 
					                (RandFloat() * CParams::WindowHeight));
	
	//充值精神等级（适应性分数？）
	m_dFitness = CParams::dStartEnergy;

	m_dRotation = RandFloat()*CParams::dTwoPi;

	return;
}

//世界坐标
void CMinesweeper::WorldTransform(vector<SPoint> &sweeper)
{
	//创建世界坐标顶点
	C2DMatrix matTransform;
	
	//scale 
	matTransform.Scale(m_dScale, m_dScale);
	
	//rotate 
	matTransform.Rotate(m_dRotation);
	
	//and translate 
	matTransform.Translate(m_vPosition.x, m_vPosition.y);
	
	//now transform the ships vertices
	matTransform.TransformSPoints(sweeper);
}

//扫雷机的更新
bool CMinesweeper::Update(vector<SVector2D> &mines)
{
	
	//存储神经网络的所有输入
	vector<double> inputs;	//存储输入

	//获取最接近的地雷
	SVector2D vClosestMine = GetClosestMine(mines);
  
	Vec2DNormalize(vClosestMine);
  
	double dot = Vec2DDot(m_vLookAt, vClosestMine);

	int sign   = Vec2DSign(m_vLookAt, vClosestMine);

	inputs.push_back(dot*sign);

	//将上面的输入传入到神经网络，并从中得到反馈
	vector<double> output = m_ItsBrain.Update(inputs); 

	//确保输出没问题
	if (output.size() < CParams::iNumOutputs) 
	{
		return false;
	}

	//把输出分配到l和r
	m_lTrack = output[0];  
	//输出所有轮速度
	m_rTrack = output[1];
	//转向力
	double RotForce = m_lTrack - m_rTrack; 

	Clamp(RotForce, -CParams::dMaxTurnRate, CParams::dMaxTurnRate);

	m_dRotation += RotForce; 

	//实际速度为左右轮之和
	m_dSpeed = (m_lTrack + m_rTrack);  

	//更新朝向
	m_vLookAt.x = -sin(m_dRotation);
	m_vLookAt.y = cos(m_dRotation);

	m_vPosition += (m_vLookAt * m_dSpeed);

	//超出界限的处理
	if (m_vPosition.x > CParams::WindowWidth) m_vPosition.x = 0;
	if (m_vPosition.x < 0) m_vPosition.x = CParams::WindowWidth;
	if (m_vPosition.y > CParams::WindowHeight) m_vPosition.y = 0;
	if (m_vPosition.y < 0) m_vPosition.y = CParams::WindowHeight;

	return true;
}


//返回扫雷机和最接近的地雷两点的向量
SVector2D CMinesweeper::GetClosestMine(vector<SVector2D> &mines)
{
	double			closest_so_far = 99999;

	SVector2D		vClosestObject(0, 0);

	//循环地雷寻找最接近的那一个
	for (int i=0; i<mines.size(); i++)
	{
		double len_to_object = Vec2DLength(mines[i] - m_vPosition);

		if (len_to_object < closest_so_far)
		{
			closest_so_far	= len_to_object;
			
			vClosestObject	= m_vPosition - mines[i];

      m_iClosestMine = i;
		}
	}

	return vClosestObject;
}

//检查是否撞到
int CMinesweeper::CheckForMine(vector<SVector2D> &mines, double size)
{
  SVector2D DistToObject = m_vPosition - mines[m_iClosestMine];
		
	if (Vec2DLength(DistToObject) < (size + 5))
	{
			return m_iClosestMine;
  }

  return -1;
}

		
