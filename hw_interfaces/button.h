#pragma once

class Button
{
   public:
   Button();
   Button(int gpioButton);
   virtual ~Button();
   void Release();
   voif Init(int gpioButton);

protected:
   virtual void onButtonDown() {};
   virtual void onButtonUp() {};
   virtual void onDoubleClick() {};

private:
   void setGPIOs();
   void onEvent(int gpio, int level, uint32_t tick);
   static void _cback(int gpio, int level, uint32_t tick, void *user);

   int gpioBtn;
   bool initialized;
   uint32_t lastBtnDn;
   bool absorbe_up;
   uint32_t last_received;
};

class PulseButton : public Button
{
   public:
      PulseButton() {cur = false;}
      bool Down() const  {return cur; }

   private:
      virtual void onButtonDown() {cur=true; }
      virtual void onButtonUp()  {cur=false; }

   private:
      bool cur;
};