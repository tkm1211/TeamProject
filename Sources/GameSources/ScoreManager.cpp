
#include "ScoreManager.h"


void ScoreManager::Init()
{
    spr_score = std::make_unique<Sprite>(L"Data/Assets/Texture/number.png");

    Clear();
}

void ScoreManager::UnInit()
{
    Clear();
}

void ScoreManager::Update()
{

}

void ScoreManager::Render(const DirectX::XMFLOAT2 pos)
{
    float ones_place = 0.0f, tens_place = 0.0f, hundreds_place = 0.0f, thousands_place = 0.0f;
    SliceDigits(scored, ones_place, tens_place, hundreds_place, thousands_place);

    // 一文字あたりの縦と横の幅
    constexpr float number_size_width = 80.0f;
    constexpr float number_size_height = 80.0f;

    spr_score.get()->Draw2(
        pos.x + number_size_width * 3.0f + 40.0f, pos.y,
        80, 80,
        0.0f + number_size_width * thousands_place, 0.0f,
        number_size_width, number_size_height,
        0,
        1, 1, 1, 1);

    spr_score.get()->Draw2(
        pos.x + number_size_width * 2.0f + 40.0f, pos.y,
        80, 80,
        0.0f + number_size_width * ones_place, 0.0f,
        number_size_width, number_size_height,
        0,
        1, 1, 1, 1);

    spr_score.get()->Draw2(
        pos.x + number_size_width * 1.0f + 40.0f, pos.y,
        80, 80,
        0.0f + number_size_width * tens_place, 0.0f,
        number_size_width, number_size_height,
        0,
        1, 1, 1, 1);

    spr_score.get()->Draw2(
        pos.x + number_size_width * 0.0f + 40.0f, pos.y,
        80, 80,
        0.0f + number_size_width * hundreds_place, 0.0f,
        number_size_width, number_size_height,
        0,
        1, 1, 1, 1);
}

void ScoreManager::SliceDigits(float digits_, float& ones_place, float& tens_place, float& hundreds_place, float& thousands_place)
{
    // 1000の位
    int thousand = digits_ * 0.001f;
    thousands_place = thousand;
    digits_ += -thousands_place * 1000;

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