#define JS_L_X 0
#define JS_L_Y 1
#define JS_L_B 52

#define JS_R_X 2
#define JS_R_Y 3
#define JS_R_B  53

#define FAST_TURN 0b11
#define MED_TURN  0b10
#define SLOW_TURN 0b01
#define NO_TURN   0b00
#define BACKWARD  0b000
#define FORWARD   0b100

#define alpha 0.25

#define LOW_BOUND      10
#define MED_LOW_BOUND  30
#define DEAD_ZONE_LOW  65
#define DEAD_ZONE_HIGH 110
#define MED_HIGH_BOUND 140
#define HIGH_BOUND     155

#define LEFT_JS_ID  0x00
#define RIGHT_JS_ID 0x80

typedef struct returnTuple {
  char turn_x_L;
  char turn_y_L;
  char turn_x_R;
  char turn_y_R;
} returnTuple;

void setupJoysticks();
char getTurnRate(int data);
struct returnTuple getTurnValues();
void createPackets(unsigned char* leftPacket, unsigned char* rightPacket);
char getButtonData(int button_id);
