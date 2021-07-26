
// 入力用
struct InBufferType
{
	int i;
	float f;
};

// 出力用
struct OutBufferType
{
	int i;
};

// 入力用バッファー
StructuredBuffer<InBufferType> InBuffer : register(t0);

// 出力用バッファー
RWStructuredBuffer<OutBufferType> OutBuffer : register(u0);