cbuffer ConstantBuffer:register(b0)
{
	float4x4 WorldViewProj;
	float    gTime;
}

struct VertexDataIn
{
	float3 Position : POSITION;
	float4 Color : COLOR;
};

struct VertexDataOut
{
	float4 PositionH : SV_POSITION;
	float4 Color : COLOR;
};

VertexDataOut VSMain(VertexDataIn Data)
{
	VertexDataOut Out;
	Out.PositionH=mul(float4(Data.Position,1), WorldViewProj);
	/*static float i=0.2;
	static int b = 1;
	Out.Position.x=i;
	Out.Position.y=i*b;
	Out.Position.z=0.5*i;
	Out.Position.w=1;
	i+=0.01;
	b=-b;*/
	Out.Color=Data.Color;
	return Out;
}

float4 PSMain(VertexDataOut Data):SV_Target
{
	return Data.Color;
}