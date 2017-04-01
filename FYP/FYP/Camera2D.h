#pragma once
#include "BasicTypes.h"

class Camera2D
{
public:
	Camera2D(Rect viewPort, float initScale);
	Rect getViewport();
	float getScale();

	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();

	void increaseScale();
	void decreaseScale();

	void setLevelSize(Size2D size);

private:
	Rect m_viewPort;
	Size2D m_levelSize;
	float m_scale;
	float m_moveUnit;
	float m_scaleUnit;

	void limitValues();
};