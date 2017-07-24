#include "CMinesweeper.h"

CMinesweeper::CMinesweeper():
                             m_dRotation(RandFloat()*CParams::dTwoPi),
                             m_lTrack(0.16),
                             m_rTrack(0.16),
                             m_dFitness(CParams::dStartEnergy),
							               m_dScale(CParams::iSweeperScale),
                             m_iClosestMine(0)
			 
{
	//����һ�������ʼλ��
	m_vPosition = SVector2D((RandFloat() * CParams::WindowWidth), 
					                (RandFloat() * CParams::WindowHeight));
  
}


void CMinesweeper::Reset()
{
	//��ֵɨ�׼�λ��
	m_vPosition = SVector2D((RandFloat() * CParams::WindowWidth), 
					                (RandFloat() * CParams::WindowHeight));
	
	//��ֵ����ȼ�����Ӧ�Է�������
	m_dFitness = CParams::dStartEnergy;

	m_dRotation = RandFloat()*CParams::dTwoPi;

	return;
}

//��������
void CMinesweeper::WorldTransform(vector<SPoint> &sweeper)
{
	//�����������궥��
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

//ɨ�׻��ĸ���
bool CMinesweeper::Update(vector<SVector2D> &mines)
{
	
	//�洢���������������
	vector<double> inputs;	//�洢����

	//��ȡ��ӽ��ĵ���
	SVector2D vClosestMine = GetClosestMine(mines);
  
	Vec2DNormalize(vClosestMine);
  
	double dot = Vec2DDot(m_vLookAt, vClosestMine);

	int sign   = Vec2DSign(m_vLookAt, vClosestMine);

	inputs.push_back(dot*sign);

	//����������봫�뵽�����磬�����еõ�����
	vector<double> output = m_ItsBrain.Update(inputs); 

	//ȷ�����û����
	if (output.size() < CParams::iNumOutputs) 
	{
		return false;
	}

	//��������䵽l��r
	m_lTrack = output[0];  
	//����������ٶ�
	m_rTrack = output[1];
	//ת����
	double RotForce = m_lTrack - m_rTrack; 

	Clamp(RotForce, -CParams::dMaxTurnRate, CParams::dMaxTurnRate);

	m_dRotation += RotForce; 

	//ʵ���ٶ�Ϊ������֮��
	m_dSpeed = (m_lTrack + m_rTrack);  

	//���³���
	m_vLookAt.x = -sin(m_dRotation);
	m_vLookAt.y = cos(m_dRotation);

	m_vPosition += (m_vLookAt * m_dSpeed);

	//�������޵Ĵ���
	if (m_vPosition.x > CParams::WindowWidth) m_vPosition.x = 0;
	if (m_vPosition.x < 0) m_vPosition.x = CParams::WindowWidth;
	if (m_vPosition.y > CParams::WindowHeight) m_vPosition.y = 0;
	if (m_vPosition.y < 0) m_vPosition.y = CParams::WindowHeight;

	return true;
}


//����ɨ�׻�����ӽ��ĵ������������
SVector2D CMinesweeper::GetClosestMine(vector<SVector2D> &mines)
{
	double			closest_so_far = 99999;

	SVector2D		vClosestObject(0, 0);

	//ѭ������Ѱ����ӽ�����һ��
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

//����Ƿ�ײ��
int CMinesweeper::CheckForMine(vector<SVector2D> &mines, double size)
{
  SVector2D DistToObject = m_vPosition - mines[m_iClosestMine];
		
	if (Vec2DLength(DistToObject) < (size + 5))
	{
			return m_iClosestMine;
  }

  return -1;
}

		
