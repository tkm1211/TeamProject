
#include "Timer.h"
#include "FrameWork.h"

void Timer::Init()
{
    // Tips : Init() = 初期化の時点でCountDown()するかCountUp()するかを設定できると Update()がごちゃごちゃにならずに済む
    Clear();
    spr_timer = std::make_unique<Sprite>(L"Data/Assets/Texture/number.png");
}

void Timer::UnInit()
{
    Clear();
}

void Timer::Update()
{

    // 詳細 : 「　elapsedTime とは　」
    /*

       ※ - 仮定,前提 -  ディスプレイが60Hzのもの ※


    　　( elapsed "経過" , Time "時間" )

     例 :
        1フレーム目は (elapsedTime = ) 前フレームから 0.02f 秒で実行！
        2フレーム目は (elapsedTime = ) 前フレームから 0.22f 秒で実行！
        3フレーム目は (elapsedTime = ) 前フレームから 0.09f 秒で実行！
                            ...
       60フレーム目は (elapsedTime = ) 前フレームから 0.15f 秒で実行！

        1 ~ 60フレーム でかかった合計の秒数(経過時間) = 1.00f 秒



                        ~~~　まとめ　~~~

        パソコンの状態によってフレームごとにかかった処理時間が変わってしまうが、
        elapsedTimeの値を使えばパソコンの状態に関わらずに
        60フレームで1秒と、均一に (今回なら時間を) 計算することができる。

                        ~~~~~~~~~~~~~~
    */

    // elapsedTime = １フレームあたりの秒数
    float elapsedTime = FrameWork::GetInstance().GetElapsedTime();


    // ↓　時間の更新　↓
    if (is_countdown/* == true */)
    {
        CountDown(elapsedTime);
    }

    else /* if (is_countdown == false) */
    {
        CountUp(elapsedTime);
    }

    // Tips :    上の処理と同じ (? = 三項演算子)
    // is_countdown ? CountDown() : CountUp();
}

void Timer::CountUp(float elapsedTime)
{
    //time++;
    time += elapsedTime;
}

void Timer::CountDown(float elapsedTime)
{
    //time--;
    time += -elapsedTime;

    if (time <= 0)
    {
        time = 0;
    }
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