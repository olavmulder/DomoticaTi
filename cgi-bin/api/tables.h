const char ACTUATOR_FIELD_NAMES[][30] = {"actuatorid", "arduinoid", "value", "type", "arduinovalueid", "actuatorname"};

#define ACTUATOR_FIELDS 6
#define ACTUATOR_FIELD_TYPE_SIZE 30
#define ACTUATOR_FIELD_ARDUINOVALUEID_SIZE 5
#define ACTUATOR_FIELD_ACTUATORNAME_SIZE 100

#define INIT_ACTUATOR_TABLE = "CREATE TABLE actuator \
(actuatorid int unsigned NOT NULL auto_increment, \
arduinoid int unsigned not null, \
value int, \
type varchar(30), \
arduinovalueid varchar(5) not null, \
actuatorname varchar(100) not null, \
PRIMARY KEY (actuatorid), \
FOREIGN KEY (arduinoid) REFERENCES arduino(arduinoid));"

struct actuator
{
  int actuatorid;
  int arduinoid;
  int value;
  char type[ACTUATOR_FIELD_TYPE_SIZE];
  char arduinovalueid[ACTUATOR_FIELD_ARDUINOVALUEID_SIZE];
  char actuatorname[ACTUATOR_FIELD_ACTUATORNAME_SIZE];
};