#ifndef __STRUCTS
#define __STRUCTS

#include <Arduino.h>

struct BatteryStatus {
    float percentage;
    float voltage;
    int alertStatus;
};

// Joystick Location Struct
struct JoystickLocation {
  int DeadZone = 0;
  int Left = 1;
  int Right = 2;
  int Top = 3;
  int Bottom = 4;
  int TopLeft = 5;
  int BottomLeft = 6;
  int TopRight = 7;
  int BottomRight = 8;
};

// Joystick Zone Struct
struct JoystickZone {
  JoystickZone(){}
  JoystickZone( int x_max, int x_min, int y_max, int y_min, int& loc ) { // Custom constructor because referencing a value from JoystickLocation struct
    xMax = x_max;
    xMin = x_min;
    yMax = y_max;
    yMin = y_min;
    location = loc;
  }
  int xMax = 0;
  int xMin = 0;
  int yMax = 0;
  int yMin = 0;
  int location;
};

struct JoystickZoneConfig {
  // TODO Look at setting this up as an option through startup??? or some sort of config file through WiFi Connection
  // Setup X Zone Info
  int maxX;
  int minX;
  int deadZoneSizeX;
  int deadZoneMiddleX;

  // Setup Y Zone Info
  int maxY;
  int minY;
  int deadZoneSizeY;
  int deadZoneMiddleY;
};

// Commander Struct
struct Commander {
  String name;
  int commanderCastCount;
  String commanderCastCost;
};

// Game Type Struct
struct GameType {
  int Commander = 1;
  int Standard = 2;
  int Modern = 3;
  int Draft = 4;
};

#endif
