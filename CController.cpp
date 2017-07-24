#include "CController.h"


//these hold the geometry of the sweepers and the mines 扫雷机和地雷的几何图形
const int	 NumSweeperVerts = 16;
//绘制一辆扫雷机
const SPoint sweeper[NumSweeperVerts] = {SPoint(-1, -1),
                                         SPoint(-1, 1),
                                         SPoint(-0.5, 1),
                                         SPoint(-0.5, -1),

                                         SPoint(0.5, -1),
                                         SPoint(1, -1),
                                         SPoint(1, 1),
                                         SPoint(0.5, 1),
                                         
                                         SPoint(-0.5, -0.5),
                                         SPoint(0.5, -0.5),
                                         SPoint(-0.5, 0.5),
                                         SPoint(-0.25, 0.5),

                                         SPoint(-0.25, 1.75),
                                         SPoint(0.25, 1.75),
                                         SPoint(0.25, 0.5),
                                         SPoint(0.5, 0.5)};
//绘制一个地雷
const int NumMineVerts = 4;
const SPoint mine[NumMineVerts] = {SPoint(-1, -1),
                                   SPoint(-1, 1),
                                   SPoint(1, 1),
                                   SPoint(1, -1)};



//---------------------------------------constructor---------------------
//
//	initilaize the sweepers, their brains and the GA factory
//
//-----------------------------------------------------------------------
CController::CController(HWND hwndMain): m_NumSweepers(CParams::iNumSweepers), 
										                     m_pGA(NULL),
										                     m_bFastRender(false),
										                     m_iTicks(0),
										                     m_NumMines(CParams::iNumMines),
										                     m_hwndMain(hwndMain),
										                     m_iGenerations(0),
                                         cxClient(CParams::WindowWidth),
                                         cyClient(CParams::WindowHeight)
{
	//let's create the mine sweepers 创建地雷和扫雷机
	for (int i=0; i<m_NumSweepers; ++i)
	{
		m_vecSweepers.push_back(CMinesweeper());
	}

	//get the total number of weights used in the sweepers 获取扫雷机的权重数量
	//NN so we can initialise the GA
	m_NumWeightsInNN = m_vecSweepers[0].GetNumberOfWeights();

	//calculate the neuron placement in the weight vector
	vector<int> SplitPoints = m_vecSweepers[0].CalculateSplitPoints();

	//initialize the Genetic Algorithm class 初始化遗传算法类
	m_pGA = new CGenAlg(m_NumSweepers,
                      CParams::dMutationRate,
	                    CParams::dCrossoverRate,
	                    m_NumWeightsInNN,
                      SplitPoints);

	//Get the weights from the GA and insert into the sweepers brains 从遗传算法处获取权重，然后插入到扫雷机大脑
	m_vecThePopulation = m_pGA->GetChromos();

	for (i=0; i<m_NumSweepers; i++)
	
		m_vecSweepers[i].PutQuanzhong(m_vecThePopulation[i].vecWeights);

	//initialize mines in random positions within the application window 随机防止地雷
	for (i=0; i<m_NumMines; ++i)
	{
		m_vecMines.push_back(SVector2D(RandFloat() * cxClient,
                                   RandFloat() * cyClient));
	}

	//create a pen for the graph drawing 
	m_BluePen  = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	m_RedPen   = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_GreenPen = CreatePen(PS_SOLID, 1, RGB(0, 150, 0));

	m_OldPen	= NULL;

	//fill the vertex buffers
	for (i=0; i<NumSweeperVerts; ++i)
	{
		m_SweeperVB.push_back(sweeper[i]);
	}

	for (i=0; i<NumMineVerts; ++i)
	{
		m_MineVB.push_back(mine[i]);
	}

}


//--------------------------------------destructor-------------------------------------
//
//--------------------------------------------------------------------------------------
CController::~CController()
{
	if(m_pGA)
  {
    delete		m_pGA;
  }

	DeleteObject(m_BluePen);
	DeleteObject(m_RedPen);
	DeleteObject(m_GreenPen);
	DeleteObject(m_OldPen);
}


//---------------------WorldTransform--------------------------------
//
//	sets up the translation matrices for the mines and applies the
//	world transform to each vertex in the vertex buffer passed to this
//	method.
//-------------------------------------------------------------------
void CController::WorldTransform(vector<SPoint> &VBuffer, SVector2D vPos)
{
	//create the world transformation matrix
	C2DMatrix matTransform;
	
	//scale
	matTransform.Scale(CParams::dMineScale, CParams::dMineScale);
	
	//translate
	matTransform.Translate(vPos.x, vPos.y);

	//transform the ships vertices
	matTransform.TransformSPoints(VBuffer);
}


//-------------------------------------Update-----------------------------
//
//	This is the main workhorse. The entire simulation is controlled from here.
//
//	The comments should explain what is going on adequately.
//-------------------------------------------------------------------------
bool CController::Update()
{
	//run the sweepers through CParams::iNumTicks amount of cycles. During
  //this loop each sweepers NN is constantly updated with the appropriate
  //information from its surroundings. The output from the NN is obtained
  //and the sweeper is moved. If it encounters a mine its fitness is
  //updated appropriately,
	if (m_iTicks++ < CParams::iNumTicks) //每一代运行的时间，默认为2000
	{
		for (int i=0; i<m_NumSweepers; ++i)
		{
			//update the NN and position
			if (!m_vecSweepers[i].Update(m_vecMines))
			{
				//error in processing the neural net
				MessageBox(m_hwndMain, "Wrong amount of NN inputs!", "Error", MB_OK);

				return false;
			}
				
			//see if it's found a mine 检查是否碰撞到地雷，grabhit是碰到的地雷的编号，若为-1表示没有碰到地雷
      int GrabHit = m_vecSweepers[i].CheckForMine(m_vecMines,
                                                  CParams::dMineScale);

			if (GrabHit >= 0)
      {
        //we have discovered a mine so increase fitness 
        m_vecSweepers[i].IncrementFitness(); //适应性分数+1

        //碰到地雷之后，再随机地放置另一个地雷
			  m_vecMines[GrabHit] = SVector2D(RandFloat() * cxClient,
                                        RandFloat() * cyClient);
      }

			//更新每一个染色体的适应性分数
			m_vecThePopulation[i].dFitness = m_vecSweepers[i].Fitness(); 

		}
	}

	//Another generation has been completed. 已经执行完一代，是时候使用遗传算法更新新的扫雷机
	//Time to run the GA and update the sweepers with their new NNs
	else
	{
		//update the stats to be used in our stat window 将每一代的平均适应性分数和最高适应性分数记录下来
		m_vecAvFitness.push_back(m_pGA->AverageFitness());
		m_vecBestFitness.push_back(m_pGA->BestFitness());

		//increment the generation counter
		++m_iGenerations; 

		//reset cycles 重置当前的帧数，为了下一次循环
		m_iTicks = 0; 

		//使用遗传算法创建一个新的种群，这个种群里面有权重
		m_vecThePopulation = m_pGA->Epoch(m_vecThePopulation);
			
		//insert the new (hopefully)improved brains back into the sweepers
    //and reset their positions etc 将新的权重插入神经网络
    for (int i=0; i<m_NumSweepers; ++i)
		{
			m_vecSweepers[i].PutQuanzhong(m_vecThePopulation[i].vecWeights);
		
			m_vecSweepers[i].Reset();
		}
	}

	return true;
}
//渲染方法
void CController::Render(HDC surface)
{
	string s = "代数:          " + itos(m_iGenerations);
	TextOut(surface, 5, 0, s.c_str(), s.size());

	//如果是加速模式，就不渲染扫雷机
	if (!m_bFastRender)
	{
		//keep a record of the old pen
		m_OldPen = (HPEN)SelectObject(surface, m_GreenPen);

		//渲染地雷
		for (int i=0; i<m_NumMines; ++i)
		{
			//获取地雷的顶点
			vector<SPoint> mineVB = m_MineVB;

			WorldTransform(mineVB, m_vecMines[i]);

			//绘画地雷
			MoveToEx(surface, (int)mineVB[0].x, (int)mineVB[0].y, NULL);

			for (int vert=1; vert<mineVB.size(); ++vert)
			{
				LineTo(surface, (int)mineVB[vert].x, (int)mineVB[vert].y);
			}

			LineTo(surface, (int)mineVB[0].x, (int)mineVB[0].y);		
		}
    	
		//把适应性最强的扫雷机设置为红色
		SelectObject(surface, m_RedPen);

		//绘制扫雷机
		for (i=0; i<m_NumSweepers; i++)
		{
			if (i == CParams::iNumElite)
			{
				SelectObject(surface, m_OldPen);
			}
      
			//获取扫雷机顶点
			vector<SPoint> sweeperVB = m_SweeperVB;

			//转换顶点
			m_vecSweepers[i].WorldTransform(sweeperVB);

			//绘制扫雷机左边轨迹
			MoveToEx(surface, (int)sweeperVB[0].x, (int)sweeperVB[0].y, NULL);

			for (int vert=1; vert<4; ++vert)
			{
				LineTo(surface, (int)sweeperVB[vert].x, (int)sweeperVB[vert].y);
			}

			LineTo(surface, (int)sweeperVB[0].x, (int)sweeperVB[0].y);

			//绘制扫雷机右边轨迹
			MoveToEx(surface, (int)sweeperVB[4].x, (int)sweeperVB[4].y, NULL);

			for (vert=5; vert<8; ++vert)
			{
				LineTo(surface, (int)sweeperVB[vert].x, (int)sweeperVB[vert].y);
			}

			LineTo(surface, (int)sweeperVB[4].x, (int)sweeperVB[4].y);


			MoveToEx(surface, (int)sweeperVB[8].x, (int)sweeperVB[8].y, NULL);
			LineTo(surface, (int)sweeperVB[9].x, (int)sweeperVB[9].y);

			MoveToEx(surface, (int)sweeperVB[10].x, (int)sweeperVB[10].y, NULL);

			for (vert=11; vert<16; ++vert)
			{
				LineTo(surface, (int)sweeperVB[vert].x, (int)sweeperVB[vert].y);
			}

		}

		SelectObject(surface, m_OldPen);
	}
	else
	{
		PlotStats(surface);
	}
}

// 按下f键的时候，变成统计模式
void CController::PlotStats(HDC surface)
{
   
    string s = "最佳适应性分数:       " + ftos(m_pGA->BestFitness());
	  TextOut(surface, 5, 20, s.c_str(), s.size());

     s = "平均适应性分数: " + ftos(m_pGA->AverageFitness());
	  TextOut(surface, 5, 40, s.c_str(), s.size());
    
    //渲染最佳和平均两天曲线
    float HSlice = (float)cxClient/(m_iGenerations+1);
    float VSlice = (float)cyClient/((m_pGA->BestFitness()+1)*2);

    //最佳适应性
    float x = 0;
    
    m_OldPen = (HPEN)SelectObject(surface, m_RedPen);

    MoveToEx(surface, 0, cyClient, NULL);
    
    for (int i=0; i<m_vecBestFitness.size(); ++i)
    {
       LineTo(surface, (int)x, (int)(cyClient - VSlice*m_vecBestFitness[i]));

       x += HSlice;
    }

    //平均适应性
    x = 0;

    SelectObject(surface, m_BluePen);

    MoveToEx(surface, 0, cyClient, NULL);
    
    for (i=0; i<m_vecAvFitness.size(); ++i)
    {
       LineTo(surface, (int)x, (int)(cyClient - VSlice*m_vecAvFitness[i]));

       x += HSlice;
    }

    SelectObject(surface, m_OldPen);
}

