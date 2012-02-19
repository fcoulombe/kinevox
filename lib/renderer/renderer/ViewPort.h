#pragma once



namespace GCL
{
  class ViewPort
  {
  public:
    ViewPort()
#ifdef OS_IPHONE
    : mWidth(320), mHeight(480) {}
#else
    : mWidth(640), mHeight(480) {}
#endif

    size_t GetWidth() const { return mWidth; }
    size_t GetHeight() const { return mHeight; }

  private:
    size_t mWidth;
    size_t mHeight;

  };

}
