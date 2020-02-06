struct actuator
{
  int actuatorid;
  int arduinoid;
  int value;
  char type[30];
  char arduinovalueid[5];
  char actuatorname[100];
};

const char ACTUATOR_FIELD_NAMES[][30] = {"actuatorid", "arduinoid", "value", "type", "arduinovalueid", "actuatorname"};

#define ACTUATOR_FIELDS 6
#define ACTUATOR_SIZE_UNIT 30
#define ACTUATOR_SIZE_ARDUINOVALUEID 5
#define ACTUATOR_SIZE_ACTUATORNAME 100
