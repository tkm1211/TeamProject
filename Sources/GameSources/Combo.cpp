
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
    //spr_icon  = std::make_unique<Sprite>("");
    //spr_timer = std::make_unique<Sprite>("");
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

void Combo::Render()
{

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
