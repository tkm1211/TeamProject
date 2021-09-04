
#include "Combo.h"
#include "FrameWork.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <imgui_internal.h>


void Combo::Init()
{
    Clear();
    timer = std::make_unique<Timer>();
    spr_icon  = std::make_unique<Sprite>(L"Data/Assets/Texture/combo.png");
    spr_combo = std::make_unique<Sprite>(L"Data/Assets/Texture/number.png");
}

void Combo::Update()
{
    float elapsedTime = FrameWork::GetInstance().GetElapsedTime();

    //timer.get()->Update();

    // コンボの持続時間の更新  (仮)
    if (combo_duration_timer && is_combo)
    {
        combo_duration_timer += -elapsedTime;


        if (combo_duration_timer < 0.0f)
        {
            Prepare();
        }
    }

    // 最大コンボ数の記録
    if (combo > max_comboed) max_comboed = combo;
}

void Combo::Render(const DirectX::XMFLOAT2 pos)
{
    if (!is_combo)return;

    constexpr float icon_width = 320.0f;
    constexpr float icon_height = 64.0f;


    spr_icon.get()->Draw2(
        pos.x, pos.y,
        icon_width, icon_height,
        0.0f, 0.0f,
        icon_width, icon_height,
        0,
        1, 1, 1, 1);


    float ones_place = 0.0f, tens_place = 0.0f, hundreds_place = 0.0f;
    SliceDigits(combo, ones_place, tens_place, hundreds_place);

    // 一文字あたりの縦と横の幅
    constexpr float number_size_width = 80.0f;
    constexpr float number_size_height = 80.0f;

    spr_combo.get()->Draw2(
        pos.x + number_size_width * 2.0f + icon_width * 0.6f, pos.y - 10.0f,
        80, 80,
        0.0f + number_size_width * ones_place, 0.0f,
        number_size_width, number_size_height,
        0,
        1, 1, 1, 1);


    if (tens_place <= 0) return;

    spr_combo.get()->Draw2(
        pos.x + number_size_width * 1.0f + icon_width * 0.6f, pos.y - 10.0f,
        80, 80,
        0.0f + number_size_width * tens_place, 0.0f,
        number_size_width, number_size_height,
        0,
        1, 1, 1, 1);


    if (hundreds_place <= 0) return;

    spr_combo.get()->Draw2(
        pos.x + number_size_width * 0.0f + icon_width * 0.6f, pos.y - 10.0f,
        80, 80,
        0.0f + number_size_width * hundreds_place, 0.0f,
        number_size_width, number_size_height,
        0,
        1, 1, 1, 1);

}

void Combo::ImGui()
{
    ImGui::Text("is_combo : %d", is_combo);
    ImGui::Text("combo_duration_timer : %f", combo_duration_timer);
    ImGui::Text("combo : %d", combo);
    ImGui::Text("max_combo : %d", max_comboed);
}

void Combo::UnInit()
{
    Clear();
}

void Combo::SliceDigits(float digits_/*数値*/, float& ones_place/*1の位*/, float& tens_place/*10の位*/, float& hundreds_place/*100の位*/)
{
    /*

        例
            "342"の100の位の値を求めたい↓
            {
               342 * 0.01f = 3.42
               3.42の小数点以下を切り捨てると
               3　になるので　100の位の値は "3"
            }


            "342"の10の位を求めたい↓
            {
               (342 - 300) * 0.1f = 4.2
                        ...
                        ...
            }

               ~~~~~~~~ Tips : float型の変数をint型の変数に代入すると 小数点以下が切り捨てされる ~~~~~~~~
    */

    // 課題 : 100の位,10の位,1の位の値をそれぞれ、引数で持ってきたones_place, tens_place, hundreds_placeに代入する //

    // 100の位
    int hundred = digits_ * 0.01f/*0.0fを消して値を書き込む*/;
    hundreds_place = hundred;
    digits_ += -hundred * 100/*0.0fを消して値を書き込む*/;

    // 10の位
    int ten = digits_ * 0.1f/*0.0fを消して値を書き込む*/;
    tens_place = ten;
    digits_ += -tens_place * 10/*0.0fを消して値を書き込む*/;

    // 1の位
    int one = digits_;
    ones_place = one;
    digits_ += -ones_place/*0.0fを消して値を書き込む*/;


    if (static_cast<int>(digits_) <= 0) return;
    // 0　以外はエラー
    assert(false);
}