class Dimmer
{
private:
  int _brightnessMin = 0;
  int _brightnessMax = 250;
  int _fadeAmount = 25;  
  int _currentBrightness = _brightnessMin;
  void (*_callbackDimmerMin)(void);
  void (*_callbackDimmerMax)(void);
  void (*_callbackDimmerTick)(int);

public:
  Dimmer( void (*callbackDimmerMin)(void),
          void (*callbackDimmerMax)(void),
          void (*callbackDimmerTick)(int br),
          int fadeAmount = 25)
  {
    _callbackDimmerMin = callbackDimmerMin;
    _callbackDimmerMax = callbackDimmerMax;
    _callbackDimmerTick = callbackDimmerTick;
    _fadeAmount = fadeAmount;
  }

  void Tick()
  {
    _currentBrightness = _currentBrightness + _fadeAmount; 

    _callbackDimmerTick(_currentBrightness);

    if (_currentBrightness == _brightnessMax)  
    {
      _callbackDimmerMax();
    }
    else if (_currentBrightness == _brightnessMin)
    {
      _callbackDimmerMin();
    }

    if (_currentBrightness == 0 || _currentBrightness == 250)
    {
      _fadeAmount = -_fadeAmount;
      delay(250);
    }
    else
    {
      delay(10);
    }    
  }
};