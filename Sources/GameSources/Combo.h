#pragma once

#include <memory>
#include "Sprite.h"
#include "Timer.h"


class Combo
{
public:
    Combo() {};
    ~Combo() {};

public:
    void Init();
    void UnInit();
    void Update();
    void Render();
    void ImGui();

public: // Get�֐�
    int GetCombo() { return combo; }
    int GetMaxCombo() { return max_comboed; }

public: // Set�֐�
    void Clear()
    {
        is_combo = false;
        combo = 0;
        max_comboed = FLT_MIN;
        combo_duration_timer = 0.0f;
    }
    void AddCombo()
    {
        is_combo = true;
        ++combo;
        combo_duration_timer = set_combo_duration_timer;
    }
private:
    void Prepare()  // �R���{�ꎞ�I�� �R���{���̃��Z�b�g
    {
        is_combo = false;
        combo = 0;
        combo_duration_timer = 0.0f;
    }

private:
    // �萔
    static constexpr float set_combo_duration_timer = 2.0f; // �A���R���{�̎�������

    // �ϐ�
    bool is_combo = false;                      // �R���{��
    int combo = 0;                              // ���݃R���{��
    int max_comboed = INT_MIN;                  // �ߋ��ő�R���{
    float combo_duration_timer = 0.0f;          // ���^�C�}�[
    std::unique_ptr<Timer> timer = nullptr;
    std::unique_ptr<Sprite> spr_icon = nullptr;
    std::unique_ptr<Sprite> spr_combo = nullptr;
};
