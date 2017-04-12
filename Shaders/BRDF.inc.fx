#define PI 3.14159265359f   

static const float Eps = 0.0000001f;

float3 ComputeDiffuseBRDF(float3 albedo)
{
    return albedo / PI;
}

float sqr(float x)
{
    return x * x;
}

float3 sqr(float3 x)
{
    return x * x;
}

float ClampDot(float3 lhs, float3 rhs)
{
    return max(dot(lhs, rhs), Eps);
}

float3 ComputeSpecularBRDF(float3 specular, float roughness, float3 normal, float3 v, float3 l)
{
    // http://graphicrants.blogspot.com/2013/08/specular-brdf-reference.html
    // NDF(n) = GGX
    // G(l, v) = Smith's G1(l)G1(v), where G1 is Shlick-GGX
    // F = Shlick approximation of Fresnel
    // BRDF = (NDF * G * F) / ( 4 * (NoV)(NoL) )
    // BRDF normalization term is simplified by geometry term

    float3 h = normalize(v + l);
    float k = roughness / 2.0f;
    float NoV = ClampDot(normal, v);
    float NoL = ClampDot(normal, l);
    
    float3 fresnel = specular + (1.0f - specular) * pow(1.0f - ClampDot(v, h), 5.0f);
    float3 geometry = rcp(4 * (NoV * (1 - k) + k) * (NoL * (1 - k) + k));
    
    float rSqr = sqr(roughness);
    float NoH = ClampDot(normal, h);
    float normalDistr = rSqr / ( PI * sqr(sqr(NoH) * (rSqr - 1) + 1) );

    return geometry * normalDistr * fresnel;
}