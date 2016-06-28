#include "logger.h"

void Logger::Init(int level, long baud, int rxpin, int txpin) {
  _level = constrain(level, LOG_LEVEL_NOOUTPUT, LOG_LEVEL_VERBOSE);
  _baud = baud;
  _pinRx = rxpin;
  _pinTx = txpin;
  _serial = new SoftwareSerial(_pinTx, _pinRx);
  _serial->begin(_baud);
}

void Logger::Error(char const * msg, ...) {
  if (LOG_LEVEL_ERRORS <= _level) {
    print ("ERROR: ", 0);
    va_list args;
    va_start(args, msg);
    print(msg, args);
  }
}


void Logger::Info(char const * msg, ...) {
  if (LOG_LEVEL_INFOS <= _level) {
    va_list args;
    va_start(args, msg);
    print(msg, args);
  }
}

void Logger::Debug(char const * msg, ...) {
  if (LOG_LEVEL_DEBUG <= _level) {
    va_list args;
    va_start(args, msg);
    print(msg, args);
  }
}


void Logger::Verbose(char const * msg, ...) {
  if (LOG_LEVEL_VERBOSE <= _level) {
    va_list args;
    va_start(args, msg);
    print(msg, args);
  }
}



void Logger::print(const char *format, va_list args) {
  //
  // loop through format string
  for (; *format != 0; ++format) {
    if (*format == '%') {
      ++format;
      if (*format == '\0') break;
      if (*format == '%') {
        _serial->print(*format);
        continue;
      }
      if ( *format == 's' ) {
        register char *s = (char *)va_arg( args, int );
        _serial->print(s);
        continue;
      }
      if ( *format == 'd' || *format == 'i') {
        _serial->print(va_arg( args, int ), DEC);
        continue;
      }
      if ( *format == 'x' ) {
        _serial->print(va_arg( args, int ), HEX);
        continue;
      }
      if ( *format == 'X' ) {
        _serial->print("0x");
        _serial->print(va_arg( args, int ), HEX);
        continue;
      }
      if ( *format == 'b' ) {
        _serial->print(va_arg( args, int ), BIN);
        continue;
      }
      if ( *format == 'B' ) {
        _serial->print("0b");
        _serial->print(va_arg( args, int ), BIN);
        continue;
      }
      if ( *format == 'l' ) {
        _serial->print(va_arg( args, long ), DEC);
        continue;
      }

      if ( *format == 'c' ) {
        _serial->print(va_arg( args, int ));
        continue;
      }
      if ( *format == 't' ) {
        if (va_arg( args, int ) == 1) {
          _serial->print("T");
        }
        else {
          _serial->print("F");
        }
        continue;
      }
      if ( *format == 'T' ) {
        if (va_arg( args, int ) == 1) {
          _serial->print("true");
        }
        else {
          _serial->print("false");
        }
        continue;
      }

    }
    _serial->print(*format);

    _serial->println();
  }
}

Logger Log = Logger();






