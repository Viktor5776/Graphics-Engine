#include "ShaderOps.hlsl"
#include "LightVectorData.hlsl"
#include "PointLight.hlsl"

cbuffer ObjectCBuf
{
    bool normalMapEnabled;
    bool specularMapEnabled;
    bool hasGloss;
    float specularPowerConst;
    float3 specularColor;
    float specularMapWeight;
};

Texture2D tex;
Texture2D spec;
Texture2D nmap;

SamplerState splr;

float4 main( float3 viewFragPos : Position, float3 viewNormal : Normal, float3 viewTan : Tangent, float3 viewBitan : Bitangent, float2 tc : Texcoord ) : SV_Target
{
    // sample diffuse texture
    float4 dtex = tex.Sample( splr, tc );
    
    #ifdef MASK_BOI
    // bail if highly translucent
    clip( dtex.a < 0.1f ? -1 : 1 );
    
    // flip normal when backface
    if ( dot( viewNormal, viewFragPos ) >= 0.0f )
    {
        viewNormal = -viewNormal;
    }
    #endif    

    // sample normal from map if normal mapping enabled
    viewNormal = normalize( viewNormal );
    if ( normalMapEnabled )
    {
        viewNormal = MapNormal( normalize( viewTan ), normalize( viewBitan ), normalize( viewNormal ), tc, nmap, splr );
    }
	// fragment to light vector data
    const LightVectorData lv = CalculateLightVectorData( viewLightPos, viewFragPos );
	
	// reflected light vector
	// calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
    float3 specularReflectionColor;
    float specularPower = specularPowerConst;
    if( specularMapEnabled )
    {
        const float4 specularSample = spec.Sample( splr, tc );
        specularReflectionColor = specularSample.rgb * specularMapWeight;  
        if ( hasGloss )
        {
            specularPower = pow( 2.0f, specularSample.a * 13.0f );
        }
    }
    else
    {
        specularReflectionColor = specularColor;
    }
    
    // attenuation
    const float att = Attenuate( attConst, attLin, attQuad, lv.distToL );
	// diffuse intensity
    const float3 diffuse = Diffuse( diffuseColor, diffuseIntensity, att, lv.dirToL, viewNormal );
    
    const float3 specularReflected = Speculate(
        specularReflectionColor, 1.0f, viewNormal,
        lv.vToL, viewFragPos, att, specularPower
    );
    
	// final color = attenuate diffuse & ambient by diffuse texture color and add specular reflected
    return float4( saturate( (diffuse + ambient) * dtex.rgb + specularReflected ), dtex.a );
}