#pragma once

#include <memory>

#include "Widget.h"

class Slider;
typedef std::unique_ptr<Slider> SliderPtr;

class VectorSlider : public Widget
{
public:
    VectorSlider(float x, float y, float w, float h, float minVal = 0, float maxVal = 1, float3 val = 0);
    ~VectorSlider();

    float3 getValue() const;

    void draw(GraphicsDevice *gfxDevice, const FontID defaultFont, const SamplerStateID linearClamp,
        const BlendStateID blendSrcAlpha, const DepthStateID depthState) override;
    
    bool onMouseMove(const int x, const int y) override;
    bool onMouseButton(const int x, const int y, const MouseButton button, const bool pressed) override;

private:
    enum Sliders
    {
        SliderX,
        SliderY,
        SliderZ,

        SliderCount
    };
    SliderPtr _sliders[SliderCount];
    float _sliderXPos[SliderCount];
};