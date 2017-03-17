#define PI 3.14159265359f   

float3 ComputeDiffuseBRDF(float3 albedo)
{
    return albedo / PI;
}

float3 ComputeSpecularBRDF(float3 specular, float3 roughness, float3 normal, float3 v, float3 l)
{
    // http://graphicrants.blogspot.com/2013/08/specular-brdf-reference.html
    // NDF(n) = GGX
    // G(l, v) = Smith's G1(l)G1(v), where G1 is Shlick-GGX
    // F = Shlick approximation of Fresnel
    // BRDF = (NDF * G * F) / ( 4 * (NoV)(NoL) )
    // result is simplified

    float3 h = normalize(v + l);
    float k = roughness / 2.0f;
    
    float3 fresnel = specular + (1.0f - specular) * (1.0f - pow(dot(v, h), 5.0f));
    float3 normalAndGeometry = (roughness * roughness) / ( 4 * (dot(normal, v) * (1 - k) + k) * (dot(normal, l) * (1 - k) + k) );
    return normalAndGeometry * fresnel;
}