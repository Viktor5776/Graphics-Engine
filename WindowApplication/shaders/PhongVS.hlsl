cbuffer CBuf
{
    matrix modelView;
    matrix modelViewProj;
};

struct VSOut
{
    float3 viewPos : Position;
    float3 normal : Normal;
    float2 tc : Texcoord;
    float4 pos : SV_Position;
};

VSOut main( float3 pos : Position, float3 n : Normal, float2 tc : Texcoord )
{
    VSOut vso;
    vso.viewPos = mul( float4( pos, 1.0f ), modelView ).xyz;
    vso.normal = mul( n, ( float3x3 ) modelView );
    vso.pos = mul( float4( pos, 1.0f ), modelViewProj );
    vso.tc = tc;
    return vso;
}