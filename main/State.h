#ifndef __STATE
#define __STATE
class State {

public:
  State();

  virtual ~State();

  static State* Instance();

  virtual void DoInput();

  virtual void DoUpdate();

  virtual void DoRender();

  virtual void OnLostDevice();

  virtual void OnResetDevice();

  virtual void OnLoadState();

  virtual void OnLeaveState();
};

#endif // __STATE
