
#include "Timer.h"
#include "FrameWork.h"

void Timer::Init()
{
    Clear();
    spr_timer = std::make_unique<Sprite>(L"Data/Assets/Texture/number.png");
}

void Timer::UnInit()
{
    Clear();
}

void Timer::Update()
{
    float elapsedTime = FrameWork::GetInstance().GetElapsedTime();
    time += elapsedTime;
}

void Timer::Render()
{
    if (!spr_timer.get()) return;


    float ones_place = 0.0f, tens_place = 0.0f, hundreds_place = 0.0f;
    SliceDigits(time, ones_place, tens_place, hundreds_place);

    // �ꕶ��������̏c�Ɖ��̕�
    constexpr float number_size_width = 80.0f;
    constexpr float number_size_height = 80.0f;

    spr_timer.get()->Draw2(
        0.0f + number_size_width * 2.0f + 40.0f, 40.0f,
        80, 80,
        0.0f + number_size_width * ones_place, 0.0f,
        number_size_width, number_size_height,
        0,
        1, 1, 1, 1);

    spr_timer.get()->Draw2(
        0.0f + number_size_width * 1.0f + 40.0f, 40.0f,
        80, 80,
        0.0f + number_size_width * tens_place, 0.0f,
        number_size_width, number_size_height,
        0,
        1, 1, 1, 1);

    spr_timer.get()->Draw2(
        0.0f + number_size_width * 0.0f + 40.0f, 40.0f,
        80, 80,
        0.0f + number_size_width * hundreds_place, 0.0f,
        number_size_width, number_size_height,
        0,
        1, 1, 1, 1);
}

void Timer::SliceDigits(float digits_/*���l*/, float& ones_place/*1�̈�*/, float& tens_place/*10�̈�*/, float& hundreds_place/*100�̈�*/)
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