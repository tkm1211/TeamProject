
// ���͗p
struct InBufferType
{
	int i;
	float f;
};

// �o�͗p
struct OutBufferType
{
	int i;
};

// ���͗p�o�b�t�@�[
StructuredBuffer<InBufferType> InBuffer : register(t0);

// �o�͗p�o�b�t�@�[
RWStructuredBuffer<OutBufferType> OutBuffer : register(u0);