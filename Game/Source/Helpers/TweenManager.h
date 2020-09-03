#pragma once
#include "Helpers/TweenFuncs.h"
class TweenObject;
class GameObject;
class Game;

enum TweenState{
	TWEEN_NULL = -1,
	TWEEN_RUNNING = 0,
	TWEEN_SUCCESS,
	TWEEN_PAUSED
};

using namespace std;
class TweenManager
{
protected:

	Game* m_pGame;

    map<GameObject*, vector<TweenObject*>>m_pTranslationTweens;
	map<GameObject*, vector<TweenObject*>>m_pScalarTweens;
    mat4 m_pOrthoPos;
    bool m_pDoesOrtho;
	vec2 m_pWindowSize;

public:
    TweenManager(Game* pGame);
    ~TweenManager();

    void Update(float deltaTime);

    void Setup1DTween(GameObject* aGameObject, TweenFunc aTween, float* aModifier, float startvalue, float endvalue, double timepassed, double totaltime);
    void Setup2DTween(GameObject* aGameObject, TweenFunc aTween, vec2 startvalue, vec2 endvalue, double timepassed, double totaltime);
    void Setup3DTween(GameObject* aGameObject, TweenFunc aTween, vec3 startvalue, vec3 endvalue, double timepassed, double totaltime);
	void Setup2DScalerTween(GameObject* aGameObject, TweenFunc aTween, vec2 startvalue, vec2 endvalue, double timepassed, double totaltime);

	void OnSurfaceChange(GameObject* anObject, const mat4 OrthoInverse, float* aStartingPos, float* aEndingPos);
    void RemoveTweensFromObject(GameObject* anObject);

	TweenState GetTweenStateByObject(GameObject* anObject);
	void SetTweenStateByObject(GameObject* anObject, TweenState aState);
	bool GetDoesObjectTween(GameObject* anObject);
	void ResetTweensOnObject(GameObject* anObject);

    void SetInternalOrthoMatrix(mat4 anOrtho) { m_pOrthoPos = anOrtho;}
    void SetInternalOrthoPerspectiveCalculations(bool yesDoesOrtho) { m_pDoesOrtho = yesDoesOrtho; }


private:
};