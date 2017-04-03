#define PI 3.14159265359f   

float2 Hammersley(uint index, uint numSamples, uint2 random)
{
    float e1 = frac((float) index / numSamples + float(random.x & 0xffff) / (1 << 16));
    float e2 = float(reversebits(index) ^ random.y) * 2.3283064365386963e-10;
    return float2(e1, e2);
}

float4 UniformSampleHemisphere(float2 e)
{
    float phi = 2 * PI * e.x;
    float cosTheta = e.y;
    float sinTheta = sqrt(1 - cosTheta * cosTheta);

    float3 h;
    h.x = sinTheta * cos(phi);
    h.y = sinTheta * sin(phi);
    h.z = cosTheta;

    float pdf = 1.0 / (2 * PI);

    return float4(h, pdf);
}
