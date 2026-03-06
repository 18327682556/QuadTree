#pragma once
#ifndef _QUADTREE_H_
#define _QUADTREE_H_
#include <iostream>
#include <vector>

struct Point
{
	float x, y;
	Point() :x(0.0f), y(0.0f) {}
	Point(float _x, float _y) :x(_x), y(_y) {}
};

struct Rect
{
	float x, y, width, height;
	Rect() :x(0.0f), y(0.0f), width(0.0f), height(0.0f) {}
	Rect(float _x, float _y, float _width, float _height) :x(_x), y(_y), width(_width), height(_height) {}

	bool Inside(const Point& point) const;
	bool Intersect(const Rect& rect) const;
};

struct QTreeNode
{
	union
	{
		struct { QTreeNode* m_pUpRight, * m_pUpLeft, * m_pBottomLeft, * m_pBottomRight; };	// 按照第笛卡尔坐标系第 1、2、3、4 象限顺序定义四个孩子
		QTreeNode* m_aChildren[4];
	};	
	Point** m_pPoints;			// 每个区域的点的引用数组
	uint8_t m_uPointCount;		// 每个区域点的数量
	Rect* m_pRect;				// 区域边界
	bool m_bIsDivide;			// 标识该节点是否被切割过

	QTreeNode();
	QTreeNode(Rect* rect, Point** points, QTreeNode* children[4] = {nullptr});
};


class QuadTree
{
private:
	QTreeNode* m_pQTree;		// 四叉树
	uint8_t m_uMaxPoints;		// 每个节点最大的节点数量

public:
	QuadTree(Rect rect, uint8_t maxPointsInRect);
	~QuadTree();

public:
	void Insert(Point& point);										// 插入一个点
	void Quary(const Rect& rect, std::vector<Point*>& ans);
	std::vector<Point*> Quary(const Rect& rect);					// 查寻区域 rect 中的点
	std::vector<Point*> Quary(const Point& center, float radius);	// 查询 (cneter, radius) 圆形区域的点
	void Clear();				// 清除当前所有节点，只剩下一个根节点（代表屏幕）

	void Show();

private:
	void Destory(QTreeNode* tree);				// 销毁 QuadTree
	bool InsertPoint(QTreeNode* root, Point& point);
	void ShowQTree(QTreeNode* root, void* canvas);
	void Query(QTreeNode* node, const Rect& rect, std::vector<Point*>& ans);
};

#endif // !_QUADTREE_H_

