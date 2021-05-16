#ifndef __APP_HELPER
#define __APP_HELPER

#include <Arduino.h>
#include "main.h"

static unsigned long TimePassed() {
  return millis() - startupCompleteTime;
}

static bool HasPastEndTime( unsigned long startTime, unsigned long endTime ) {
  if( TimePassed() - startTime >= endTime ) {
    return true;
  }
  return false;
}

// Helper function to find how many occurrences
static int CharHowManyTimes( String sentence, String value ){
  int count = 0;
  int lastPosition = 0;
  do {
    lastPosition = sentence.indexOf( value, lastPosition );
    if( lastPosition != -1 ) {
      count++;
      lastPosition++;
    }
  } while( lastPosition != -1 && lastPosition <= sentence.length() );
  return count;
}

#endif
