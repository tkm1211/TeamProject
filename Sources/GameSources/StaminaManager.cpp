
#include "StaminaManager.h"
#include "FrameWork.h"


void StaminaManager::Init()
{
    // 実態の生成
    timer = std::make_unique<Timer>(true);
    spr_gauge = std::make_unique<Sprite>(L"Data/Assets/Texture/stamina.png");
    spr_frame = std::make_unique<Sprite>(L"Data/Assets/Texture/gage.png");

    // スタミナの設定
    stamina = max_stamina_second;

    // 初期化
    timer.get()->Init();
}

void StaminaManager::UnInit()
{
    timer.get()->UnInit();
}

void StaminaManager::Update()
{
    timer.get()->Update();

    CoolDown();
}

void StaminaManager::Render(const DirectX::XMFLOAT2 pos)
{
    constexpr float frame_width = 430.0f;
    constexpr float frame_height = 70.0f;

    spr_frame.get()->Draw2(
        pos.x, pos.y,
        frame_width, frame_height,
        0.0f, 0.0f,
        frame_width, frame_height,
        0,
        1, 1, 1, 1);


    constexpr float gauge_width = 430.0f;
    constexpr float gauge_height = 70.0f;
    float ratio = (stamina / max_stamina_second);
    float adjust = gauge_width * (1.0f - ratio) * 0.5f;

    spr_gauge.get()->Draw2(
        pos.x - adjust, pos.y,
        gauge_width * ratio, gauge_height,
        0.0f, 0.0f,
        gauge_width * ratio, gauge_height,
        0,
        1, 1, 1, 1);
}

bool StaminaManager::IsDash()
{
    float elapsedTime = FrameWork::GetInstance().GetElapsedTime();
    float now_stamina = stamina;

    now_stamina += -elapsedTime;

    return (now_stamina >= 0.0f);
}

void StaminaManager::Dash()
{
    if (stamina < zero_stamina) return;

    float elapsedTime = FrameWork::GetInstance().GetElapsedTime();
    stamina += -elapsedTime;

    // 1秒間はスタミナが回復しない
    timer.get()->SetTime(1);
}

void StaminaManager::Clear()
{
    timer.get()->Clear();
    stamina = max_stamina_second;
}

void StaminaManager::CoolDown()
{
    if (stamina == max_stamina_second) return;
    if (timer.get()->GetNowTime() > 0.0f)return;

    float elapsedTime = FrameWork::GetInstance().GetElapsedTime();
    stamina += elapsedTime;
    if (stamina >= max_stamina_second) Clear();
}