#ifndef CMINESWEEPER_H
#define CMINESWEEPER_H

//扫雷类

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

	CSshenjingNet  m_ItsBrain; //扫雷机的大脑，在这里就是神经网络
  
    //扫雷机的二维坐标位置
	SVector2D m_vPosition;

	// 扫雷机面对的方向向量
	SVector2D m_vLookAt;

	//旋转角度
	double m_dRotation;

	double m_dSpeed;

	//扫雷机的左右轮，也就是神经网络的输出
	double m_lTrack, m_rTrack;

	//扫雷机适应性分数
	double m_dFitness;

	double m_dScale;

	//最靠近地雷的位置
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

	
	