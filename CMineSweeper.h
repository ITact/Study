#ifndef CMINESWEEPER_H
#define CMINESWEEPER_H

//ɨ����

#include <vector>
#include <math.h>

#include "CNeuralNet.h"
#include "utils.h"
#include "C2DMatrix.h"
#include "SVector2D.h"
#include "CParams.h"

using namespace std;




class CMinesweeper
{

private:

	CSshenjingNet  m_ItsBrain; //ɨ�׻��Ĵ��ԣ����������������
  
    //ɨ�׻��Ķ�ά����λ��
	SVector2D m_vPosition;

	// ɨ�׻���Եķ�������
	SVector2D m_vLookAt;

	//��ת�Ƕ�
	double m_dRotation;

	double m_dSpeed;

	//ɨ�׻��������֣�Ҳ��������������
	double m_lTrack, m_rTrack;

	//ɨ�׻���Ӧ�Է���
	double m_dFitness;

	double m_dScale;

	//������׵�λ��
	int m_iClosestMine;
  

public:
	

	CMinesweeper();
	
	bool Update(vector<SVector2D> &mines);

	void WorldTransform(vector<SPoint> &sweeper);

	SVector2D	GetClosestMine(vector<SVector2D> &objects);

	int CheckForMine(vector<SVector2D> &mines, double size);

	void Reset();
  
	SVector2D	        Position()const{return m_vPosition;}

	void IncrementFitness(){++m_dFitness;}

	double Fitness()const{return m_dFitness;}

	void              PutQuanzhong(vector<double> &w){m_ItsBrain.PutQuanzhong(w);}

	int               GetNumberOfWeights()const{return m_ItsBrain.GetNumberOfWeights();}

	vector<int>       CalculateSplitPoints()const{return m_ItsBrain.CalculateSplitPoints();}
};


#endif

	
	