#include <iostream>
#include <graphics.h>
#include <conio.h>
#include <cstdlib>
#include "InputManager.h"
#include "SceneManager.h"
#include "Button.h"
#include "ScrollBar.h"
#include "Label.h"
#include "Canvas.h"
#include "QuadTree.h"
#include "Time.h"
#include "Ball.h"

using namespace std;

sy::InputManager* inputMgr = nullptr;
sy::SceneManager* sceneMgr = nullptr;
Time* timeMgr = nullptr;
Canvas* canvas = nullptr;

bool initFlag = true;
void SimulationLoop();
bool firstEnd = true;
void EndSimulationLoop();
void ShowFPS(Canvas* canvas);

Rect wall = { 0.0f,0.0f,800.0f, 800.0f };
QuadTree *qTree = nullptr;	// 四叉树
int row;					// 每行多少个
int col;					// 每列多少个
int ballsNumber;			// 小球总数
float ballR;				// 小球半径
float ballShowTime;			// 小球碰撞后高亮显示延时
std::vector<Point*> ans;
Ball* balls = nullptr;

bool showDetectRect = false;// 显示坚持边框线
bool showQuadTree = false;	// 显示构建的四叉树

int main()
{
	Canvas::SetCanvas(1200, 800);
	inputMgr = &sy::InputManager::GetInstance();
	sceneMgr = &sy::SceneManager::GetInstance();
	canvas = &Canvas::GetInstance();
	timeMgr = &Time::GetInstance();	

#pragma region 搭建 UI 界面
	// 每行多少个
	row = 10;
	sy::Label lblRow(L"横行数量:");
	sy::ScrollBar scrRow;
	sy::Label lblRowCnt(L" ");
	lblRow.SetArea({ 800, 10, 100, 30 });
	scrRow.SetArea({ 900, 10, 250, 30 });
	lblRowCnt.SetArea({ 1150, 10, 50, 30 });
	lblRowCnt.SetText(L"10");
	scrRow.SetValue(0.2);
	scrRow.AddClickInvoke([&](double value)->void { row = (int)(value * 50); wstring tmp(4,' '); wsprintf((LPWSTR)tmp.data(), L"%d", row); lblRowCnt.SetText(tmp); });
	inputMgr->AddMouseListener(&scrRow);
	sceneMgr->Add(&lblRow);
	sceneMgr->Add(&scrRow);
	sceneMgr->Add(&lblRowCnt);

	// 每列多少个
	col = 10;
	sy::Label lblCol(L"竖行数量");
	sy::ScrollBar scrCol;
	sy::Label lblColCnt(L" ");
	lblCol.SetArea({ 800, 70, 100, 30 });
	scrCol.SetArea({ 900, 70, 250, 30 });
	lblColCnt.SetArea({ 1150, 70, 50, 30 });
	lblColCnt.SetText(L"10");
	scrCol.SetValue(0.2);
	scrCol.AddClickInvoke([&](double value)->void { col = (int)(value * 50); wstring tmp(4, ' '); wsprintf((LPWSTR)tmp.data(), L"%d", col); lblColCnt.SetText(tmp); });
	inputMgr->AddMouseListener(&scrCol);
	sceneMgr->Add(&lblCol);
	sceneMgr->Add(&scrCol);
	sceneMgr->Add(&lblColCnt);

	// 小球半径
	ballR = 8.0f;
	sy::Label lblBallR(L"小球半径:");
	sy::ScrollBar scrBallR;
	sy::Label lblBallRValue(L" ");
	lblBallR.SetArea({ 800, 130, 100, 30 });
	scrBallR.SetArea({ 900, 130, 250, 30 });
	lblBallRValue.SetArea({ 1150, 130, 50, 30 });
	lblBallRValue.SetText(L"8.0");
	scrBallR.AddClickInvoke([&](double value)->void { 
		// 注意这里使用了 mbstowcs_s 完成 string 向 wstring 的转化，因为 wsprintf 没有提供 %f，而标准库里的 sprintf 提供了 %f
		ballR = (float)value * 16.0f;
		wstring tmp(8, ' ');
		string tmp1(8, ' ');
		sprintf_s((char* const)tmp1.data(), 8, "%.1f", ballR);
		size_t wirteLen;
		mbstowcs_s(&wirteLen, (wchar_t*)tmp.data(), tmp1.size(), (char const*)tmp1.c_str(), 8);
		lblBallRValue.SetText(tmp);
	});
	inputMgr->AddMouseListener(&scrBallR);
	sceneMgr->Add(&lblBallR);
	sceneMgr->Add(&scrBallR);
	sceneMgr->Add(&lblBallRValue);


	// 碰撞后高亮显示延时
	ballShowTime = 0.1f;
	sy::Label lblBallDelayTime(L"碰撞后显示延时:");
	sy::ScrollBar scrBallDelay;
	sy::Label lblBallDelayTimeValue(L" ");
	lblBallDelayTime.SetArea({ 800, 190, 150, 30 });
	scrBallDelay.SetArea({ 950, 190, 200, 30 });
	lblBallDelayTimeValue.SetArea({ 1150, 190, 50, 30 });
	lblBallDelayTimeValue.SetText(L"0.10s");
	scrBallDelay.SetValue(0.1);
	scrBallDelay.AddClickInvoke([&](double value)->void {
		// 注意这里使用了 mbstowcs_s 完成 string 向 wstring 的转化，因为 wsprintf 没有提供 %f，而标准库里的 sprintf 提供了 %f
		ballShowTime = (float)value;
		wstring tmp(8, ' ');
		string tmp1(8, ' ');
		sprintf_s((char* const)tmp1.data(), 8, "%.2fs", ballShowTime);
		size_t wirteLen;
		mbstowcs_s(&wirteLen, (wchar_t*)tmp.data(), tmp1.size(), (char const*)tmp1.c_str(), 8);
		lblBallDelayTimeValue.SetText(tmp);
		});
	inputMgr->AddMouseListener(&scrBallDelay);
	sceneMgr->Add(&lblBallDelayTime);
	sceneMgr->Add(&scrBallDelay);
	sceneMgr->Add(&lblBallDelayTimeValue);


	// 是否显示检测框线
	sy::Button btnShowDectedRect;
	btnShowDectedRect.SetArea({ 900,630,200,40 });
	btnShowDectedRect.SetText(L"打开检测范围");
	btnShowDectedRect.SetVisualAble(false);
	btnShowDectedRect.SetActive(false);
	btnShowDectedRect.AddClickInvoke(
		[&]()->void
		{
			if (showDetectRect == false)
			{
				btnShowDectedRect.SetText(L"关闭检测范围");
				showDetectRect = true;
			}
			else
			{
				btnShowDectedRect.SetText(L"显示检测范围");
				showDetectRect = false;
			}
		}
	);
	inputMgr->AddMouseListener(&btnShowDectedRect);
	sceneMgr->Add(&btnShowDectedRect);

	// 是否显示构建的四叉树
	sy::Button btnShowQuadTree;
	btnShowQuadTree.SetVisualAble(false);
	btnShowQuadTree.SetActive(false);
	btnShowQuadTree.SetArea({ 900,530,200,40 });
	btnShowQuadTree.SetText(L"显示四叉树");
	btnShowQuadTree.AddClickInvoke(
		[&]()->void
		{
			if (showQuadTree == false)
			{
				btnShowQuadTree.SetText(L"不显示四叉树");
				showQuadTree = true;
			}
			else
			{
				btnShowQuadTree.SetText(L"显示四叉树");
				showQuadTree = false;
			}
		}
	);
	inputMgr->AddMouseListener(&btnShowQuadTree);
	sceneMgr->Add(&btnShowQuadTree);

	// 开始模拟按钮
	bool beginSim = false;
	sy::Button btnBeginSimulation;
	btnBeginSimulation.SetArea({ 900,730,200,40 });
	btnBeginSimulation.SetText(L"开 始 模 拟");
	btnBeginSimulation.AddClickInvoke(
		[&]()->void 
		{
			if (beginSim == false)
			{
				btnBeginSimulation.SetText(L"结 束 模 拟");
				beginSim = true;
				lblRow.SetActive(false);
				scrRow.SetActive(false);
				lblRowCnt.SetActive(false);
				lblCol.SetActive(false);
				scrCol.SetActive(false);
				lblColCnt.SetActive(false);
				lblBallR.SetActive(false);
				scrBallR.SetActive(false);
				lblBallRValue.SetActive(false);
				lblBallDelayTime.SetActive(false);;
				scrBallDelay.SetActive(false);
				lblBallDelayTimeValue.SetActive(false);

				btnShowQuadTree.SetActive(true);
				btnShowQuadTree.SetVisualAble(true);
				btnShowDectedRect.SetActive(true);
				btnShowDectedRect.SetVisualAble(true);
			}
			else
			{
				btnBeginSimulation.SetText(L"开 始 模 拟");
				beginSim = false;
				lblRow.SetActive(true);
				scrRow.SetActive(true);
				lblRowCnt.SetActive(true);
				lblCol.SetActive(true);
				scrCol.SetActive(true);
				lblColCnt.SetActive(true);
				lblBallR.SetActive(true);
				scrBallR.SetActive(true);
				lblBallRValue.SetActive(true);
				lblBallDelayTime.SetActive(true);;
				scrBallDelay.SetActive(true);
				lblBallDelayTimeValue.SetActive(true);

				btnShowQuadTree.SetActive(false);
				btnShowQuadTree.SetVisualAble(false);
				btnShowDectedRect.SetActive(false);
				btnShowDectedRect.SetVisualAble(false);
			}
		}
	);
	inputMgr->AddMouseListener(&btnBeginSimulation);
	sceneMgr->Add(&btnBeginSimulation);
#pragma endregion // 搭建 UI 界面

	srand((unsigned int)time(NULL));

	while (!_kbhit())
	{
		cleardevice();
		line(800, 0, 800, 800);

		if (beginSim)
		{
			ballsNumber = col * row;
			SimulationLoop();
			firstEnd = true;
			initFlag = false;
		}
		else
		{
			if (firstEnd)
			{
				EndSimulationLoop();
				firstEnd = false;
				initFlag = true;
			}
		}

		timeMgr->Update();

		inputMgr->ProcessMessage();

		sceneMgr->Update();

		sceneMgr->Show();

		canvas->Present();		// 呈现
	}
	EndBatchDraw();
	return 0;
}

void SimulationLoop()
{
	if (initFlag)
	{
		qTree = new QuadTree(wall, 1);
		ans.resize(ballsNumber, nullptr);
		balls = new Ball[ballsNumber];
		Ball::lastTime = ballShowTime;
		for (int i = 0; i < row; ++i)
		{
			for (int j = 0; j < col; ++j)
			{
				int index = i * col + j;
				balls[index].x = rand() % ((800 / row) - 11) + 5 + (i * (800.0f / row));
				balls[index].y = rand() % ((800 / col) - 11) + 5 + (j * (800.0f / col));

				balls[index].m_vx = (rand() % 2001) / 10.0f - 100.0f;
				balls[index].m_vy = (rand() % 2001) / 10.0f - 100.0f;

				balls[index].m_vx > 0 ? balls[index].m_vx += 100.0f : balls[index].m_vx -= 100.0f;
				balls[index].m_vy > 0 ? balls[index].m_vy += 100.0f : balls[index].m_vx -= 100.0f;

				balls[index].m_color = balls[index].m_originColor = RGB(50, 50, 50);
				balls[index].m_radius = ballR;
			}
		}
		initFlag = false;
	}

	for (int i = 0; i < ballsNumber; ++i)
	{
		qTree->Insert(balls[i]);
	}

	for (int i = 0; i < ballsNumber; ++i)
	{
		static Rect detectRect = { wall.x + ballR, wall.y + ballR, wall.width - ballR * 2, wall.height - ballR * 2 };
		balls[i].CollideWith(detectRect);
	}

	for (int i = 0; i < ballsNumber; ++i)
	{
		ans.resize(0);
		qTree->Quary({ balls[i].x - 50, balls[i].y - 50, 100, 100 }, ans);
		if(showDetectRect)
			canvas->Rectangle({ balls[i].x - 50, balls[i].y - 50, 100, 100 }, GREEN);
		for (int j = 0; j < ans.size(); ++j)
		{
			if (ans[j] != &balls[i])
			{
				balls[i].CollideWith(*(Ball*)ans[j]);
			}
		}
	}

	for (int i = 0; i < ballsNumber; ++i)
	{
		balls[i].Update((float)timeMgr->deltaTime());
		balls[i].Draw(canvas);
	}

	if(showQuadTree)
		qTree->Show();

	ShowFPS(canvas);

	qTree->Clear();			// 清除 QuadTree
}

void EndSimulationLoop()
{
	delete[] balls;
	balls = nullptr;
	ans.clear();
	delete qTree;
	qTree = nullptr;
}

void ShowFPS(Canvas* canvas)
{
	if (!canvas) return;
	static TCHAR tcFrame[16];
	static float nextTime;
	int frame;

	if (Time::GetInstance().time() > nextTime)	// 绘制 FPS
	{
		nextTime += 1.0f;
		frame = (int)(1.0 / Time::GetInstance().deltaTime());
		wsprintf(tcFrame, L"FPS:%d\n", (int)(frame + 0.5));
	}
	settextcolor(GREEN);
	settextstyle(15, 0, L"宋体");
	outtextxy(1, 1, tcFrame);
}
