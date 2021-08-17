
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

    // 一文字あたりの縦と横の幅
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

void Timer::SliceDigits(float digits_/*数値*/, float& ones_place/*1の位*/, float& tens_place/*10の位*/, float& hundreds_place/*100の位*/)
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