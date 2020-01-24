
class PneumaticSystem
{
public:
  PneumaticSystem();

  void initialize(const int pump_pin, const int k1, const int k2, const int k3);
  void terminate();

  void activate_pump();
  void deactivate_pump();

  void activate_relay(const int relay_id);
  void deactivate_relay(const int relay_id);

  void deactivate_all();
  void activate_all();

private:
  int _k1, _k2, _k3;
  int _pump_pin;
};


PneumaticSystem::PneumaticSystem()
{
}

void PneumaticSystem::initialize(const int pump_pin, const int k1, const int k2, const int k3)
{
  _k1 = k1;
  _k2 = k2;
  _k3 = k3;
  _pump_pin = pump_pin;
  
  pinMode(_k1, OUTPUT);
  digitalWrite(_k1, HIGH); // turns it OFF

  pinMode(_k2, OUTPUT);
  digitalWrite(_k2, HIGH); // turns it OFF

  pinMode(_k3, OUTPUT);
  digitalWrite(_k3, HIGH); // turns it OFF

  pinMode(_pump_pin, OUTPUT);
  digitalWrite(_pump_pin, HIGH); // turns it OFF

  deactivate_all();
}

void PneumaticSystem::terminate()
{
  deactivate_all();
}

void PneumaticSystem::activate_pump()
{
  digitalWrite(_pump_pin, LOW); // turns it ON
}


void PneumaticSystem::deactivate_pump()
{
  digitalWrite(_pump_pin, HIGH); // turns it ON
}

void PneumaticSystem::activate_relay(const int relay_id)
{
  switch(relay_id)
  {
    case 0:
      digitalWrite(_k1, LOW); // turns it ON
      break;
    case 1:
      digitalWrite(_k2, LOW); // turns it ON
      break;
    case 2:
      digitalWrite(_k3, LOW); // turns it ON
      break;
  }
}

void PneumaticSystem::deactivate_relay(const int relay_id)
{
  switch(relay_id)
  {
    case 0:
      digitalWrite(_k1, HIGH); // turns it OFF
      break;
    case 1:
      digitalWrite(_k2, HIGH); // turns it OFF
      break;
    case 2:
      digitalWrite(_k3, HIGH); // turns it OFF
      break;
  }
}

void PneumaticSystem::deactivate_all()
{
  digitalWrite(_k1, HIGH); // turns it OFF
  digitalWrite(_k2, HIGH); // turns it OFF
  digitalWrite(_k3, HIGH); // turns it OFF
  digitalWrite(_pump_pin, HIGH); // turns it OFF
}

void PneumaticSystem::activate_all()
{
  digitalWrite(_k1, LOW); // turns it ON
  digitalWrite(_k2, LOW); // turns it ON
  digitalWrite(_k3, LOW); // turns it ON
  digitalWrite(_pump_pin, LOW); // turns it ON
}



