const char ACTUATOR_FIELD_NAMES[][30] = {"actuatorid", "arduinoid", "value", "type", "arduinovalueid", "actuatorname"};

#define ACTUATOR_FIELDS 6
#define ACTUATOR_FIELD_TYPE_SIZE 30
#define ACTUATOR_FIELD_ARDUINOVALUEID_SIZE 5
#define ACTUATOR_FIELD_ACTUATORNAME_SIZE 100

struct actuator
{
  int actuatorid;
  int arduinoid;
  int value;
  char type[ACTUATOR_FIELD_TYPE_SIZE];
  char arduinovalueid[ACTUATOR_FIELD_ARDUINOVALUEID_SIZE];
  char actuatorname[ACTUATOR_FIELD_ACTUATORNAME_SIZE];
};