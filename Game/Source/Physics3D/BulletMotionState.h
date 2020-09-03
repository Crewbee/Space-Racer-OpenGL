#pragma once
class GameObject;

class BulletMotionState : public btMotionState
{
protected:

public:
	BulletMotionState();
	~BulletMotionState();

	void getWorldTransform(btTransform& worldTrans) const override;
	void setWorldTransform();

private:

};