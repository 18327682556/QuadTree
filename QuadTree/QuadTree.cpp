#include "QuadTree.h"
#include "Canvas.h"

QuadTree::QuadTree(Rect rect, uint8_t maxPointsInRect)
	:m_uMaxPoints(maxPointsInRect)
{
	Rect* tmpRect = new Rect(rect);
	Point** tmpPoints = new Point*[m_uMaxPoints];
	m_pQTree = new QTreeNode(tmpRect, tmpPoints);
}

QuadTree::~QuadTree()
{
	Destory(m_pQTree);
	m_pQTree = nullptr;
}

void QuadTree::Insert(Point& point)
{
	InsertPoint(m_pQTree,point);
}

void QuadTree::Quary(const Rect& rect, std::vector<Point*>& ans)
{
	Query(this->m_pQTree, rect, ans);
}

std::vector<Point*> QuadTree::Quary(const Rect& rect)
{
	std::vector<Point*> ret;
	Query(this->m_pQTree, rect, ret);
	return ret;
}

std::vector<Point*> QuadTree::Quary(const Point& center, float radius)
{
	return std::vector<Point*>();
}

void QuadTree::Clear()
{
	for (int i = 0; i < 4; ++i)
	{
		Destory(m_pQTree->m_aChildren[i]);
		m_pQTree->m_aChildren[i] = nullptr;
	}
	if (m_pQTree->m_pPoints == nullptr)
	{
		m_pQTree->m_pPoints = new Point * [m_uMaxPoints];
	}
	m_pQTree->m_bIsDivide = false;
	m_pQTree->m_uPointCount = 0;
}

void QuadTree::Show()
{
	Canvas* canvas = &Canvas::GetInstance();
	ShowQTree(m_pQTree, (void *)canvas);
}

void QuadTree::Destory(QTreeNode* tree)
{ 
	if (tree != nullptr)
	{
		/* 递归销毁孩子 */
		for (int i = 0; i < 4; ++i)	Destory(tree->m_aChildren[i]);

		if (tree->m_pRect != nullptr)
		{
			delete tree->m_pRect;
		}

		if (tree->m_pPoints != nullptr)
		{
			delete[] tree->m_pPoints;
		}

		delete tree;
	}
}

bool QuadTree::InsertPoint(QTreeNode* root, Point& point)
{
	if (root != nullptr)
	{
		if (!root->m_pRect->Inside(point)) return false;

		if (root->m_uPointCount < m_uMaxPoints)		// 当前区域可以插入该点
		{
			root->m_pPoints[root->m_uPointCount++] = &point;
			return true;
		}
		else if (root->m_bIsDivide)					// 当前节点已经分裂过了
		{
			for (int i = 0; i < 4; ++i)
			{
				if (InsertPoint(root->m_aChildren[i], point))
				{
					return true;
				}
			}
			return false;
		}
		else										// 分列节点
		{
			root->m_bIsDivide = true;
			Rect* cur = root->m_pRect;
			/* 创建所有孩子 */
			root->m_pUpRight = new QTreeNode(new Rect(cur->x + cur->width * 0.5f, cur->y, cur->width * 0.5f, cur->height * 0.5f), new Point * [m_uMaxPoints]);
			root->m_pUpLeft = new QTreeNode(new Rect(cur->x, cur->y, cur->width * 0.5f, cur->height * 0.5f), new Point * [m_uMaxPoints]);
			root->m_pBottomLeft = new QTreeNode(new Rect(cur->x, cur->y + cur->height * 0.5f, cur->width * 0.5f, cur->height * 0.5f), new Point * [m_uMaxPoints]);
			root->m_pBottomRight = new QTreeNode(new Rect(cur->x + cur->width * 0.5f, cur->y + cur->height * 0.5f, cur->width * 0.5f, cur->height * 0.5f), new Point * [m_uMaxPoints]);

			/* 将所有点插入到孩子中 */
			for (int i = 0; i < root->m_uPointCount; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					if (InsertPoint(root->m_aChildren[j], *root->m_pPoints[i])) break;
				}
			}

			/* 清除当前的点索引 */
			delete[] root->m_pPoints;		// 释放掉现在不需要的内存
			root->m_pPoints = nullptr;

			/* 插入新节点 */
			for (int i = 0; i < 4; ++i)
			{
				if (InsertPoint(root->m_aChildren[i], point))
				{
					return true;
				}
			}
			return false;
		}
		return true;
	}
	return false;
}

void QuadTree::ShowQTree(QTreeNode* root, void* canvas)
{
	if (root)
	{
		(*(Canvas*)canvas).Rectangle(*(root->m_pRect), RGB(50, 50, 50));

		for (int i = 0; i < 4; ++i)
		{
			ShowQTree(root->m_aChildren[i], canvas);
		}
	}
}

void QuadTree::Query(QTreeNode* node, const Rect& rect, std::vector<Point*>& ans)
{
	if (node)
	{
		if (!node->m_pRect->Intersect(rect))	// 当前区域跟查询区域无相交区域
		{
			return;
		}

		if (node->m_bIsDivide)					// 被分裂过，在孩子中查询，因为所有的点都被挂在了孩子上
		{
			for (int i = 0; i < 4; ++i)
			{
				Query(node->m_aChildren[i], rect, ans);
			}
		}
		else
		{
			for (int i = 0; i < node->m_uPointCount; ++i)
			{
				if (rect.Inside(*node->m_pPoints[i]))
				{
					ans.push_back(node->m_pPoints[i]);
				}
			}
		}
	}
}


QTreeNode::QTreeNode()
	:m_pUpLeft(nullptr), m_pBottomLeft(nullptr), m_pUpRight(nullptr), m_pBottomRight(nullptr)
	, m_pPoints(nullptr), m_pRect(nullptr), m_bIsDivide(false), m_uPointCount(0)
{

}

QTreeNode::QTreeNode(Rect* rect, Point** points, QTreeNode* children[4])
	:m_pUpLeft(nullptr), m_pBottomLeft(nullptr), m_pUpRight(nullptr), m_pBottomRight(nullptr),
	 m_pPoints(points), m_pRect(rect), m_bIsDivide(false), m_uPointCount(0)
{
	if (children != nullptr)
	{
		m_pUpRight = children[0];
		m_pUpLeft = children[1];
		m_pBottomLeft = children[2];   
		m_pBottomRight = children[3];
	}
}

bool Rect::Inside(const Point& point) const
{
	return (point.x >= x && point.y >= y
		&& point.x <= x + width && point.y <= y + height);
}

bool Rect::Intersect(const Rect& rect) const
{
	return !((this->x + this->width < rect.x)
		|| (this->y + this->height < rect.y)
		|| (rect.x + rect.width < this->x)
		|| (rect.y + rect.height < this->y)
		);
}
