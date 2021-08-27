#pragma once

#include <memory>
#include "Sprite.h"
#include "Timer.h"


class StaminaManager
{
public:
    StaminaManager(const float max_stamina_second = 5.0f) : max_stamina_second(max_stamina_second) {};
    ~StaminaManager() {};

public:
    void Init();
    void UnInit();
    void Update();
    void Render(const DirectX::XMFLOAT2 pos = {});

public: // Get関数
    bool IsDash();      // ダッシュが可能か

public: // Set関数
    void Dash();        // ダッシュ処理

private:
    void Clear();
    void CoolDown();

private:
    static constexpr float zero_stamina = 0.0f;
    const float max_stamina_second;

    float stamina = 0.0f;
    std::unique_ptr<Timer> timer = nullptr;
    std::unique_ptr<Sprite> spr_gauge = nullptr;
    std::unique_ptr<Sprite> spr_frame = nullptr;
};