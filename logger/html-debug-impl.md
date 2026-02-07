## HTML-impl

[//]: # (<span>)
    #include <stdio.h>  
    #include <stdarg.h>  
    #include <time.h>  
    #include <string.h>  
    // HTML color definitions
    #define HTML_RED(s)     "<span style='color:red;font-weight:bold'>" s "</span>"
    #define HTML_GREEN(s)   "<span style='color:green'>" s "</span>"
    #define HTML_YELLOW(s)  "<span style='color:orange'>" s "</span>"
    #define HTML_BLUE(s)    "<span style='color:blue'>" s "</span>"
    #define HTML_GRAY(s)    "<span style='color:gray'>" s "</span>"
    
    void html_log_init(FILE* f) {
        fprintf(f, "<!DOCTYPE html>\n");
        fprintf(f, "<html>\n<head>\n");
        fprintf(f, "<title>Debug Log</title>\n");
        fprintf(f, "<style>\n");
        fprintf(f, "body { font-family: monospace; background: #1e1e1e; color: #d4d4d4; }\n");
        fprintf(f, ".entry { margin: 2px 0; padding: 2px; }\n");
        fprintf(f, ".time { color: #888; }\n");
        fprintf(f, ".error { color: #f44747; font-weight: bold; }\n");
        fprintf(f, ".warn { color: #ff8800; }\n");
        fprintf(f, ".info { color: #4ec9b0; }\n");
        fprintf(f, ".debug { color: #569cd6; }\n");
        fprintf(f, "pre { margin: 0; }\n");
        fprintf(f, "</style>\n</head>\n<body>\n");
        fprintf(f, "<h2>Debug Log</h2>\n<div id='log'>\n");
    }
    
    void html_log_close(FILE* f) {
        fprintf(f, "</div>\n</body>\n</html>\n");
    }
    
    void html_log(FILE* f, const char* level, const char* format, ...) {
        // Get timestamp
        time_t now = time(NULL);
        struct tm* t = localtime(&now);
        char timestamp[64];
        strftime(timestamp, sizeof(timestamp), "%H:%M:%S", t);
        
        // Format message
        char message[1024];
        va_list args;
        va_start(args, format);
        vsnprintf(message, sizeof(message), format, args);
        va_end(args);
        
        // Escape HTML special characters
        char escaped[2048];
        char *d = escaped;
        const char *s = message;
        while (*s && (d - escaped) < sizeof(escaped) - 10) {
            switch (*s) {
                case '<': strcpy(d, "&lt;"); d += 4; break;
                case '>': strcpy(d, "&gt;"); d += 4; break;
                case '&': strcpy(d, "&amp;"); d += 5; break;
                case '\"': strcpy(d, "&quot;"); d += 6; break;
                case '\'': strcpy(d, "&#39;"); d += 5; break;
                default: *d++ = *s; break;
            }
            s++;
        }
        *d = '\0';
        
        // Write HTML entry
        fprintf(f, "<div class='entry %s'>\n", level);
        fprintf(f, "  <span class='time'>[%s]</span>\n", timestamp);
        fprintf(f, "  <span class='%s'>", level);
        fprintf(f, "%s", escaped);
        fprintf(f, "</span>\n</div>\n");
    }
    
    // Convenience wrappers
    void log_error(FILE* f, const char* format, ...) {
        va_list args;
        va_start(args, format);
        char buf[1024];
        vsnprintf(buf, sizeof(buf), format, args);
        va_end(args);
        html_log(f, "error", "%s", buf);
    }
    
    void log_warn(FILE* f, const char* format, ...) {
        va_list args;
        va_start(args, format);
        char buf[1024];
        vsnprintf(buf, sizeof(buf), format, args);
        va_end(args);
        html_log(f, "warn", "%s", buf);
    }
    
    void log_info(FILE* f, const char* format, ...) {
        va_list args;
        va_start(args, format);
        char buf[1024];
        vsnprintf(buf, sizeof(buf), format, args);
        va_end(args);
        html_log(f, "info", "%s", buf);
    }
    
    int main() {
        FILE* f = fopen("debug.html", "w");
        if (!f) return 1;
        
        html_log_init(f);
        
        // Log some messages
        log_error(f, "Failed to open file: config.txt");
        log_warn(f, "Memory usage at %d%%, consider cleanup", 85);
        log_info(f, "Started process PID: %d", 12345);
        log_info(f, "User <admin> logged in from 192.168.1.1");
        log_error(f, "Invalid XML: <tag attr=\"value\">text</tag>");
        
        html_log_close(f);
        fclose(f);
        
        printf("Log written to debug.html\n");
        printf("Open it in any browser!\n");
        
        return 0;
    }

[//]: # (</span>)

---
## finished HTML 

    <!DOCTYPE html>
    <html>
    <head>
    <title>Debug Log</title>
    <style>
    body { font-family: monospace; background: #1e1e1e; color: #d4d4d4; }
    .entry { margin: 2px 0; padding: 2px; }
    .time { color: #888; }
    .error { color: #f44747; font-weight: bold; }
    .warn { color: #ff8800; }
    .info { color: #4ec9b0; }
    .debug { color: #569cd6; }
    </style>
    </head>
    <body>
    <h2>Debug Log</h2>
    <div id='log'>
    <div class='entry error'>
      <span class='time'>[14:30:25]</span>
      <span class='error'>Failed to open file: config.txt</span>
    </div>
    <div class='entry warn'>
      <span class='time'>[14:30:25]</span>
      <span class='warn'>Memory usage at 85%, consider cleanup</span>
    </div>
    </div>
    </body>
    </html>






