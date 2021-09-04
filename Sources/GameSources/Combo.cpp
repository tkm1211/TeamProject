
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

    // �R���{�̎������Ԃ̍X�V  (��)
    if (combo_duration_timer && is_combo)
    {
        combo_duration_timer += -elapsedTime;


        if (combo_duration_timer < 0.0f)
        {
            Prepare();
        }
    }

    // �ő�R���{���̋L�^
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

    // �ꕶ��������̏c�Ɖ��̕�
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

void Combo::SliceDigits(float digits_/*���l*/, float& ones_place/*1�̈�*/, float& tens_place/*10�̈�*/, float& hundreds_place/*100�̈�*/)
{
    /*

        ��
            "342"��100�̈ʂ̒l�����߂�����
            {
               342 * 0.01f = 3.42
               3.42�̏����_�ȉ���؂�̂Ă��
               3�@�ɂȂ�̂Ł@100�̈ʂ̒l�� "3"
            }


            "342"��10�̈ʂ����߂�����
            {
               (342 - 300) * 0.1f = 4.2
                        ...
                        ...
            }

               ~~~~~~~~ Tips : float�^�̕ϐ���int�^�̕ϐ��ɑ������� �����_�ȉ����؂�̂Ă���� ~~~~~~~~
    */

    // �ۑ� : 100�̈�,10�̈�,1�̈ʂ̒l�����ꂼ��A�����Ŏ����Ă���ones_place, tens_place, hundreds_place�ɑ������ //

    // 100�̈�
    int hundred = digits_ * 0.01f/*0.0f�������Ēl����������*/;
    hundreds_place = hundred;
    digits_ += -hundred * 100/*0.0f�������Ēl����������*/;

    // 10�̈�
    int ten = digits_ * 0.1f/*0.0f�������Ēl����������*/;
    tens_place = ten;
    digits_ += -tens_place * 10/*0.0f�������Ēl����������*/;

    // 1�̈�
    int one = digits_;
    ones_place = one;
    digits_ += -ones_place/*0.0f�������Ēl����������*/;


    if (static_cast<int>(digits_) <= 0) return;
    // 0�@�ȊO�̓G���[
    assert(false);
}