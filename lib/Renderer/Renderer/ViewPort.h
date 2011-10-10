#pragma once



namespace GCL
{
  class ViewPort
  {
  public:
    ViewPort()
    : mWidth(640), mHeight(480) {}

    size_t GetWidth() const { return mWidth; }
    size_t GetHeight() const { return mHeight; }

  private:
    size_t mWidth;
    size_t mHeight;

  };

}
