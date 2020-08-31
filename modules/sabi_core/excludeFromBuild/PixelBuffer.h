#pragma once

using PixelBufferHandle = std::shared_ptr<struct PixelBuffer>;

struct PixelBuffer : public HasId
{
    static PixelBufferHandle createHandle() { return std::make_shared<PixelBuffer>(); }

    PixelBuffer()
    {
        // default is uint8Pixels
        spec.width = DEFAULT_DESKTOP_WINDOW_WIDTH;
        spec.height = DEFAULT_DESKTOP_WINDOW_HEIGHT;
        spec.channels = DEFAULT_DESKTOP_WINDOW_CHANNELS;

        uint8Pixels.resize (spec.channels, getPixelCount());

        // just make it black
        std::memset (uint8Pixels.data(), 0, byteCountUint8());
    }

    PixelBuffer (const ImageInfo& spec)
    {
        init (spec);
    }

    ~PixelBuffer()
    {
        //LOG(DBUG) << _FN_;
        reset();
    }

    const ImageID getID() const { return id(); }

    void init (const ImageInfo& info, bool makeBlack = true)
    {
        spec = info;

        if (spec.dataType == ImageDataType::UINT8_DATA)
        {
            uint8Pixels.resize (spec.channels, getPixelCount());

            if (makeBlack)
                std::memset (uint8Pixels.data(), 0, byteCountUint8());

            floatPixels.resize (0, 0);
        }
        else if (spec.dataType == ImageDataType::FLOAT_DATA)
        {
            floatPixels.resize (spec.channels, getPixelCount());

            if (makeBlack)
                std::memset (floatPixels.data(), 0.0f, bytecountFloat());

            uint8Pixels.resize (0, 0);
        }
    }

    int getPixelCount() const { return spec.width * spec.height; }
    int byteCountUint8() const { return spec.width * spec.height * spec.channels * sizeof (uint8_t); }
    int bytecountFloat() const { return spec.width * spec.height * spec.channels * sizeof (float); }

    void flipVertical (MatrixXc& flipped)
    {
        if (!uint8Pixels.data()) return;

        flipped.resize (spec.channels, getPixelCount());

        // from polymer https ://github.com/ddiakopoulos/polymer/blob/dev/lib-engine/gfx/gl/glfw-app.cpp
        for (int y = 0; y < spec.height; ++y)
        {
            memcpy (flipped.data() + y * spec.width * 4, uint8Pixels.data() + (spec.height - y - 1) * spec.width * 4, spec.width * 4);
        }
    }

    void flipVertical (MatrixXf& flipped)
    {
        if (!floatPixels.data()) return;

        flipped.resize (spec.channels, getPixelCount());

        // from polymer https ://github.com/ddiakopoulos/polymer/blob/dev/lib-engine/gfx/gl/glfw-app.cpp
        for (int y = 0; y < spec.height; ++y)
        {
            memcpy (flipped.data() + y * spec.width * 16, floatPixels.data() + (spec.height - y - 1) * spec.width * 16, spec.width * 16);
        }
    }

    // move only!
    PixelBuffer (const PixelBuffer& other) = delete;
    PixelBuffer& operator= (const PixelBuffer& other) = delete;

    PixelBuffer (PixelBuffer&& other) noexcept
    {
        spec = std::move (other.spec);
        uint8Pixels = std::move (other.uint8Pixels);
        floatPixels = std::move (other.floatPixels);

        other.reset();
    }

    void PixelBuffer::swap (PixelBuffer& other)
    {
        std::swap (spec, other.spec);
        std::swap (uint8Pixels, other.uint8Pixels);
        std::swap (floatPixels, other.floatPixels);
    }

    PixelBuffer& operator= (const PixelBuffer&& other)
    {
        spec = std::move (other.spec);
        uint8Pixels = std::move (other.uint8Pixels);
        floatPixels = std::move (other.floatPixels);

        return *this;
    }

    void reset()
    {
        uint8Pixels.resize (0, 0);
        floatPixels.resize (0, 0);
    }

    void resize (float w, float h)
    {
        int width = static_cast<int> (w);
        int height = static_cast<int> (h);

        // nothing changed then nothing to do
        if (spec.width == width && spec.height == height) return;

        reset();
        ImageInfo newSpec = spec;
        newSpec.width = (int)w;
        newSpec.height = (int)h;
        newSpec.dirty = true;
        init (newSpec);
    }

    ImageInfo spec;
    MatrixXc uint8Pixels;
    MatrixXf floatPixels;
};

using Images = std::vector<PixelBufferHandle>;
using ImageDB = std::unordered_map<ImageID, PixelBufferHandle>;