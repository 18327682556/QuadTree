#pragma once
#include "Canvas.h"

struct Ball : public Point
{
	static float lastTime;		// 碰撞高亮延迟时间
	float		m_radius;
	COLORREF	m_color;
	COLORREF	m_originColor;
	float		m_vx;
	float		m_vy;
	float		m_endTime = 0.0f;

	void CollideWith(const Ball& other);
	void CollideWith(const Rect& wall);
	void Update();
	void Update(float deltaTime);
	void Draw(Canvas* canvas);

private:
	inline std::pair<float, float> _Reflect(float inX, float inY, float nX, float nY);
	inline std::pair<float, float> _Project(float inX, float inY, float nX, float nY);		// 求向量投影向量
	inline float _Dot(float x1, float y1, float x2, float y2);
	inline std::pair<float, float> _Normalize(float inX, float inY);
	inline bool _IsAAwayFromB(const Ball& A, const Ball& B) const;			// 判定两个小球是否在远离状态
};
