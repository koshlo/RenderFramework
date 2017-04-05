#define PI 3.14159265359f   

float2 Hammersley(uint index, uint numSamples, uint2 random)
{
    float e1 = frac((float) index / numSamples + float(random.x & 0xffff) / (1 << 16));
    float e2 = float(reversebits(index) ^ random.y) * 2.3283064365386963e-10;
    return float2(e1, e2);
}

float2 UniformGrid(uint index, uint width, uint height)
{
    float x = index % width;
    float y = index / height;
    return float2(x / (width-1), y / (height-1));
}

float3 TangentToWorld(float3 vec, float3 tangentZ)
{
    float3 up = abs(tangentZ.z) < 0.999 ? float3(0, 0, 1) : float3(1, 0, 0);
    float3 tangentX = normalize(cross(up, tangentZ));
    float3 tangentY = cross(tangentZ, tangentX);
    return tangentX * vec.x + tangentY * vec.y + tangentZ * vec.z;
}

float4 UniformSampleHemisphere(float2 e, out float cosTheta, out float sinTheta)
{
    float phi = 2 * PI * e.x;
    float theta = (PI / 2) * e.y;
    sincos(theta, sinTheta, cosTheta);

    float3 h;
    h.x = sinTheta * cos(phi);
    h.y = sinTheta * sin(phi);
    h.z = cosTheta;

    float pdf = 1.0 / (2 * PI);

    return float4(h, pdf);
}

float4 UniformSampleHemisphere(float2 e)
{
    float cosTheta, sinTheta;
    return UniformSampleHemisphere(e, cosTheta, sinTheta);
}

