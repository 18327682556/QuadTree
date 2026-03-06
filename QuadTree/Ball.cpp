#include "Ball.h"
#include "Time.h"

#define COLLISION_COLOR RGB(255,255,255)

float Ball::lastTime = 0.1f;

void Ball::CollideWith(const Ball& other)
{
	float distance = sqrtf(powf(this->x - other.x, 2.0f) + powf(this->y - other.y, 2.0f));
	if (distance < (other.m_radius + m_radius) && !_IsAAwayFromB(*this, other))
	{
		this->m_endTime = (float)Time::GetInstance().time() + Ball::lastTime;
		this->m_color = COLLISION_COLOR;
		float nx = this->x - other.x, ny = this->y - other.y;
		auto V1 = _Project(m_vx, m_vy, nx, ny);
		if (nx == 0)
		{
			nx = 1.0f, ny = 0.0f;
		}
		else
		{
			ny = -nx / ny;
			nx = 1.0f;
		}
		auto V2 = _Project(m_vx, m_vy, nx, ny);
		this->m_vx = V2.first - V1.first;
		this->m_vy = V2.second - V1.second;
	}
	else
	{
		if (this->m_endTime < Time::GetInstance().time())
			this->m_color = this->m_originColor;
	}
}

void Ball::CollideWith(const Rect& wall)
{
	Point nextFrame = Point(this->x, this->y);
	if (wall.Inside(nextFrame))
	{
		if (this->m_endTime < Time::GetInstance().time())
			this->m_color = this->m_originColor;
	}
	else
	{
		this->m_endTime = (float)Time::GetInstance().time() + Ball::lastTime;
		this->m_color = COLLISION_COLOR;

		if (nextFrame.x < wall.x)
		{
			this->m_vx = abs(this->m_vx);
		}
		if(nextFrame.x > wall.x + wall.width)
		{
			this->m_vx = -abs(this->m_vx);
		}
		if (nextFrame.y < wall.y)
		{
			this->m_vy = abs(this->m_vy);
		}
		if(nextFrame.y > wall.y + wall.height)
		{
			this->m_vy = -abs(this->m_vy);
		}
	}
}

void Ball::Update()
{
	x += m_vx;
	y += m_vy;
}

void Ball::Update(float deltaTime)
{
	x += m_vx * deltaTime;
	y += m_vy * deltaTime;
}

void Ball::Draw(Canvas* canvas)
{
	if (canvas)
		canvas->SolidCircle(x, y, m_radius, m_color);
}

std::pair<float, float> Ball::_Project(float inX, float inY, float nX, float nY)
{
	float c = _Dot(inX, inY, nX, nY) / (powf(nX, 2.0f) + powf(nY, 2.0f));

	return std::pair<float, float>(c * nX, c * nY);
}

float Ball::_Dot(float x1, float y1, float x2, float y2)
{
	return x1 * x2 + y1 * y2;
}

inline std::pair<float, float> Ball::_Normalize(float inX, float inY)
{
	float magn = sqrtf(powf(inX, 2.0f) + powf(inY, 2.0f));
	return std::pair<float, float>(inX / magn, inX / magn);
}

inline bool Ball::_IsAAwayFromB(const Ball& A, const Ball& B) const
{
	float curDistance = powf(A.x - B.x, 2.0f) + powf(A.y - B.y, 2.0f);
	float nextDx = A.x + A.m_vx * Time::GetInstance().deltaTime() - B.x - B.m_vx * Time::GetInstance().deltaTime();
	float nextDy = A.y + A.m_vy * Time::GetInstance().deltaTime() - B.y - B.m_vy * Time::GetInstance().deltaTime();
	float nextDistance = powf(nextDx, 2.0f) + powf(nextDy, 2.0f);
	return curDistance < nextDistance;
}

std::pair<float, float> Ball::_Reflect(float inX, float inY, float nX, float nY)
{
	std::pair<float, float> ret;
	float magnN = sqrtf(nX * nX + nY * nY);
	float magnIn = sqrtf(inX * inX + inY * inY);
	float norInX = -inX / magnIn;
	float norInY = -inY / magnIn;
	float norNX = nX / magnN;
	float norNY = nY / magnN;
	float cosTheta = norInX * norNX + norInY * norNY;
	ret.first = 2 * magnIn * cosTheta * norNX + inX;
	ret.second = 2 * magnIn * cosTheta * norNY + inY;
	return ret;
}