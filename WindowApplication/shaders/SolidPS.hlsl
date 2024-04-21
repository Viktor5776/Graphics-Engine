cbuffer CBuf : register(b1)
{
    float4 color;
};


float4 main() : SV_TARGET
{
    return float4( color );
}