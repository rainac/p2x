// -*- c++ -*- 
// This file has been automatically generated by 
// gennc.sh $Id: gennc.xsl 50 2015-04-08 12:45:49Z jwillkomm $ 
// from definition file xml/logger.ncd.xml. Class mode is true.
static char const *getLogLevelName(unsigned long c) {
      switch(c) { 
       case IGNORE: return "IGNORE";
       case ERROR: return "ERROR";
       case WARNING: return "WARNING";
       case INFO: return "INFO";
       case DEBUG: return "DEBUG";
       case CONFIG: return "CONFIG";
       case PARSE: return "PARSE";
       case SCAN: return "SCAN";
       case TIMES: return "TIMES"; 
        default: break;
      }
      return "unknown value in enumeration LogLevel";
    }
    static char const *getLogLevelComment(unsigned long c) {
      switch(c) { 
       case IGNORE: return "";
       case ERROR: return "";
       case WARNING: return "";
       case INFO: return "";
       case DEBUG: return "";
       case CONFIG: return "";
       case PARSE: return "";
       case SCAN: return "";
       case TIMES: return ""; 
        default: break;
      }
      return "unknown value in enumeration LogLevel";
    }
    static LogLevel getLogLevelValue(char const *name, int *res) {
       if (res) *res = 0;
       if (name == 0 || *name == 0) { return IGNORE; 
       } else if (strcasecmp("IGNORE", name) == 0) {
          return IGNORE; 
       } else if (strcasecmp("ERROR", name) == 0) {
          return ERROR; 
       } else if (strcasecmp("WARNING", name) == 0) {
          return WARNING; 
       } else if (strcasecmp("INFO", name) == 0) {
          return INFO; 
       } else if (strcasecmp("DEBUG", name) == 0) {
          return DEBUG; 
       } else if (strcasecmp("CONFIG", name) == 0) {
          return CONFIG; 
       } else if (strcasecmp("PARSE", name) == 0) {
          return PARSE; 
       } else if (strcasecmp("SCAN", name) == 0) {
          return SCAN; 
       } else if (strcasecmp("TIMES", name) == 0) {
          return TIMES;  
       } else {
          if (! res) {
            fprintf(stderr, "error: unknown %s constant named `%s'\n",
              "LogLevel", name);
          } else {
             *res = 1;
          }
          return IGNORE;
       }
    }
    static unsigned long getNumLogLevel() {
      return 9;
    }
    static LogLevel getLogLevel(int which) {
      switch(which) { 
       case 0: return IGNORE;
       case 1: return ERROR;
       case 2: return WARNING;
       case 3: return INFO;
       case 4: return DEBUG;
       case 5: return CONFIG;
       case 6: return PARSE;
       case 7: return SCAN;
       case 8: return TIMES; 
       default: break;
      }
      fprintf(stderr, "error: LogLevel constant index %d out of range\n",
              which);
      return IGNORE;
    }
    
    inline static LogLevel getLogLevelValue(std::string const &t, int *res = 0) {
       return getLogLevelValue(t.c_str(), res);
    }
    
