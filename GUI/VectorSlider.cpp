#include "VectorSlider.h"
#include "Slider.h"

VectorSlider::VectorSlider(float x, float y, float w, float h, float minVal, float maxVal, float3 val)
{
    setPosition(x, y);
    setSize(w, h);

    static const float Padding = 10.0f;
    float elementWidth = (w - Padding * (SliderCount - 1)) / SliderCount;
    float currentX = 0.0f;
    for (uint i = 0; i < SliderCount; ++i)
    {
        _sliders[i].reset(new Slider(0, 0, elementWidth, h, minVal, maxVal, val[i]));
        _sliderXPos[i] = currentX;
        currentX += elementWidth + Padding;
    }
}

VectorSlider::~VectorSlider() = default;

float3 VectorSlider::getValue() const
{
    return float3(_sliders[SliderX]->getValue(), _sliders[SliderY]->getValue(), _sliders[SliderZ]->getValue());
}

void VectorSlider::draw(GraphicsDevice* gfxDevice, const FontID defaultFont, const SamplerStateID linearClamp, const BlendStateID blendSrcAlpha, const DepthStateID depthState)
{
    for (uint i = 0; i < SliderCount; ++i)
    {
        _sliders[i]->setPosition(xPos + _sliderXPos[i], yPos);
        _sliders[i]->draw(gfxDevice, defaultFont, linearClamp, blendSrcAlpha, depthState);
    }
}

bool VectorSlider::onMouseMove(const int x, const int y)
{
    for (uint i = 0; i < SliderCount; ++i)
    {
        if (_sliders[i]->isInWidget(x, y))
        {
            return _sliders[i]->onMouseMove(x, y);
        }
    }
    return false;
}

bool VectorSlider::onMouseButton(const int x, const int y, const MouseButton button, const bool pressed)
{
    for (uint i = 0; i < SliderCount; ++i)
    {
        if (_sliders[i]->isInWidget(x, y))
        {
            return _sliders[i]->onMouseButton(x, y, button, pressed);
        }
    }
    return true;
}
