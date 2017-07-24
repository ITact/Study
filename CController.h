#ifndef CCONTROLLER_H
#define CCONTROLLER_H

#include <vector>
#include <sstream>
#include <string>
#include <windows.h>

#include "CMinesweeper.h"
#include "CGenAlg.h"
#include "utils.h"
#include "C2DMatrix.h"
#include "SVector2D.h"
#include "CParams.h"


using namespace std;



class CController
{

private:

	vector<SGenome> m_vecThePopulation; //染色体种群
 
	vector<CMinesweeper> m_vecSweepers; //扫雷机集合

	//地雷
	vector<SVector2D> m_vecMines; //地雷集合

	//pointer to the GA
	CGenAlg* m_pGA;

	int m_NumSweepers;

	int m_NumMines;

	//神经网络权重的数量
	int m_NumWeightsInNN;

	//扫雷机形状顶点
	vector<SPoint> m_SweeperVB;

	//地雷形状的顶点
	vector<SPoint> m_MineVB;

	//存储每一代的平均适应分数
	vector<double> m_vecAvFitness; 

	//存储每一代最好的适应分数
	vector<double> m_vecBestFitness;

	//不同颜色的画笔
	HPEN m_RedPen;
	HPEN m_BluePen;
	HPEN m_GreenPen;
	HPEN m_OldPen;
	
	HWND m_hwndMain;

	bool m_bFastRender;
	
	int m_iTicks;

	//代计数器
	int m_iGenerations;

  // 窗口尺寸
  int cxClient, cyClient;

  void PlotStats(HDC surface);


public:

	CController(HWND hwndMain);

	~CController();


	void Render(HDC surface);

	void WorldTransform(vector<SPoint> &VBuffer, SVector2D vPos);
	
	bool Update();


	//accessor methods
	bool FastRender() {return m_bFastRender;}
	void FastRender(bool arg){m_bFastRender = arg;}
	void FastRenderToggle()  {m_bFastRender = !m_bFastRender;}

};

#endif
	
