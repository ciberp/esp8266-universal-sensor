    #include <ESP8266WiFi.h>
    #include <WiFiClient.h>
    #include <ESP8266WebServer.h>
    #include <EEPROM.h>
    #include <Servo.h>
    #include <OneWire.h>
    #include <DallasTemperature.h>
    #include <BME280I2C.h>
    #include <Wire.h>
    #include "DHT.h"
    #include <WiFiUdp.h>
    #include <ntp.h>
    #include <Time.h>
    #include <ESP8266HTTPClient.h>
    #include <NewPing.h>
    #include <MQTTClient.h>
    #include "EspSaveCrash.h"
    
    static const char CSS[] PROGMEM =
    "<style>"
    "@import url(http://fonts.googleapis.com/css?family=Roboto:400,500,700,300,100);"
    "html {"
    "font-family: \"Roboto\", helvetica, arial, sans-serif;"
    "text-rendering: optimizeLegibility;"
    "font-weight: 400 !important;"
    "font-size: 100% !important;"
    "}"
    ".f9{"
    "max-width: 95%;"
    "background: #FAFAFA;"
    "padding: 10px;"
    "margin: 10px auto;"
    "box-shadow: 1px 1px 25px rgba(0, 0, 0, 0.35);"
    "border-radius: 10px;"
    "border: 3px solid #305A72;"
    "}"
    ".set a:link, a:visited {"
    "position: absolute;"
    "left: 96%;"
    "top: 2%;"
    "font-size: 150%;"
    "text-decoration: none;"
    "color: #305A72;"
    "}"
    ".f9 ul{"
    "padding:0;"
    "margin:0;"
    "list-style:none;"
    "}"
    ".f9 ul li{"
    "display: block;"
    "margin-bottom: 10px;"
    "min-height: 35px;"
    "}"
    ".f9 ul li  .field-style{"
    "box-sizing: border-box;"
    "-webkit-box-sizing: border-box;"
    "-moz-box-sizing: border-box;"
    "padding: 8px;"
    "outline: none;"
    "border: 1px solid #B0CFE0;"
    "}.f9 ul li  .field-style:focus{"
    "box-shadow: 0 0 5px #B0CFE0;"
    "border:1px solid #B0CFE0;"
    "}"
    ".f9 ul li .field-full{"
    "width: 100%;"
    "}"
    ".f9 ul li .field-split{"
    "font-size: 100%;"
    "}"
    ".f9 ul li input.align-left{"
    "float:left;"
    "}"
    ".f9 ul li input.align-right{"
    "float:right;"
    "}"
    ".f9 ul li textarea{"
    "width: 100%;"
    "height: 100px;"
    "}"
    ".g {"
    "-moz-box-shadow: inset 0px 1px 0px 0px #3985B1;"
    "-webkit-box-shadow: inset 0px 1px 0px 0px #3985B1;"
    "box-shadow: inset 0px 1px 0px 0px #3985B1;"
    "background-color: #216288;"
    "border: 1px solid #17445E;"
    "display: inline-block;"
    "cursor: pointer;"
    "color: #FFFFFF;"
    "padding: 8px 18px;"
    "text-decoration: none;"
    "font: 12px Arial, Helvetica, sans-serif;"
    "width: 33%;"
    "height: 8%;"
    "margin: 2px;"
    "}"
    "#rtdiv {"
    "width: 100%;"
    "}"
    ".cloud {"
    "background: #F1F1F1;"
    "border-radius: 0.4em;"
    "-moz-border-radius: 0.4em;"
    "-webkit-border-radius: 0.4em;"
    "color: #333;"
    "display: inline-block;"
    "margin-right: 5px;"
    "text-align: center;"
    "margin-bottom: 10px;"
    "border:1px solid #B0CFE0;"
    "padding: 3px 3px;"
    "min-width: 10.1%; "
    "}"
    ".f9 ul li input[type=\"submit\"], {"
    "-moz-box-shadow: inset 0px 1px 0px 0px #3985B1;"
    "-webkit-box-shadow: inset 0px 1px 0px 0px #3985B1;"
    "box-shadow: inset 0px 1px 0px 0px #3985B1;"
    "background-color: #216288;"
    "border: 1px solid #17445E;"
    "display: inline-block;"
    "cursor: pointer;"
    "color: #FFFFFF;"
    "padding: 8px 18px;"
    "text-decoration: none;"
    "font: 12px Arial, Helvetica, sans-serif;"
    "font-size: 250% !important;"
    "}"
    ".f9 ul li input[type=\"button\"]:hover,"
    ".f9 ul li input[type=\"submit\"]:hover {"
    "background: linear-gradient(to bottom, #2D77A2 5%, #337DA8 100%);"
    "background-color: #28739E;"
    "}"
    "input[type=\"text\"], select, .d {"
    "font-size: 100% !important;"
    "}"
    "table {"
    "border:1px solid #B0CFE0;"
    "color: #333;"
    "width: 100%;"
    "margin-bottom: 10px;"
    "font-size: 100% !important;"
    "}"
    "td, th {"
    "border: 1px solid transparent;"
    "height: 8%; "
    "}"
    "th {"
    "font-weight: bold;"
    "}"
    "td {"
    "background: #FAFAFA;"
    "text-align: center;"
    "}"
    "th {"
    "background: #DFDFDF;"
    "font-weight: bold;"
    "}"
    "td {  "
    "background: #FAFAFA;"
    "text-align: center;"
    "}"
    "@media screen and (max-width: 999px) {"
    "html {"
    "font-size: 250% !important;"
    "}"
    ".f9 ul li .field-split{"
    "font-size: 250% !important;"
    "}"
    ".g {"
    "font-size: 66% !important;"
    "width: 32.5% !important;"
    "height: 150px !important;"
    "}"
    ".cloud {"
    "min-width: 31.8% !important; "
    "}"
    "table {"
    "font-size: 40% !important;"
    "}"
    "}"
    "</style>";

    //static const char static_html_handle_show[] PROGMEM =
    String  static_html_handle_show =
    "</head><body class=\"f9\">"
    "<script type=\"text/javascript\">function loadContent(){var xmlhttp;if (window.XMLHttpRequest){xmlhttp = new XMLHttpRequest();}xmlhttp.onreadystatechange = function(){\n"
    "if (xmlhttp.readyState == XMLHttpRequest.DONE ){if(xmlhttp.status == 200){document.getElementById(\"shdiv\").innerHTML = xmlhttp.responseText;\n"
    "setTimeout(loadContent, 200);}}}\n"
    "xmlhttp.open(\"GET\", \"/values\", true); xmlhttp.send(); } loadContent(); </script>\n"
    "<div id=\"shdiv\">Checking ...</div><br>"
    "<a href=\"/\"><button class=\"g\">NAZAJ</button></a>"
    "</body></html>\n";

    //static const char static_html_handle_root[] PROGMEM =
    String static_html_handle_root =
    "</head><body class=\"f9\">"
    "<script type=\"text/javascript\">function loadContent(){var xmlhttp;if (window.XMLHttpRequest){xmlhttp = new XMLHttpRequest();}xmlhttp.onreadystatechange = function(){\n"
    "if (xmlhttp.readyState == XMLHttpRequest.DONE ){if(xmlhttp.status == 200){document.getElementById(\"rtdiv\").innerHTML = xmlhttp.responseText;\n"
    "setTimeout(loadContent, 200);}}}\n"
    "xmlhttp.open(\"GET\", \"/values\", true); xmlhttp.send(); } loadContent(); </script>\n"
    "<span id=\"rtdiv\">Checking ...</span><br>";


    String data_collector_type_1 = "influxdb_tcp";
    String data_collector_type_2 = "carbon_udp";
    
//        REASON_DEFAULT_RST      = 0,   /* normal startup by power on */
//        REASON_WDT_RST         = 1,   /* hardware watch dog reset */
//        REASON_EXCEPTION_RST   = 2,   /* exception reset, GPIO status won’t change */
//        REASON_SOFT_WDT_RST      = 3,   /* software watch dog reset, GPIO status won’t change */
//        REASON_SOFT_RESTART    = 4,   /* software restart ,system_restart , GPIO status won’t change */
//        REASON_DEEP_SLEEP_AWAKE   = 5,   /* wake up from deep-sleep */
//        REASON_EXT_SYS_RST      = 6      /* external system reset */ - See more at: http://www.esp8266.com/viewtopic.php?f=32&t=13388#sthash.Pi59f7lq.dpuf 
    
    WiFiUDP udp;
    unsigned int localPort = 2390;
    // default IP, povozijo ga nastavitve iz EEPROM-a
    IPAddress syslogServer(0, 0, 0, 0);
    String Version = "20180302";

    // ntp
    time_t getNTPtime(void);
    NTP NTPclient;
    //#define GMT +0 //timezone
        
    ADC_MODE(ADC_VCC);
    unsigned long wifi_connect_previous_millis = 0;
    
    ESP8266WebServer server(80);
    const char* serverIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";

    // Tell it where to store your config data in EEPROM
    #define EEPROM_START 0     // začetek  
    #define EEPROM_SIZE  3071  // velikost za EPROM

    // Sizes
    #define StringLenghtNormal 25
    #define StringLenghtURL 100
    #define StringLenghtIP 15
    #define DS18B20_Max_Sensors 10
    #define DHT_Max_Sensors 10
    #define BME280_Max_Sensors 5
    #define MAX6675_Max_Sensors 5
    #define IO_Max 10
    #define Ultrasonic_Max_Sensors 5
    #define data_collector_Max_Clients 5
    
    #define Minimum_update_interval 5 // http post & domoticz update & MQTT & influxDB
    #define Not_used_pin_number 100
    #define LoadDefaultNumber 2018
    
    typedef struct {
      // wifi
      char ssid[StringLenghtNormal];
      char password[StringLenghtNormal];
      char ap_ssid[StringLenghtNormal];
      char ap_password[StringLenghtNormal];
      char css_link[StringLenghtURL];
      char syslog[StringLenghtIP];
      char NTP_hostname[StringLenghtNormal];
      int  timezone_GMT;
      int defaults;
      char hostname[StringLenghtNormal];
      bool DS18B20_enabled;
      int DS18B20_pin;
      int DS18B20_num_configured;  // število skonfiguriranih
      byte DS18B20_address[DS18B20_Max_Sensors][8]; // support up to 10 sensors addr!!!
      char DS18B20_ident[DS18B20_Max_Sensors][StringLenghtNormal];  // support up to 10 sensors names!!!
      int DS18B20_resolution[DS18B20_Max_Sensors];
      int DS18B20_domoticz_idx[DS18B20_Max_Sensors];
      int DHT_num_configured;      // število skonfiguriranih
      int DHT_pin[DHT_Max_Sensors];             // support up to 10 sensors !!!
      int DHT_type[DHT_Max_Sensors];            // DHT type => 11 = DHT11, 22 = DHT22, 21 = DHT21/AM2301
      char DHT_ident[DHT_Max_Sensors][StringLenghtNormal];      // support up to 10 sensors names!!!
      int DHT_domoticz_idx[DHT_Max_Sensors];
      int BME280_num_configured;     // število skonfiguriranih
      int BME280_pin_sda[BME280_Max_Sensors];         // 
      int BME280_pin_scl[BME280_Max_Sensors];         // 
      char BME280_ident[BME280_Max_Sensors][StringLenghtNormal];      // support up to 5 sensors names!!!
      int BME280_domoticz_idx[BME280_Max_Sensors];
      int MAX6675_num_configured;     // število skonfiguriranih
      int MAX6675_pin_clk[MAX6675_Max_Sensors];         // 
      int MAX6675_pin_cs[MAX6675_Max_Sensors];
      int MAX6675_pin_do[MAX6675_Max_Sensors];
      char MAX6675_ident[MAX6675_Max_Sensors][StringLenghtNormal];      // support up to 5 sensors names!!!
      int MAX6675_domoticz_idx[MAX6675_Max_Sensors];
      boolean domoticz_update_enabled;                 
      int domoticz_update_interval;
      char domoticz_ip[StringLenghtIP];
      int domoticz_port;
      int domoticz_idx_voltage;
      int domoticz_idx_uptime;
      int domoticz_idx_wifi;
      int IO_num_configured;
      char IO_ident[IO_Max][StringLenghtNormal];
      int IO_pin[IO_Max];
      int IO_mode[IO_Max];
      float IO_timer[IO_Max]; // in seconds
      int IO_domoticz_idx[IO_Max];
      int Ultrasonic_num_configured;     // število skonfiguriranih
      int Ultrasonic_pin_trigger[Ultrasonic_Max_Sensors];         // 
      int Ultrasonic_pin_echo[Ultrasonic_Max_Sensors];
      int Ultrasonic_max_distance[Ultrasonic_Max_Sensors];
      char Ultrasonic_ident[Ultrasonic_Max_Sensors][StringLenghtNormal];      // support up to 5 sensors names!!!
      int Ultrasonic_domoticz_idx[Ultrasonic_Max_Sensors];
      int Ultrasonic_offset[Ultrasonic_Max_Sensors];
      boolean mqtt_enabled; 
      char mqtt_server[StringLenghtURL];
      int mqtt_port;
      int mqtt_interval;
      char mqtt_key[StringLenghtNormal];
      char mqtt_secret[StringLenghtNormal];
      int data_collector_num_configured;
      char data_collector_host[data_collector_Max_Clients][StringLenghtNormal];
      int data_collector_port[data_collector_Max_Clients];
      char data_collector_db_name[data_collector_Max_Clients][StringLenghtNormal];
      char data_collector_db_username[data_collector_Max_Clients][StringLenghtNormal];
      char data_collector_db_password[data_collector_Max_Clients][StringLenghtNormal];
      int data_collector_push_interval[data_collector_Max_Clients];
      int data_collector_type[data_collector_Max_Clients];
      
    } ObjectSettings;
    ObjectSettings SETTINGS;

    // HTML back url for root and settings
    String BackURLSystemSettings="<meta http-equiv=\"refresh\" content=\"0;url=/settings\">";
    //String BackURLShow="<meta http-equiv=\"refresh\" content=\"0;url=/show\">";
    String BackURLRoot = "<meta http-equiv=\"refresh\" content=\"0;url=/\">";
    
    // main loop interval
    unsigned long previousMillis = 0;
    unsigned long previousMillisMinis = 0;
    unsigned long previousHttpPost = 0;
    unsigned long previousDomoticzUpdate = 0;
    unsigned long previousMQTTReconnect = 0;
    unsigned long previousMQTTInterval = 0;
    unsigned long previousNTPReconnect = 0;
    unsigned long previousDataCollector[data_collector_Max_Clients];

    // DS18B20 Temperature sensors
    OneWire oneWire(Not_used_pin_number); // za default pin nastavim 100, in ga potem prenastavim
    DallasTemperature DS18B20(&oneWire);
    byte DS18B20_All_Sensor_Addresses[DS18B20_Max_Sensors][8]; // support up to 30 sensors!!!
    float DS18B20_All_Sensor_Values[DS18B20_Max_Sensors];      // support up to 30 sensors!!!
    int DS18B20_Count = 0;

    // DHT Temperature & humidity sensors
    float DHT_All_Sensor_Values_Hum[DHT_Max_Sensors], DHT_All_Sensor_Values_Temp[DHT_Max_Sensors], DHT_All_Sensor_Values_Heat[DHT_Max_Sensors], DHT_All_Sensor_Values_Dew[DHT_Max_Sensors];
    DHT dht(101, 22); // za default pin nastavim 101 in sensor 22, in ga potem prenastavim

    // BME280 
    BME280I2C BME280;
    float BME280_All_Sensor_Values_Temp[BME280_Max_Sensors], BME280_All_Sensor_Values_Hum[BME280_Max_Sensors], BME280_All_Sensor_Values_Press[BME280_Max_Sensors], BME280_All_Sensor_Values_Heat[BME280_Max_Sensors], BME280_All_Sensor_Values_Dew[BME280_Max_Sensors], BME280_All_Sensor_Values_Alt[BME280_Max_Sensors];

    // MAX6675
    float MAX6675_All_Sensor_Values_Temp[MAX6675_Max_Sensors];

    // IO pins
    int IO_All_Write[IO_Max];
    unsigned long previousIOmillis[IO_Max];
    #define pushbutton_set_to_low_after 300 // in msec

    // Ultrasonic sensor
    int Ultrasonic_All_Sensor_Values_Distance[Ultrasonic_Max_Sensors];
    

    //mqtt
    WiFiClient MQTT_Client;        // http
    MQTTClient mqtt_client;
    
    // pins, 
    // ! pazi ob boot-u, normal mode: D8=LOW, D4=HIGH, D3=HIGH  
    // D0  = 16  = PWM Motor 3x
    // D1  =  5  = BME280 (I2C SCL)
    // D2  =  4  = BME280 (I2C SDA)
    // D3! =  0  = PWM Motor 1x
    // D4! =  2  = Servo Inner
    // D5  = 14  = DS18B20
    // D6  = 12  = Rele 1
    // D7  = 13  = Servo Ext
    // D8! = 15  = Rele 2
    // D9  =  3  = DHT22 1
    // D10 =  1  = DHT22 2

    // time
    long days = 0;
    long hours = 0;
    long mins = 0;
    unsigned long secs = 0;



    void setup(void)
    {
      Serial.begin(115200);
      loadConfig();
      if (SETTINGS.defaults != LoadDefaultNumber) {
        handle_load_defaults();
      }
      ConnectToWifi();
      udp.begin(localPort);
      server.on("/espupdate", HTTP_GET, [](){
        //server.sendHeader("Connection", "close");
        //server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(200, "text/html", serverIndex);
      });
      server.on("/update", HTTP_POST, [](){
        server.sendHeader("Connection", "close");
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(200, "text/plain", (Update.hasError())?"FAIL":"OK");
        ESP.restart();
      },[](){
        HTTPUpload& upload = server.upload();
        if(upload.status == UPLOAD_FILE_START){
          //Serial.setDebugOutput(true);
          //WiFiUDP::stopAll();
          //Serial.printf("Update: %s\n", upload.filename.c_str());
          String TempString = " UPDATE: ESPupdate, uploading file ";
          TempString += upload.filename.c_str();
          SendLog(TempString);
          uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
          if(!Update.begin(maxSketchSpace)){//start with max available size
            //Update.printError(Serial);
            SendLog(" UPDATE: Error no space...");
          }
        } else if(upload.status == UPLOAD_FILE_WRITE){
          if(Update.write(upload.buf, upload.currentSize) != upload.currentSize){
            //Update.printError(Serial);
            SendLog(" UPDATE: Error upload file write...");
          }
        } else if(upload.status == UPLOAD_FILE_END){
          if(Update.end(true)){ //true to set the size to the current progress
            //Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
            SendLog(" UPDATE: Success, Rebooting... ");
          } else {
            //Update.printError(Serial);
            SendLog(" UPDATE: Error upload file end...");
          }
          //Serial.setDebugOutput(false);
        }
        yield();
      });
      server.begin();
      server.on("/", handle_root);
      server.on("/scan", http_WiFi_Scan);
      server.on("/stat", http_Status);
      server.on("/clearcrash", http_clear_crash_info);
      server.on("/crashinfo", http_crash_info);
      server.on("/ntpupdate", init_ntp);
      server.on("/ultadd", []() {
        if (SETTINGS.Ultrasonic_num_configured < 0 || SETTINGS.Ultrasonic_num_configured > Ultrasonic_Max_Sensors) {
          SETTINGS.Ultrasonic_num_configured = 0;
        } 
        if (SETTINGS.Ultrasonic_num_configured <= Ultrasonic_Max_Sensors && server.arg("ultpintr").toInt() != Not_used_pin_number ) { // dodam samo ce je pin razlicen od 100
          strcpy (SETTINGS.Ultrasonic_ident[SETTINGS.Ultrasonic_num_configured], server.arg("ultname").c_str());
          SETTINGS.Ultrasonic_pin_trigger[SETTINGS.Ultrasonic_num_configured] = server.arg("ultpintr").toInt();
          SETTINGS.Ultrasonic_pin_echo[SETTINGS.Ultrasonic_num_configured] = server.arg("ultpinec").toInt();
          SETTINGS.Ultrasonic_max_distance[SETTINGS.Ultrasonic_num_configured] = server.arg("ultime").toInt();
          SETTINGS.Ultrasonic_domoticz_idx[SETTINGS.Ultrasonic_num_configured] = server.arg("ultidx").toInt();
          SETTINGS.Ultrasonic_offset[SETTINGS.Ultrasonic_num_configured] = server.arg("ultoff").toInt();
          SETTINGS.Ultrasonic_num_configured++;
        }
        server.send(200, "text/html", BackURLSystemSettings);
      });
      yield();
      server.on("/ultrem", http_Ultrasonic_remove_pin);
      server.on("/ioadd", []() {
        if (SETTINGS.IO_num_configured < 0 || SETTINGS.IO_num_configured > IO_Max) {
          SETTINGS.IO_num_configured = 0;
        } 
        if (SETTINGS.IO_num_configured <= IO_Max && server.arg("iopin").toInt() != Not_used_pin_number ) { // dodam samo ce je pin razlicen od 100
          strcpy (SETTINGS.IO_ident[SETTINGS.IO_num_configured], server.arg("ioname").c_str());
          SETTINGS.IO_pin[SETTINGS.IO_num_configured] = server.arg("iopin").toInt();
          SETTINGS.IO_mode[SETTINGS.IO_num_configured] = server.arg("iomode").toInt();
          SETTINGS.IO_timer[SETTINGS.IO_num_configured] = server.arg("iotime").toFloat();
          SETTINGS.IO_domoticz_idx[SETTINGS.IO_num_configured] = server.arg("ioidx").toInt();
          SETTINGS.IO_num_configured++;
          IO_init_ports();
        }
        server.send(200, "text/html", BackURLSystemSettings);
      });
      yield();
      server.on("/iorem", http_IO_remove_pin);
      server.on("/iocmd", []() {
        for (int i = 0; i < server.args(); i++) {
          // primer url-ja http://172.22.0.18/iocmd?0=1 0=id 1=state(on/off)
          int pin_id;
          int pin_state;
          pin_id = server.argName(i).toInt();
          pin_state = server.arg(i).toInt();
          if (pin_id <= SETTINGS.IO_num_configured && pin_id >= 0) {
            if (SETTINGS.IO_mode[pin_id] > 3) {
              previousIOmillis[pin_id] = millis();
              digitalWrite(SETTINGS.IO_pin[pin_id], pin_state);
            }
          }
        }
        server.send(200, "text/html", BackURLRoot);
      });
      yield();
      server.on("/dsset", []() {
        SETTINGS.DS18B20_pin = server.arg("dspin").toInt();
        DS18B20_begin();
        server.send(200, "text/html", BackURLSystemSettings);
      });
      server.on("/dsadd", []() {
        if (SETTINGS.DS18B20_num_configured < 0 || SETTINGS.DS18B20_num_configured > DS18B20_Max_Sensors) {
          SETTINGS.DS18B20_num_configured = 0;
        } 
        if (SETTINGS.DS18B20_num_configured <= DS18B20_Max_Sensors && SETTINGS.DS18B20_pin != Not_used_pin_number ) { // dodam samo ce je pin razlicen od 100
          oneWire.apin(SETTINGS.DS18B20_pin);
          strcpy (SETTINGS.DS18B20_ident[SETTINGS.DS18B20_num_configured], server.arg("dsname").c_str());
          DS18B20_String_To_Byte(server.arg("dsaddr").c_str(),SETTINGS.DS18B20_address[SETTINGS.DS18B20_num_configured]);
          SETTINGS.DS18B20_domoticz_idx[SETTINGS.DS18B20_num_configured] = server.arg("dsidx").toInt();
          SETTINGS.DS18B20_resolution[SETTINGS.DS18B20_num_configured] = server.arg("dsres").toInt();
          SETTINGS.DS18B20_num_configured++;
          DS18B20_begin();
        }
        server.send(200, "text/html", BackURLSystemSettings);
      });
      yield();
      server.on("/dsrem", http_ds18b20_remove_sensor);
      server.on("/dhtadd", []() {
        if (SETTINGS.DHT_num_configured < 0 || SETTINGS.DHT_num_configured > DHT_Max_Sensors) {
          SETTINGS.DHT_num_configured = 0;
        } 
        if (SETTINGS.DHT_num_configured <= DHT_Max_Sensors && server.arg("dhtpin").toInt() != Not_used_pin_number ) { // dodam samo ce je pin razlicen od 100
          strcpy (SETTINGS.DHT_ident[SETTINGS.DHT_num_configured], server.arg("dhtname").c_str());
          SETTINGS.DHT_pin[SETTINGS.DHT_num_configured] = server.arg("dhtpin").toInt(); 
          SETTINGS.DHT_type[SETTINGS.DHT_num_configured] = server.arg("dhttype").toInt();
          SETTINGS.DHT_domoticz_idx[SETTINGS.DHT_num_configured] = server.arg("dhtidx").toInt();
          SETTINGS.DHT_num_configured++;
        }
        server.send(200, "text/html", BackURLSystemSettings);
      });
      yield();
      server.on("/dhtrem", http_dht_remove_sensor);
      server.on("/bmeadd", []() {
        if (SETTINGS.BME280_num_configured < 0 || SETTINGS.BME280_num_configured > BME280_Max_Sensors) {
          SETTINGS.BME280_num_configured = 0;
        }
        if (SETTINGS.BME280_num_configured <= BME280_Max_Sensors && server.arg("bmesda").toInt() != Not_used_pin_number && server.arg("bmescl").toInt() != Not_used_pin_number) { // dodam samo ce je pin razlicen od 100
          strcpy (SETTINGS.BME280_ident[SETTINGS.BME280_num_configured], server.arg("bmename").c_str());
          SETTINGS.BME280_pin_sda[SETTINGS.BME280_num_configured] = server.arg("bmesda").toInt(); 
          SETTINGS.BME280_pin_scl[SETTINGS.BME280_num_configured] = server.arg("bmescl").toInt();
          SETTINGS.BME280_domoticz_idx[SETTINGS.BME280_num_configured] = server.arg("bmeidx").toInt();
          SETTINGS.BME280_num_configured++;
        }
        server.send(200, "text/html", BackURLSystemSettings);
      });
      yield();
      server.on("/bmerem", http_bme_remove_sensor);
      server.on("/maxadd", []() {
        if (SETTINGS.MAX6675_num_configured < 0 || SETTINGS.MAX6675_num_configured > MAX6675_Max_Sensors) {
          SETTINGS.MAX6675_num_configured = 0;
        }
        if (SETTINGS.MAX6675_num_configured <= MAX6675_Max_Sensors && server.arg("maxclk").toInt() != Not_used_pin_number && server.arg("maxcs").toInt() != Not_used_pin_number) { // dodam samo ce je pin razlicen od 100
          strcpy (SETTINGS.MAX6675_ident[SETTINGS.MAX6675_num_configured], server.arg("maxname").c_str());
          SETTINGS.MAX6675_pin_clk[SETTINGS.MAX6675_num_configured] = server.arg("maxclk").toInt();
          SETTINGS.MAX6675_pin_cs[SETTINGS.MAX6675_num_configured] = server.arg("maxcs").toInt();
          SETTINGS.MAX6675_pin_do[SETTINGS.MAX6675_num_configured] = server.arg("maxdo").toInt();
          SETTINGS.MAX6675_domoticz_idx[SETTINGS.MAX6675_num_configured] = server.arg("maxidx").toInt();
          SETTINGS.MAX6675_num_configured++;
        }
        server.send(200, "text/html", BackURLSystemSettings);
      });
      yield();
      server.on("/flxadd", []() {
        if (SETTINGS.data_collector_num_configured < 0 || SETTINGS.data_collector_num_configured > data_collector_Max_Clients) {
          SETTINGS.data_collector_num_configured = 0;
        }
        if (SETTINGS.data_collector_num_configured <= data_collector_Max_Clients) {
          strcpy (SETTINGS.data_collector_host[SETTINGS.data_collector_num_configured], server.arg("flxho").c_str());
          SETTINGS.data_collector_port[SETTINGS.data_collector_num_configured] = server.arg("flxpo").toInt();
          strcpy (SETTINGS.data_collector_db_name[SETTINGS.data_collector_num_configured], server.arg("flxdn").c_str());
          strcpy (SETTINGS.data_collector_db_username[SETTINGS.data_collector_num_configured], server.arg("flxdu").c_str());
          strcpy (SETTINGS.data_collector_db_password[SETTINGS.data_collector_num_configured], server.arg("flxdp").c_str());
          if (server.arg("flxin").toInt() < Minimum_update_interval) {
            SETTINGS.data_collector_push_interval[SETTINGS.data_collector_num_configured] = Minimum_update_interval;
          }
          else {
            SETTINGS.data_collector_push_interval[SETTINGS.data_collector_num_configured] = server.arg("flxin").toInt(); 
          }          
          SETTINGS.data_collector_type[SETTINGS.data_collector_num_configured] = server.arg("flxty").toInt();
          SETTINGS.data_collector_num_configured++;
        }
        server.send(200, "text/html", BackURLSystemSettings);
      });
      yield();
      server.on("/flxrem", http_data_collector_remove_client);
      server.on("/maxrem", http_max_remove_sensor);
      server.on("/show", handle_show);
      server.on("/json",handle_json);
      server.on("/default",handle_load_defaults);
      server.on("/settings",handle_Settings);
      server.on("/values",http_values);
      server.on("/reboot",handle_reboot);
      server.on("/save",saveConfig);
      server.on("/commit", []() {
        strcpy (SETTINGS.ssid, server.arg("ssid").c_str());        
        strcpy (SETTINGS.password, server.arg("pass").c_str());
        strcpy (SETTINGS.ap_ssid, server.arg("ap_ssid").c_str());        
        strcpy (SETTINGS.ap_password, server.arg("ap_pass").c_str());
        strcpy (SETTINGS.css_link, server.arg("css").c_str());
        strcpy (SETTINGS.syslog, server.arg("syslog").c_str());
        strcpy (SETTINGS.NTP_hostname, server.arg("ntp").c_str());
        SETTINGS.timezone_GMT = server.arg("gmt").toInt();  
        strcpy (SETTINGS.hostname, server.arg("hostname").c_str());
        // mqtt
        if (server.arg("mqena").toInt() == 1) {
          SETTINGS.mqtt_enabled = true;        
        }
        else {
          SETTINGS.mqtt_enabled = false;
        }
        strcpy (SETTINGS.mqtt_server, server.arg("mqbr").c_str());
        SETTINGS.mqtt_port = server.arg("mqprt").toInt();
        if (server.arg("mqint").toInt() < Minimum_update_interval) {
          SETTINGS.mqtt_interval = Minimum_update_interval;
        }
        else {
          SETTINGS.mqtt_interval = server.arg("mqint").toInt(); 
        }
        strcpy (SETTINGS.mqtt_key, server.arg("mqkey").c_str());
        strcpy (SETTINGS.mqtt_secret, server.arg("mqsec").c_str());
        // domoticz update
        if (server.arg("domen").toInt() == 1) {
          SETTINGS.domoticz_update_enabled = true;        
        }
        else {
          SETTINGS.domoticz_update_enabled = false;
        }
        if (server.arg("domint").toInt() < Minimum_update_interval) {
          SETTINGS.domoticz_update_interval = Minimum_update_interval;
        }
        else {
          SETTINGS.domoticz_update_interval = server.arg("domint").toInt(); 
        }
        strcpy (SETTINGS.domoticz_ip, server.arg("domip").c_str()); 
        SETTINGS.domoticz_port = server.arg("domport").toInt();
        SETTINGS.domoticz_idx_voltage = server.arg("domvolt").toInt();
        SETTINGS.domoticz_idx_uptime = server.arg("domtime").toInt();
        SETTINGS.domoticz_idx_wifi = server.arg("domwifi").toInt();
        // end
        server.send(200, "text/html", BackURLSystemSettings);
        Set_Syslog_From_Settings();
        IO_init_ports();
        init_ntp();
        saveConfig();
        });
      yield();
      server.on("/enapsta",handle_enable_ap_sta);
      server.on("/disap",handle_disable_ap);
      server.on("/recon",ConnectToWifi);    
      server.begin();
      if (SETTINGS.DS18B20_pin != Not_used_pin_number ) { // dodam samo ce je pin razlicen od 100
        DS18B20_begin();
        DS18B20_Discover_Devices();
      }
      if (SETTINGS.DHT_pin[0] != Not_used_pin_number ) { 
        dht.begin();
      }
      IO_init_ports();
      mqtt_init();
      init_ntp();
      String BootText;
      BootText += " BOOT: Time " + String(millis()) + "ms";
      BootText += " Firmware ver.: " + String(Version);
      BootText += " BootMode: " + String(ESP.getBootMode());
      BootText += " BootVersion: " + String(ESP.getBootVersion());
      BootText += " FreeHeap: " + String(ESP.getFreeHeap());
      BootText += " CPUFreqMhz: " + String(ESP.getCpuFreqMHz());
      BootText += " ResetInfo: " + String(ESP.getResetInfo());
      //BootText += " DS18B20 - Found " + String(DS18B20_Count) + " devices.";
      BootText += " Time/Date:" + show_time() + " " + show_date();
      SendLog(BootText);
      http_WiFi_Scan();
    }

    void loop(void) {
      // MAIN LOOP //
      server.handleClient();
      yield();
      domoticz_update_client();
      yield();
      data_collector_send_data();
      yield();
      IO_set_pin_to_low_after_sometime(); 
      yield();
      mqtt_loop();
      yield();
      mqtt_send_data(); 
      if (millis() - previousMillis >= 2000) {
        previousMillis = millis();
        yield();
        // tole se izvaja na 2sek
        DS18B20_read_data();
        yield();
        DHT_read_data();
        yield();
        BME280_read_data();
        yield();
        MAX6675_read_data();
        yield();
        Ultrasonic_read_data();
      }
    }

    void http_Status() {
      SendLog(" INFO: View status.");
      String html = "<!DOCTYPE html><html><head>";
      html += "<title>status</title>";  
      html += "<meta charset=\"UTF-8\">\n";
      html += "Wifi.status=";
      html += String(WiFi.status());
      html += "<br>";
      server.send(200, "text/html", html);
    }

    byte String_To_Byte(String MyString) {
      byte MyByte;
      char temp [25];
      MyString.toCharArray(temp, 8);
      char* pos = temp;
      MyByte = strtol(pos, &pos, 8);
      return MyByte;
    }

    void init_ntp() {
      //Serial.println("waiting for ntp");
      SendLog(" ERROR: waiting for ntp.");
      NTPclient.begin(SETTINGS.NTP_hostname, SETTINGS.timezone_GMT);
      setSyncInterval(18000);
      setSyncProvider(getNTPtime);
      yield();
      SendLog(" INFO: NTP init.");
      String html = BackURLSystemSettings;
      server.send(200, "text/html", html);
    }

    time_t getNTPtime(void)
    {
      return NTPclient.getNtpTime();
    }

    String show_time() {
      String _hour, _minute, _second;
      if (hour() < 10) {
        _hour = "0" + String(hour());
      }
      else {
        _hour = String(hour());
      }
      if (minute() < 10) {
        _minute = "0" + String(minute());
      }
      else {
        _minute = String(minute());
      }
      if (second() < 10) {
        _second = "0" + String(second());
      }
      else {
        _second = String(second());
      }
      return _hour + ":" + _minute + ":" + _second;
    }

    String show_date() {
      return String(day()) + "." + String(month()) + "." + String(year());
    }

    void saveConfig() {
      SETTINGS.defaults = LoadDefaultNumber;
      EEPROM.begin(EEPROM_SIZE);
      delay(10);
      EEPROM.put(EEPROM_START, SETTINGS);
      yield();
      EEPROM.commit();
      String html = BackURLSystemSettings;
      server.send(200, "text/html", html);
      Serial.println("Settings saved!");
      SendLog(" INFO: Settings saved.");
    }
    
    void loadConfig() {
      EEPROM.begin(EEPROM_SIZE);
      delay(10);
      EEPROM.get(EEPROM_START, SETTINGS);
      yield();
      Set_Syslog_From_Settings();
      SendLog(" INFO: Settings loaded.");
      // Check for invalid settings
      if (SETTINGS.DS18B20_num_configured > DS18B20_Max_Sensors && SETTINGS.DS18B20_num_configured < 0) {
        SETTINGS.DS18B20_num_configured = 0;
      }
      if (SETTINGS.DHT_num_configured > DHT_Max_Sensors && SETTINGS.DHT_num_configured < 0) {
        SETTINGS.DHT_num_configured = 0;
      }
      if (SETTINGS.BME280_num_configured > BME280_Max_Sensors && SETTINGS.BME280_num_configured < 0) {
        SETTINGS.BME280_num_configured = 0;
      }
      if (SETTINGS.MAX6675_num_configured > MAX6675_Max_Sensors && SETTINGS.MAX6675_num_configured < 0) {
        SETTINGS.MAX6675_num_configured = 0;
      }
      if (SETTINGS.IO_num_configured > IO_Max && SETTINGS.IO_num_configured < 0) {
        SETTINGS.IO_num_configured = 0;
      }
      if (SETTINGS.Ultrasonic_num_configured > Ultrasonic_Max_Sensors && SETTINGS.Ultrasonic_num_configured < 0) {
        SETTINGS.Ultrasonic_num_configured = 0;
      }
      if (SETTINGS.data_collector_num_configured > data_collector_Max_Clients && SETTINGS.data_collector_num_configured < 0) {
        SETTINGS.data_collector_num_configured = 0;
      }
    }

    void handle_load_defaults() {
      // wifi
      //strcpy (SETTINGS.ssid, "ssid-to-connect-to"); ne potrebujemo imamo dropdown
      strcpy (SETTINGS.password, "password");
      strcpy (SETTINGS.ap_ssid, "universal");
      strcpy (SETTINGS.ap_password, "password");
      strcpy (SETTINGS.css_link, "");
      strcpy (SETTINGS.syslog, "");
      strcpy (SETTINGS.hostname, "universal");
      strcpy (SETTINGS.NTP_hostname, "ntp1.arnes.si");
      SETTINGS.timezone_GMT = 1;
      SETTINGS.DS18B20_num_configured = 0;
      strcpy (SETTINGS.DS18B20_ident[0], "");
      SETTINGS.DS18B20_resolution[0] = 0;
      SETTINGS.DS18B20_domoticz_idx[0] = 0;
      SETTINGS.DHT_num_configured = 0;
      SETTINGS.DHT_pin[0] = 100;   
      SETTINGS.DHT_type[0] = 0;
      strcpy (SETTINGS.DHT_ident[0], "");
      SETTINGS.DHT_domoticz_idx[0] = 0;
      SETTINGS.BME280_num_configured = 0;
      SETTINGS.BME280_pin_sda[0] = 100;
      SETTINGS.BME280_pin_scl[0] = 100;
      strcpy (SETTINGS.BME280_ident[0], "");
      SETTINGS.BME280_domoticz_idx[0] = 0;
      SETTINGS.MAX6675_num_configured = 0;
      SETTINGS.MAX6675_pin_clk[0] = 100;
      SETTINGS.MAX6675_pin_cs[0] = 100;
      SETTINGS.MAX6675_pin_do[0] = 100;
      strcpy (SETTINGS.MAX6675_ident[0], "");
      SETTINGS.domoticz_update_enabled = false;                 
      SETTINGS.domoticz_update_interval = 5;
      strcpy (SETTINGS.domoticz_ip, "");
      SETTINGS.domoticz_port = 0;
      SETTINGS.domoticz_idx_voltage = 0;
      SETTINGS.domoticz_idx_uptime = 0;
      SETTINGS.domoticz_idx_wifi = 0;
      SETTINGS.IO_num_configured = 0;
      strcpy (SETTINGS.IO_ident[0], "");
      SETTINGS.IO_pin[0] = 100;
      SETTINGS.IO_mode[0] = 0;
      SETTINGS.IO_timer[0] = 0;
      SETTINGS.IO_domoticz_idx[0] = 0;
      SETTINGS.Ultrasonic_num_configured = 0;
      SETTINGS.data_collector_num_configured = 0;
      SETTINGS.Ultrasonic_pin_trigger[0] = 100;
      SETTINGS.Ultrasonic_pin_echo[0] = 100;
      SETTINGS.Ultrasonic_max_distance[0] = 0;
      strcpy (SETTINGS.Ultrasonic_ident[0], "");
      SETTINGS.Ultrasonic_domoticz_idx[0] = 0;
      SETTINGS.Ultrasonic_offset[0] = 0;
      strcpy (SETTINGS.mqtt_server, "");
      SETTINGS.mqtt_port = 1883;
      SETTINGS.mqtt_interval = 10;
      SETTINGS.mqtt_enabled = 0;
      strcpy (SETTINGS.mqtt_key, "");
      strcpy (SETTINGS.mqtt_secret, "");
      strcpy (SETTINGS.data_collector_host[0], "");
      SETTINGS.data_collector_port[0] = 8086;
      strcpy (SETTINGS.data_collector_db_name[0], "db");
      strcpy (SETTINGS.data_collector_db_username[0], "");
      strcpy (SETTINGS.data_collector_db_password[0], "");
      SETTINGS.data_collector_push_interval[0] = 30;
      SETTINGS.data_collector_type[0] = 1;
      // clear crash
      http_clear_crash_info();
      String html = BackURLSystemSettings;
      server.send(200, "text/html", html);
      Serial.println("Defaults loaded!");
      SendLog(" INFO: Defaults loaded."); 
    }
    
    void ConnectToWifi() {
      SendLog(" INFO: WiFi connecting ...");
      WiFi.hostname(String(SETTINGS.hostname)); 
      WiFi.mode(WIFI_AP_STA); // AP & client mode
      /* You can remove the password parameter if you want the AP to be open. */
      WiFi.softAP(SETTINGS.ap_ssid, SETTINGS.ap_password);
      Serial.print("AP IP address: ");
      Serial.println(WiFi.softAPIP());
      // Connect to WiFi network
      WiFi.begin(SETTINGS.ssid, SETTINGS.password);
      Serial.print("\n\r \n\rConnecting to WiFi");
      // Wait for connection
      wifi_connect_previous_millis = millis();
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        if (millis() > wifi_connect_previous_millis + 5000) {
          Serial.println("");
          Serial.println("--- AP+STA diagnostic ---");
          WiFi.printDiag(Serial);
          Serial.println("--- AP+STA diagnostic ---");
          Serial.println("Switching to only AP mode");
          WiFi.mode(WIFI_AP); // only AP mode
          Serial.println("--- AP settings ---");
          Serial.print("ssid: ");
          Serial.println(SETTINGS.ap_ssid);
          Serial.print("psk: ");
          Serial.println(SETTINGS.ap_password);
          Serial.println("--- AP settings ---");
          break; //prekinem povezovanje in nadaljujem z AP_STA nacinom
        }
      }
      if (WiFi.status() == WL_CONNECTED) {
        WiFi.mode(WIFI_STA); // only client mode
        Serial.println("");
        Serial.print("Connected to SSID: ");
        Serial.println(SETTINGS.ssid);
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());       
      }
    }


    String urlencode(String str)
    {
        String encodedString="";
        char c;
        char code0;
        char code1;
        char code2;
        for (int i =0; i < str.length(); i++){
          c=str.charAt(i);
          if (c == ' '){
            encodedString+= '+';
          } else if (isalnum(c)){
            encodedString+=c;
          } else{
            code1=(c & 0xf)+'0';
            if ((c & 0xf) >9){
                code1=(c & 0xf) - 10 + 'A';
            }
            c=(c>>4)&0xf;
            code0=c+'0';
            if (c > 9){
                code0=c - 10 + 'A';
            }
            code2='\0';
            encodedString+='%';
            encodedString+=code0;
            encodedString+=code1;
            //encodedString+=code2;
          }
          yield();
        }
        return encodedString;
    }

    void SendLog(String msgtosend) {
      Serial.print("log: ");
      Serial.println(msgtosend);
      SendUDPSyslog(msgtosend);
      mqtt_send_log(String(SETTINGS.hostname) + msgtosend);
    }
    

    void SendUDPSyslog(String msgtosend) {
      if (WiFi.status() == WL_CONNECTED) {
        if (SETTINGS.syslog[0] != '\0') { // preverim ali syslog IP nastavljen
          unsigned int msg_length = msgtosend.length();
          byte* p = (byte*)malloc(msg_length);
          memcpy(p, (char*) msgtosend.c_str(), msg_length);
          udp.beginPacket(syslogServer, 514);
          udp.write(SETTINGS.hostname);
          udp.write(p, msg_length);
          udp.endPacket();
          free(p);
        }
      }
    }
    
    void Set_Syslog_From_Settings() {
      if (syslogServer.fromString(SETTINGS.syslog)) {
        //SendLog(" INFO: Valid syslog IP Address");
        int Parts[4] = {0,0,0,0};
        int Part = 0;
        for ( int i=0; i<sizeof(SETTINGS.syslog); i++ )
        {
          char c = SETTINGS.syslog[i];
          if ( c == '.' )
          {
            Part++;
            continue;
          }
          Parts[Part] *= 10;
          Parts[Part] += c - '0';
        }
        IPAddress syslogServer( Parts[0], Parts[1], Parts[2], Parts[3] );
      } 
    }


    void DS18B20_StringAddr_To_Byte(String Name, byte Address[8]) {
      char temp [25];
      server.arg(Name).toCharArray(temp, 25);
      char* pos = temp;
      for (uint8_t i = 0; i < 8; i++) {
          Address[i] = strtol(pos, &pos, 16); 
       }
    }

    void DS18B20_String_To_Byte(String Saddress, byte Address[8]) {
      char temp [25];
      Saddress.toCharArray(temp, 25);
      char* pos = temp;
      for (uint8_t i = 0; i < 8; i++) {
          Address[i] = strtol(pos, &pos, 16); 
       }
    }

    String DS18B20_Print_Address(byte Address[8]) {
      String my_address;
      //Serial.println("Printing address:");
        for (uint8_t i = 0; i < 8; i++ ) {
          if (Address[i] < 16) {
            my_address += '0';
          }
          my_address += String(Address[i], HEX);
          //Serial.print(Address[i], HEX);
          //Serial.println(' ');
        }
        return my_address;
    }
    
    void DS18B20_Discover_Devices(void) {
      if (SETTINGS.DS18B20_pin != Not_used_pin_number) { // ce je pin=100 potem je disabled
        byte i;
        byte addr[8];
        DS18B20_Count = 0;
        //Serial.print("Looking for 1-Wire devices...\n\r");
        SendLog(" INFO: DS18B20 - Looking for 1-Wire devices...");
        while(oneWire.search(addr)) {
          //oneWire.getAddress(addr, index)
          //Serial.print("Found device:");
          String TempAddress = " INFO: DS18B20 - Found device: ";
          for( i = 0; i < 8; i++) {
            DS18B20_All_Sensor_Addresses [DS18B20_Count][i] = addr[i];
            if (addr[i] < 16) {
              TempAddress += '0';
            }
            //Serial.print(addr[i], HEX);
            TempAddress += String(addr[i], HEX);
            //SendLog(TempAddress);
          }
          //Serial.println("");
          if ( OneWire::crc8( addr, 7) != addr[7]) {
              //Serial.print("CRC is not valid!\n");
              SendLog(" ERROR: DS18B20 - CRC is not valid.");
              return;
          }
          DS18B20_Count ++;
        }
        oneWire.reset_search();
        DS18B20_begin();
        return;
      }
    }
    
    void DS18B20_begin() {
      if (SETTINGS.DS18B20_pin != Not_used_pin_number) { // ce je pin=100 potem je disabled
        oneWire.apin(SETTINGS.DS18B20_pin);
        // vsem sensorjem nastavim resolucijo
        DS18B20.begin();
        for (int i = 0; i < DS18B20_Count; i++) {
          DS18B20.setResolution(DS18B20_All_Sensor_Addresses[i],SETTINGS.DS18B20_resolution[i]);
        }
      }
    } 



    float DS18B20_validate_reading(float new_temp, String sensor) {
      if (new_temp == -127 ){
        SendLog("ERROR: DS18B20 " +  sensor + " reported -127°C.");
        return NAN;
      }
      if (new_temp == 85 ){
        SendLog("ERROR: DS18B20 " +  sensor + " reported 85°C.");
        return NAN;
      }
      else {
        return new_temp;
      }      
    }
      
    void DS18B20_read_data () {
      // Pobiranje podatkov s senzorjev
      if (SETTINGS.DS18B20_pin != Not_used_pin_number) { // ce je pin=100 potem je disabled
        DS18B20.requestTemperatures();  
        for (int i = 0; i < DS18B20_Count; i++) {
          DS18B20_All_Sensor_Values[i] = DS18B20_validate_reading(DS18B20.getTempC(SETTINGS.DS18B20_address[i]), SETTINGS.DS18B20_ident[i]);
        }
      }
    }
    
    // Kreira dropdown meni naslovov DS18B20 senzorjev
    String http_DS18B20_DropDownMenu(String Name) {     
      String DropDown = "<select name=\"" + Name + "\">";
      for (int i = 0; i < DS18B20_Count; i++) {
        String TempAddress;
        for(int j = 0; j < 8; j++) {
          if (DS18B20_All_Sensor_Addresses[i][j] < 16) {
            TempAddress = TempAddress + '0';
          }
          TempAddress += String(DS18B20_All_Sensor_Addresses[i][j],HEX);
          if (j < 7) {
            TempAddress += ' ';
          }
        }
        DropDown += "<option>" + TempAddress + "</option>";
      }
      DropDown += "</select>";
      return DropDown;
    }

    void http_ds18b20_remove_sensor() {
      if (SETTINGS.DS18B20_num_configured > 0) {
        SETTINGS.DS18B20_num_configured--; 
      }
      String html = BackURLSystemSettings;
      server.send(200, "text/html", html);
    }
    
    void DHT_read_data() {
      for (int n = 0; n < SETTINGS.DHT_num_configured; n++) {
        if (SETTINGS.DHT_pin[n] != Not_used_pin_number) { // ce je pin=100 potem je disabled
          DHT dht(SETTINGS.DHT_pin[n], SETTINGS.DHT_type[n]);
          float dtemperature;
          float dhumidity;
          dtemperature = dht.readTemperature();
          yield();
          dhumidity = dht.readHumidity();
          yield();
          DHT_All_Sensor_Values_Temp[n] = dtemperature;
          DHT_All_Sensor_Values_Hum[n] =  dhumidity;
          DHT_All_Sensor_Values_Heat[n] = NAN;
          DHT_All_Sensor_Values_Dew[n] = NAN;
          if (isnan(dtemperature) || isnan(dhumidity)) {
            //Serial.println("Failed to read from DHT sensor!");
            String DHT_err;
            DHT_err += " ERROR: Failed to read from DHT sensor " + String(SETTINGS.DHT_ident[n]) + ", pin:" + String(SETTINGS.DHT_pin[n]) + ".";
            SendLog(DHT_err);
            return;
          }
          DHT_All_Sensor_Values_Heat[n] = dht.computeHeatIndex(dtemperature, dhumidity, false);
          DHT_All_Sensor_Values_Dew[n] = CalculateDewPoint(dtemperature, dhumidity);
        }
      }
    }

    // reference: http://en.wikipedia.org/wiki/Dew_point
    float CalculateDewPoint(float celsius, float humidity)
    {
      float a = 17.271;
      float b = 237.7;
      float temp = (a * celsius) / (b + celsius) + log(humidity*0.01);
      float Td = (b * temp) / (a - temp);
      return Td;
    }

    void http_dht_remove_sensor() {
      if (SETTINGS.DHT_num_configured > 0) {
        SETTINGS.DHT_num_configured--; 
      }
      String html = BackURLSystemSettings;
      server.send(200, "text/html", html);      
    }

    void BME280_read_data() {
      for (int n = 0; n < SETTINGS.BME280_num_configured; n++) {
        if (SETTINGS.BME280_pin_sda[n] != Not_used_pin_number) { // ce je pin=100 potem je disabled
          BME280.begin(SETTINGS.BME280_pin_sda[n], SETTINGS.BME280_pin_scl[n]); //start Wire and set SDA - SCL
          yield();
          delay(10);
          if (!BME280.begin()) { // read error
            BME280_All_Sensor_Values_Temp[n] = NAN;
            BME280_All_Sensor_Values_Hum[n]  = NAN;
            BME280_All_Sensor_Values_Press[n] = NAN;
            BME280_All_Sensor_Values_Dew[n] = NAN;
            BME280_All_Sensor_Values_Alt[n] = NAN;
            BME280_All_Sensor_Values_Heat[n] = NAN;
            String BME280_err;
            BME280_err += " ERROR: Failed to read from BME280 sensor " + String(SETTINGS.BME280_ident[n]) + ".";
            SendLog(BME280_err);
          }
          else {
            BME280_All_Sensor_Values_Temp[n] = BME280.temp();
            BME280_All_Sensor_Values_Hum[n]  = BME280.hum();
            BME280_All_Sensor_Values_Press[n] = BME280.press(0x1); //vrne mBar
            BME280_All_Sensor_Values_Dew[n] = BME280.dew();
            BME280_All_Sensor_Values_Alt[n] = BME280.alt();
            BME280_All_Sensor_Values_Heat[n] = dht.computeHeatIndex(BME280_All_Sensor_Values_Temp[n], BME280_All_Sensor_Values_Hum[n], false);
          }
        }
      }
    }

   void http_bme_remove_sensor () {
      if (SETTINGS.BME280_num_configured > 0) {
        SETTINGS.BME280_num_configured--; 
      }
      String html = BackURLSystemSettings;
      server.send(200, "text/html", html);      
    }
    
    void MAX6675_read_data() {
      for (int n = 0; n < SETTINGS.MAX6675_num_configured; n++) {
        if (SETTINGS.MAX6675_pin_clk[n] != Not_used_pin_number) { // ce je pin=100 potem je disabled
          uint16_t v;
          pinMode(SETTINGS.MAX6675_pin_cs[n], OUTPUT);
          pinMode(SETTINGS.MAX6675_pin_do[n], INPUT);
          pinMode(SETTINGS.MAX6675_pin_clk[n], OUTPUT);
          digitalWrite(SETTINGS.MAX6675_pin_cs[n], LOW);
          delay(1);
          // Read in 16 bits,
          //  15    = 0 always
          //  14..2 = 0.25 degree counts MSB First
          //  2     = 1 if thermocouple is open circuit  
          //  1..0  = uninteresting status 
          v = shiftIn(SETTINGS.MAX6675_pin_do[n], SETTINGS.MAX6675_pin_clk[n], MSBFIRST);
          v <<= 8;
          v |= shiftIn(SETTINGS.MAX6675_pin_do[n], SETTINGS.MAX6675_pin_clk[n], MSBFIRST); 
          digitalWrite(SETTINGS.MAX6675_pin_cs[n], HIGH);
          if (v & 0x4) 
          {    
            // Bit 2 indicates if the thermocouple is disconnected
            MAX6675_All_Sensor_Values_Temp[n] = NAN;
            SendLog(" ERROR: thermocouple " + String(SETTINGS.MAX6675_ident[n]) + " disconnected.");
            return;     
          }
          // The lower three bits (0,1,2) are discarded status bits
          v >>= 3;
          // The remaining bits are the number of 0.25 degree (C) counts
          MAX6675_All_Sensor_Values_Temp[n] = v*0.25;
        }
      }
    }

   void http_max_remove_sensor () {
      if (SETTINGS.MAX6675_num_configured > 0) {
        SETTINGS.MAX6675_num_configured--; 
      }
      String html = BackURLSystemSettings;
      server.send(200, "text/html", html);      
    }

    void handle_enable_ap_sta() {
      SendLog(" INFO: Soft AP enabled.");
      WiFi.mode(WIFI_AP_STA);
      WiFi.softAP(SETTINGS.ap_ssid, SETTINGS.ap_password);
      //Serial.print("Soft AP enabled! ");
      //Serial.println(WiFi.softAPIP());
      String html = BackURLSystemSettings;
      server.send(200, "text/html", html);
    }
    
    void handle_disable_ap() {
      SendLog(" INFO: SoftAP disabled.");
      WiFi.mode(WIFI_STA);
      Serial.print("SoftAP disabled! ");
      Serial.println(WiFi.localIP());
      String html = BackURLSystemSettings;
      server.send(200, "text/html", html);
    }   
    
    void handle_reboot() {
      SendLog(" INFO: Restart requested."); 
      String html = BackURLSystemSettings;
      server.send(200, "text/html", html);
      delay(300);
      ESP.restart();
    }
      
    void handle_root() {
      String html = "<!DOCTYPE html><html><head>";
      html += "<title>" + String(SETTINGS.hostname) + "</title>";
      html += "<meta charset=\"UTF-8\">\n";
        if (SETTINGS.css_link[0] == '\0') { // preverim ali je css settings prazen, potem nalozim default css
          //html += css_string();
          html += FPSTR(CSS);
        }
        else {
          html += "<link rel=\"stylesheet\" type=\"text/css\" href=\"" + String(SETTINGS.css_link) + "\">"; 
        }
      html += static_html_handle_root;
      for (int n = 0; n < SETTINGS.IO_num_configured; n++) {
        if (SETTINGS.IO_pin[n] != Not_used_pin_number) { // ce je pin=100 potem je disabled
          if (SETTINGS.IO_mode[n] == 4) { // TOGGLE
            if (digitalRead(SETTINGS.IO_pin[n]) == 1) { // pin = ON
              html += "<a href=\"/iocmd?" + String(n) + "=0\"><button class=\"g\">"+ String(SETTINGS.IO_ident[n]) + " OFF</button></a>";  
            }
            else {
              html += "<a href=\"/iocmd?" + String(n) + "=1\"><button class=\"g\">"+ String(SETTINGS.IO_ident[n]) + " ON</button></a>";
            }
          }
          if (SETTINGS.IO_mode[n] == 5) { // PUSH BUTTON (set value to 1)
            html += "<a href=\"/iocmd?" + String(n) + "=1\"><button class=\"g\">"+ String(SETTINGS.IO_ident[n]) + "</button></a>";
          }
          if (SETTINGS.IO_mode[n] == 6) { // INVERTED PUSH BUTTON (set value to 0)
            html += "<a href=\"/iocmd?" + String(n) + "=0\"><button class=\"g\">"+ String(SETTINGS.IO_ident[n]) + "</button></a>";
          }      
        }
      }
      html += "<div class=\"set\"><a href=\"/settings\">⚙</a></div>";
      html += "</body></html>\n";
      server.send(200, "text/html", html);
      yield();
    }

    void http_values() {
      String html;
      html += html_dynamic_data();
      html += html_fixed_data();
      server.send(200, "text/html", html);
      yield();
    } 

    String html_dynamic_data() {
      String html;
      for (int n = 0; n < SETTINGS.IO_num_configured; n++) {
        if (SETTINGS.IO_pin[n] != Not_used_pin_number) { // ce je pin=100 potem je disabled
          html += "<span class=\"cloud\">" + String(SETTINGS.IO_ident[n]) + "<br>" + String(digitalRead(SETTINGS.IO_pin[n])) + "</span>";
        }
      }
      if (SETTINGS.DS18B20_pin != Not_used_pin_number) { // ce je pin=100 potem je disabled
        for (int n = 0; n < SETTINGS.DS18B20_num_configured; n++) {
          html += "<span class=\"cloud\">" + String(SETTINGS.DS18B20_ident[n]) + "<br>" + String(DS18B20_All_Sensor_Values[n]) + "°C</span>";
        }
      }
      for (int n = 0; n < SETTINGS.DHT_num_configured; n++) {
        if (SETTINGS.DHT_pin[n] != Not_used_pin_number) { // ce je pin=100 potem je disabled
          html += "<span class=\"cloud\">" + String(SETTINGS.DHT_ident[n]) + "<br>" + String(DHT_All_Sensor_Values_Temp[n]) + "°C/" + String(DHT_All_Sensor_Values_Hum[n]) + "%</span>";
          html += "<span class=\"cloud\">" + String(SETTINGS.DHT_ident[n]) + " heat/dew<br>" + String(DHT_All_Sensor_Values_Heat[n]) + "°C/" + String(DHT_All_Sensor_Values_Dew[n]) + "°C</span>";
        }
      }
      for (int n = 0; n < SETTINGS.BME280_num_configured; n++) {
        if (SETTINGS.BME280_pin_sda[n] != Not_used_pin_number) { // ce je pin=100 potem je disabled
          html += "<span class=\"cloud\">" + String(SETTINGS.BME280_ident[n]) + "<br>" + String(BME280_All_Sensor_Values_Temp[n]) + "°C/" + String(BME280_All_Sensor_Values_Hum[n]) + "%</span>";
          html += "<span class=\"cloud\">" + String(SETTINGS.BME280_ident[n]) + " pressure<br>" + String(BME280_All_Sensor_Values_Press[n]) + "mBar</span>";
          html += "<span class=\"cloud\">" + String(SETTINGS.BME280_ident[n]) + " heat/dew<br>" + String(BME280_All_Sensor_Values_Heat[n]) + "°C/" + String(BME280_All_Sensor_Values_Dew[n]) + "°C</span>";
        }
      }
      for (int n = 0; n < SETTINGS.MAX6675_num_configured; n++) {
        if (SETTINGS.MAX6675_pin_clk[n] != Not_used_pin_number) { // ce je pin=100 potem je disabled
          html += "<span class=\"cloud\">" + String(SETTINGS.MAX6675_ident[n]) + "<br>" + String(MAX6675_All_Sensor_Values_Temp[n]) + "°C</span>";
        }
      }
      for (int n = 0; n < SETTINGS.Ultrasonic_num_configured; n++) {
        if (SETTINGS.Ultrasonic_pin_trigger[n] != Not_used_pin_number) { // ce je pin=100 potem je disabled
          html += "<span class=\"cloud\">" + String(SETTINGS.Ultrasonic_ident[n]) + "<br>" + String(Ultrasonic_All_Sensor_Values_Distance[n]) + "cm</span>";
        }
      }
      return html;
    }

    String html_fixed_data() {
      long days = 0;
      long hours = 0;
      long mins = 0;
      unsigned long secs = 0;
      secs = millis()/1000; //convert milliseconds to seconds
      mins=secs/60; //convert seconds to minutes
      hours=mins/60; //convert minutes to hours
      days=hours/24; //convert hours to days
      secs=secs-(mins*60); //subtract the coverted seconds to minutes in order to display 59 secs max
      mins=mins-(hours*60); //subtract the coverted minutes to hours in order to display 59 minutes max
      hours=hours-(days*24); //subtract the coverted hours to days in order to display 23 hours max
      String html;
      html += "<span class=\"cloud\"> clock <br>" + show_time() + "</span>";
      html += "<span class=\"cloud\"> date <br>" + show_date() + "</span>";
      html += "<span class=\"cloud\"> wifi <br>" + String(WiFi.RSSI()) + "dB</span>";
      html += "<span class=\"cloud\"> Vcc <br>" + String((float)ESP.getVcc()/890) + "V</span>";
      html += "<span class=\"cloud\"> uptime <br>" + String((long)days) + "d " + String((long)hours) + "h " + String((long)mins) + "m " + String((long)secs)+ "s</span>";
      return html;
    }
        
    void handle_show() {
        String html = "<!DOCTYPE html><html><head>";
        html += "<title>" + String(SETTINGS.hostname) + "</title>";
        html += "<meta charset=\"UTF-8\">\n";
        if (SETTINGS.css_link[0] == '\0') { // preverim ali je css settings prazen, potem nalozim default css
          //html += css_string();
          html += FPSTR(CSS);
        }
        else {
          html += "<link rel=\"stylesheet\" type=\"text/css\" href=\"" + String(SETTINGS.css_link) + "\">"; 
        }
        html += static_html_handle_show;
        server.send(200, "text/html", html);
        yield();
    }

    void handle_Settings() {
        DS18B20_Discover_Devices(); 
        DS18B20_read_data();
        String html = "<!DOCTYPE html><html><head>";
        html += "<title>" + String(SETTINGS.hostname) + " settings</title>";
        html += "<meta charset=\"UTF-8\">\n";
        if (SETTINGS.css_link[0] == '\0') { // preverim ali je css settings prazen, potem nalozim default css
          //html += css_string();
          html += FPSTR(CSS);
        }
        else {
          html += "<link rel=\"stylesheet\" type=\"text/css\" href=\"" + String(SETTINGS.css_link) + "\">"; 
        }
        html += "</head><body class=\"f9\">";
        server.sendContent(html);
        delay(10);
        yield();
        html = ""; // ne brisi
        html += http_IO_config();
        html += "<tr><td><form method='get' action='ioadd' id=\"ioadd\">" + String(SETTINGS.IO_num_configured) + "</td><td><input name='ioname' value='' maxlength='25' type='text'/>";
        html += "</td><td>" + http_pin_dropdown("iopin", Not_used_pin_number);
        html += "</td><td>" + http_IO_mode("iomode");
        html += "</td><td><input name='iotime' value='' maxlength='6' type='text' size='6'/>";
        html += "</td><td>";
        html += "</td><td><input name='ioidx' value='' maxlength='6' type='text' size='6'/></form>";
        html += "<button type='submit' form=\"ioadd\" class=\"d\">+</button><a href=\"/iorem\"><button class=\"d\">-</button></a></td><tr></table>";
        server.sendContent(html);
        delay(10);
        yield();
        html = ""; // ne brisi
        html += http_DS18B20_config();
        html += "<tr><td><form method='get' action='dsadd' id=\"dsadd\">" + String(SETTINGS.DS18B20_num_configured) + "</td><td><input name='dsname' value='' maxlength='25' type='text'/>";
        html += "</td><td>" + http_DS18B20_DropDownMenu("dsaddr");
        html += "</td><td>" + http_DS18B20_resolution_dropdown("dsres");
        html += "</td><td>";
        html += "</td><td><input name='dsidx' value='' maxlength='6' type='text' size='6'/></form>";
        html += "<button type='submit' form=\"dsadd\" class=\"d\">+</button><a href=\"/dsrem\"><button class=\"d\">-</button></a></td><tr></table>";
        server.sendContent(html);
        delay(10);
        yield();
        html = ""; // ne brisi
        html += http_DHT_config();
        html += "<tr><td><form method='get' action='dhtadd' id=\"dhtadd\" >" + String(SETTINGS.DHT_num_configured);
        html += "</td><td><input name='dhtname' value='' maxlength='25' type='text'/>";
        html += "</td><td>" + http_pin_dropdown("dhtpin", Not_used_pin_number); // nastavil invalid pin 100, da je undef
        html += "</td><td>" + http_DHT_type("dhttype");
        html += "</td><td>";
        html += "</td><td><input name='dhtidx' value='' maxlength='6' type='text' size='6'/></form>";
        html += "<button type='submit' form=\"dhtadd\" class=\"d\">+</button><a href=\"/dhtrem\"><button class=\"d\">-</button></a></td><tr></table>";
        server.sendContent(html);
        delay(10);
        yield();
        html = ""; // ne brisi
        html += http_BME280_config();
        html += "<tr><td><form method='get' action='bmeadd' id=\"bmeadd\" >" + String(SETTINGS.BME280_num_configured);
        html += "</td><td><input name='bmename' value='' maxlength='25' type='text'/>";
        html += "</td><td>" + http_pin_dropdown("bmesda", Not_used_pin_number); // nastavil invalid pin 100, da je undef
        html += "</td><td>" + http_pin_dropdown("bmescl", Not_used_pin_number) + "</td><td>";
        html += "</td><td>";
        html += "<input name='bmeidx' value='' maxlength='6' type='text' size='6'/></form>";
        html += "<button type='submit' form=\"bmeadd\" class=\"d\">+</button><a href=\"/bmerem\"><button class=\"d\">-</button></a></td><tr></table>";
        server.sendContent(html);
        delay(10);
        yield();
        html = ""; // ne brisi
        html += http_MAX6675_config();
        html += "<tr><td><form method='get' action='maxadd' id=\"maxadd\" >" + String(SETTINGS.MAX6675_num_configured);
        html += "</td><td><input name='maxname' value='' maxlength='25' type='text'/>";
        html += "</td><td>" + http_pin_dropdown("maxclk", Not_used_pin_number); // nastavil invalid pin 100, da je undef
        html += "</td><td>" + http_pin_dropdown("maxcs", Not_used_pin_number); // nastavil invalid pin 100, da je undef
        html += "</td><td>" + http_pin_dropdown("maxdo", Not_used_pin_number) + "</td><td>";
        html += "</td><td>";
        html += "<input name='maxidx' value='' maxlength='6' type='text' size='6'/></form>";
        html += "<button type='submit' form=\"maxadd\" class=\"d\">+</button><a href=\"/maxrem\"><button class=\"d\">-</button></a></td><tr></table>";
        server.sendContent(html);
        delay(10);
        yield();
        html = ""; // ne brisi
        html += http_Ultrasonic_config();
        html += "<tr><td><form method='get' action='ultadd' id=\"ultadd\" >" + String(SETTINGS.Ultrasonic_num_configured);
        html += "</td><td><input name='ultname' value='' maxlength='25' type='text'/>";
        html += "</td><td>" + http_pin_dropdown("ultpintr", Not_used_pin_number); // nastavil invalid pin 100, da je undef
        html += "</td><td>" + http_pin_dropdown("ultpinec", Not_used_pin_number); // nastavil invalid pin 100, da je undef
        html += "</td><td>";
        html += "<input name='ultime' value='' maxlength='6' type='text' size='6'/></td><td>";
        html += "<input name='ultoff' value='' maxlength='6' type='text' size='6'/></td><td>";
        html += "</td><td>";
        html += "<input name='ultidx' value='' maxlength='6' type='text' size='6'/></form>";
        html += "<button type='submit' form=\"ultadd\" class=\"d\">+</button><a href=\"/ultrem\"><button class=\"d\">-</button></a></td><tr></table>";
        server.sendContent(html);
        delay(10);
        yield();
        html = ""; // ne brisi
        html += "<table><tr><th colspan=\"9\">influxdb, carbon, redis settings</th></tr>";
        html += "<tr><th>#</th><th>type</th><th>hostname/ip</th><th>port</th><th>interval[sec]</th><th>db name</th><th>db username</th><th>db password</th></tr>";
        html += http_data_collector_config();
        html += "<tr><td><form method='get' action='flxadd' ></td><td><select name=\"flxty\" ><option value=\"1\">influxdb_tcp</option><option value=\"4\">influxdb_udp</option><option value=\"2\">carbon_udp</option><option value=\"3\">redis_tcp</option></select></td><td>";
        html += "<input name='flxho' value='' maxlength='100' type='text' size='15'/></td><td>";
        html += "<input name='flxpo' value='' maxlength='6' type='text' size='6'/></td><td>";
        html += "<input name='flxin' value='' maxlength='6' type='text' type='text' size='6'/></td><td>";
        html += "<input name='flxdn' value='' maxlength='15' type='text' size='6'/></td><td>";
        html += "<input name='flxdu' value='' maxlength='15' type='text' size='6'/></td><td>";
        html += "<input name='flxdp' value='' maxlength='15' type='password' size='6'/>";
        html += "<button type=\"submit\" class=\"d\">+</button></form><a href=\"/flxrem\"><button class=\"d\">-</button></a></td><tr></table>";
        server.sendContent(html);
        delay(10);
        yield();
        html = ""; // ne brisi
        html += "<form method='get' action='commit' id=\"commit\" >";
        html += "<table><tr><th colspan=\"7\">Domoticz update sensors settings</th></tr>";
        html += "<tr><th>hostname/ip</th><th>port</th><th>interval[sec]</th><th>voltage idx</th><th>wifi idx</th><th>uptime</th><th>enabled</th></tr>";
        html += "<tr><td><input name='domip' value='" + String(SETTINGS.domoticz_ip) + "' maxlength='15' type='text' size='15'/></td><td>";
        html += "<input name='domport' value='" + String(SETTINGS.domoticz_port) + "' maxlength='6' type='text' size='6'/></td><td>";
        html += "<input name='domint' value='" + String(SETTINGS.domoticz_update_interval) + "' maxlength='6' type='text' size='6'/></td><td>";
        html += "<input name='domvolt' value='" + String(SETTINGS.domoticz_idx_voltage) + "' maxlength='6' type='text' size='6'/></td><td>";
        html += "<input name='domwifi' value='" + String(SETTINGS.domoticz_idx_wifi) + "' maxlength='6' type='text' size='6'/></td><td>";
        html += "<input name='domtime' value='" + String(SETTINGS.domoticz_idx_uptime) + "' maxlength='6' type='text' size='6'/></td><td>";
        if (SETTINGS.domoticz_update_enabled) {
          html += "<input type='checkbox' name='domen' value='1' checked/>";
        } else {
          html += "<input type='checkbox' name='domen' value='1'/>"; 
        }
        html += "</td><tr></table>";
        server.sendContent(html);
        delay(10);
        yield();
        html = ""; // ne brisi
        html += "<table><tr><th colspan=\"6\">MQTT settings</th></tr>";
        html += "<tr><th>broker/ip</th><th>port</th><th>interval[sec]</th><th>username</th><th>password</th><th>enabled</th></tr>";
        html += "<tr><td><input name='mqbr' value='" + String(SETTINGS.mqtt_server) + "' maxlength='100' type='text' size='15'/></td><td>";;
        html += "<input name='mqprt' value='" + String(SETTINGS.mqtt_port) + "' maxlength='6' type='text' size='6'/></td><td>";
        html += "<input name='mqint' value='" + String(SETTINGS.mqtt_interval) + "' maxlength='6' type='text' size='6'/></td><td>";
        html += "<input name='mqkey' value='" + String(SETTINGS.mqtt_key) + "' maxlength='25' type='text' size='15'/></td><td>";
        html += "<input name='mqsec' value='" + String(SETTINGS.mqtt_secret) + "' maxlength='25' type='password' size='15'/></td><td>";
        if (SETTINGS.mqtt_enabled) {
          html += "<input type='checkbox' name='mqena' value='1' checked/>";
        } else {
          html += "<input type='checkbox' name='mqena' value='1'/>"; 
        }
        html += "</td><tr></table>";
        server.sendContent(html);
        delay(10);
        yield();
        html = "<table>"; // ne brisi
        html += "<tr><th colspan=\"3\">NTP settings</th></tr>";
        html += "<tr><th>ntp hostname</th><th>timezone GMT</th></tr>";
        html += "<tr><td><input name='ntp' value='" + String(SETTINGS.NTP_hostname) + "' maxlength='150' type='text' size='15'/></td><td>";
        html += "<input name='gmt' value='" + String(SETTINGS.timezone_GMT) + "' maxlength='3' type='text' size='6' /></td>";
        html += "<tr></table>";
        server.sendContent(html);
        delay(10);
        yield();
        html = "<table>"; // ne brisi
        html += "<tr><th colspan=\"3\">system settings - version " + Version + "</th></tr>";
        html += "<tr><th>hostname</th><th>CSS url</th><th>syslog ip</th></tr>";
        html += "<tr><td><input name='hostname' value='" + String(SETTINGS.hostname) + "' maxlength='150' type='text' size='15'/></td><td>";
        html += "<input name='css' value='" + String(SETTINGS.css_link) + "' maxlength='150' type='text' size='25' /></td><td>";
        html += "<input name='syslog' value='" + String(SETTINGS.syslog) + "' maxlength='20' type='text' size='15'/></td></tr>";
        html += "</td><tr></table>";
        server.sendContent(html);
        delay(10);
        yield();
        html = "<table>"; // ne brisi
        html += "<tr><th  colspan=\"4\">WiFi settings</th></tr>";
        html += "<tr><th>SSID</th><th>PSK</th><th>AP SSID</th><th>AP PSK</th></tr>";
        html += "<tr><td>" + http_print_wifi_networks() + "</td><td>";
        html += "<input name='pass' value='" + String(SETTINGS.password) + "' maxlength='32' pattern='[0-9A-Za-z]{8,30}' type='password' size='15'/></td><td>";
        html += "<input name='ap_ssid' value='" + String(SETTINGS.ap_ssid) + "' maxlength='32' type='text' size='15' /></td><td>";
        html += "<input name='ap_pass' value='" + String(SETTINGS.ap_password) + "' maxlength='32' type='password' pattern='[0-9A-Za-z]{8,30}' size='15'/></td></tr>";
        html += "</table></form>";
        server.sendContent(html);
        delay(10);
        yield();
        html = ""; // ne brisi
        html += "<button class=\"g\" form=\"commit\" type=\"submit\">SAVE</button>";
        //html += "<a href=\"/save\"><button class=\"g\">SAVE</button></a>";
        html += "<a href=\"/\"><button class=\"g\">BACK</button></a>";
        html += "<a href=\"/disap\"><button class=\"g\">ONLY STATION MODE</button></a>";
        html += "<a href=\"/enapsta\"><button class=\"g\">AP + STATION MODE</button></a>";
        html += "<a href=\"/recon\"><button class=\"g\">RECONNECT</button></a>";
        html += "<a href=\"/default\"><button class=\"g\">LOAD DEFAULTS</button></a>";
        html += "<a href=\"/settings\"><button class=\"g\">REFRESH</button></a>";
        html += "<a href=\"/espupdate\"><button class=\"g\">OTA UPDATE</button></a>";
        html += "<a href=\"/reboot\"><button class=\"g\">REBOOT</button></a>";
        html += "<a href=\"/crashinfo\"><button class=\"g\">VIEW CRASH INFO</button></a>";
        html += "<a href=\"/clearcrash\"><button class=\"g\">CLEAR CRASH INFO</button></a>";
        html += "<a href=\"/ntpupdate\"><button class=\"g\">NTP UPDATE</button></a>";
        html += "</body></html>\n";
        server.sendContent(html); // poslje paket
        delay(10);
        yield();
        server.client().stop(); // sporocimo clientu, da smo poslali vse!
        yield();
    }

    void handle_json() {
      String json;
      json += "{";
      if (SETTINGS.DS18B20_pin != Not_used_pin_number) { // ce je pin=100 potem je disabled
        for (int n = 0; n < SETTINGS.DS18B20_num_configured; n++) {
          if (!isnan(DS18B20_All_Sensor_Values[n])) {
            json += "\"" + String(SETTINGS.DS18B20_ident[n]) + "temp\": \"" + String(DS18B20_All_Sensor_Values[n]) + "\", ";
          }
        }
      }
      for (int n = 0; n < SETTINGS.DHT_num_configured; n++) {
        if (SETTINGS.DHT_pin[n] != Not_used_pin_number) { // ce je pin=100 potem je disabled
          if (!isnan(DHT_All_Sensor_Values_Temp[n]) && !isnan(DHT_All_Sensor_Values_Hum[n])) {
            json += "\"" + String(SETTINGS.DHT_ident[n]) + "temp\": \"" + String(DHT_All_Sensor_Values_Temp[n]) + "\", ";
            json += "\"" + String(SETTINGS.DHT_ident[n]) + "hum\": \"" + String(DHT_All_Sensor_Values_Hum[n]) + "\", ";
            json += "\"" + String(SETTINGS.DHT_ident[n]) + "heat\": \"" + String(DHT_All_Sensor_Values_Heat[n]) + "\", ";
            json += "\"" + String(SETTINGS.DHT_ident[n]) + "dew\": \"" + String(DHT_All_Sensor_Values_Dew[n]) + "\", ";
          }
        }
      }
      for (int n = 0; n < SETTINGS.BME280_num_configured; n++) {
        if (SETTINGS.BME280_pin_sda[n] != Not_used_pin_number) { // ce je pin=100 potem je disabled
          if (!isnan(BME280_All_Sensor_Values_Temp[n])) {
            json += "\"" + String(SETTINGS.BME280_ident[n]) + "temp\": \"" + String(BME280_All_Sensor_Values_Temp[n]) + "\", ";
            json += "\"" + String(SETTINGS.BME280_ident[n]) + "hum\": \"" + String(BME280_All_Sensor_Values_Hum[n]) + "\", ";
            json += "\"" + String(SETTINGS.BME280_ident[n]) + "press\": \"" + String(BME280_All_Sensor_Values_Press[n]) + "\", ";
            json += "\"" + String(SETTINGS.BME280_ident[n]) + "heat\": \"" + String(BME280_All_Sensor_Values_Heat[n]) + "\", ";
            json += "\"" + String(SETTINGS.BME280_ident[n]) + "dew\": \"" + String(BME280_All_Sensor_Values_Dew[n]) + "\", ";
          }
        }
      }
      for (int n = 0; n < SETTINGS.MAX6675_num_configured; n++) {
        if (SETTINGS.MAX6675_pin_clk[n] != Not_used_pin_number) { // ce je pin=100 potem je disabled
          json += "\"" + String(SETTINGS.MAX6675_ident[n]) + "temp\": \"" + String(MAX6675_All_Sensor_Values_Temp[n]) + "\", ";
        }
      }
      for (int n = 0; n < SETTINGS.Ultrasonic_num_configured; n++) {
        if (SETTINGS.Ultrasonic_pin_trigger[n] != Not_used_pin_number) { // ce je pin=100 potem je disabled
          json += "\"" + String(SETTINGS.Ultrasonic_ident[n]) + "cm\": \"" + String(Ultrasonic_All_Sensor_Values_Distance[n]) + "\", ";
        }
      }
      json += "\"wifi\": \"" + String(WiFi.RSSI()) + "\", ";
      json += "\"wifi_status\": \"" + String(WiFi.status()) + "\", ";
      json += "\"uptime\": \"" + String((unsigned long)millis()) + "\", ";
      json += "\"freeheap\": \"" + String(ESP.getFreeHeap()) + "\", ";
      json += "\"vcc\": \"" + String((float)ESP.getVcc()/890) + "\"";
      json += "}";
      server.send(200, "text/plain", json);
      yield();
    }

    void http_WiFi_Scan() {
      // WiFi.scanNetworks(async, show_hidden) 
      // asysnc - if set to true then scanning will start in background and function will exit without waiting for result. To check for result use separate function scanComplete that is described below.
      // show_hidden - set it to true to include in scan result networks with hidden SSID.
      WiFi.scanNetworks(true, true);
      String html = BackURLSystemSettings;
      server.send(200, "text/html", html);
    }

    // preskenira wifi in vrne String pull down networkov
    String http_print_wifi_networks() {
      String DropDown = "<select name=\"ssid\">";
      int n = WiFi.scanComplete();
      String MyWiFiNetworks;
      if (n == 0)
        SendLog(" INFO: Wifi scan done, no networks found");
      else
      {
        for (int i = 0; i < n; ++i) {
          // Print SSID and RSSI for each network found
          if (WiFi.SSID(i) == SETTINGS.ssid) { // že izbran ssid
            DropDown += "<option value=\"" + String(WiFi.SSID(i)) + "\" selected>" + String(WiFi.SSID(i)) +" ("+ String(WiFi.RSSI(i)) +"dBm)</option>";
          }
          else {
            DropDown += "<option value=\"" + String(WiFi.SSID(i)) + "\">" + String(WiFi.SSID(i)) +" ("+ String(WiFi.RSSI(i)) +"dBm)</option>"; 
          }
          MyWiFiNetworks += String(WiFi.SSID(i)) + "("+ String(WiFi.RSSI(i)) + "dBm) ";
        }
        DropDown += "</select>";
      }
      delay(5);
      SendLog(" INFO: Wifi scan done, found: " + MyWiFiNetworks);
      return DropDown;
    }

    // popravi izpis 0 -> 00 oz 5 -> 05
    String print_time(int num) {
      if (num < 10) {
        return "0" + String(num);
      }
      if (num < 0 || num > 59) {
        return "00";
      }
      return String(num);
    }


    // izpise nastavitve DS18B20
    String http_DS18B20_config(void) {
      String html = "<table><tr><th colspan=\"6\"><form method='get' action='dsset'>DS18B20 sensor settings, pin:" + http_pin_dropdown("dspin", SETTINGS.DS18B20_pin) + "<button type='submit' class=\"d\">Set</button></form></th></tr>";
      html += "<tr><th>#</th><th>identificator</th><th>address</th><th>resolution</th><th>value</th><th>idx</th></tr>";
      for(int n = 0; n < SETTINGS.DS18B20_num_configured; n++) {
        html += "<tr><td>" + String(n) + "</td><td>" + String(SETTINGS.DS18B20_ident[n]) + "</td><td>" + DS18B20_Print_Address(SETTINGS.DS18B20_address[n]) + "</td><td>" + String(SETTINGS.DS18B20_resolution[n]) + "</td><td>" + String(DS18B20_All_Sensor_Values[n]) + "°C</td><td>" + String(SETTINGS.DS18B20_domoticz_idx[n]) + "</td></tr>";
      }
      //html += "</table>"; // nadaljujem s formo za vnos!!!
      return html;
    }
    
   // Kreira dropdown meni za pin-e
    String http_pin_dropdown(String Name, int Pin) {     
      String DropDown = "<select name=\"" + String(Name) + "\">";
      // pin-i:
      // D0=16, D1=5, D2=4, D3=0, D4=2, D5=14, D6=12, D7=13, D8=15, D9=3, D10=1
      // D3=0, D10=1, D4=2, D9=3, D2=4, D1=5, D6=12, D7=13, D5=14, D8=15, D0=16
      switch (Pin) {
        case 0:
         DropDown += "<option value=\"0\" selected>D3</option><option value=\"1\">D10</option><option value=\"2\">D4</option><option value=\"3\">D9</option><option value=\"4\">D2</option><option value=\"5\">D1</option><option value=\"12\">D6</option><option value=\"13\">D7</option><option value=\"14\">D5</option><option value=\"15\">D8</option><option value=\"16\">D0</option><option value=\""+ String(Not_used_pin_number) +"\">NC</option>";
          break;
        case 1:
         DropDown += "<option value=\"0\">D3</option><option value=\"1\" selected>D10</option><option value=\"2\">D4</option><option value=\"3\">D9</option><option value=\"4\">D2</option><option value=\"5\">D1</option><option value=\"12\">D6</option><option value=\"13\">D7</option><option value=\"14\">D5</option><option value=\"15\">D8</option><option value=\"16\">D0</option><option value=\""+ String(Not_used_pin_number) +"\">NC</option>";
          break;
        case 2:
         DropDown += "<option value=\"0\">D3</option><option value=\"1\">D10</option><option value=\"2\" selected>D4</option><option value=\"3\">D9</option><option value=\"4\">D2</option><option value=\"5\">D1</option><option value=\"12\">D6</option><option value=\"13\">D7</option><option value=\"14\">D5</option><option value=\"15\">D8</option><option value=\"16\">D0</option><option value=\""+ String(Not_used_pin_number) +"\">NC</option>";
          break;
        case 3:
         DropDown += "<option value=\"0\">D3</option><option value=\"1\">D10</option><option value=\"2\">D4</option><option value=\"3\" selected>D9</option><option value=\"4\">D2</option><option value=\"5\">D1</option><option value=\"12\">D6</option><option value=\"13\">D7</option><option value=\"14\">D5</option><option value=\"15\">D8</option><option value=\"16\">D0</option><option value=\""+ String(Not_used_pin_number) +"\">NC</option>";
          break;
        case 4:
         DropDown += "<option value=\"0\">D3</option><option value=\"1\">D10</option><option value=\"2\">D4</option><option value=\"3\">D9</option><option value=\"4\" selected>D2</option><option value=\"5\">D1</option><option value=\"12\">D6</option><option value=\"13\">D7</option><option value=\"14\">D5</option><option value=\"15\">D8</option><option value=\"16\">D0</option><option value=\""+ String(Not_used_pin_number) +"\">NC</option>";
          break;
        case 5:
         DropDown += "<option value=\"0\">D3</option><option value=\"1\">D10</option><option value=\"2\">D4</option><option value=\"3\">D9</option><option value=\"4\">D2</option><option value=\"5\" selected>D1</option><option value=\"12\">D6</option><option value=\"13\">D7</option><option value=\"14\">D5</option><option value=\"15\">D8</option><option value=\"16\">D0</option><option value=\""+ String(Not_used_pin_number) +"\">NC</option>";
          break;
        case 12:
         DropDown += "<option value=\"0\">D3</option><option value=\"1\">D10</option><option value=\"2\">D4</option><option value=\"3\">D9</option><option value=\"4\">D2</option><option value=\"5\">D1</option><option value=\"12\" selected>D6</option><option value=\"13\">D7</option><option value=\"14\">D5</option><option value=\"15\">D8</option><option value=\"16\">D0</option><option value=\""+ String(Not_used_pin_number) +"\">NC</option>";
          break;
        case 13:
         DropDown += "<option value=\"0\">D3</option><option value=\"1\">D10</option><option value=\"2\">D4</option><option value=\"3\">D9</option><option value=\"4\">D2</option><option value=\"5\">D1</option><option value=\"12\">D6</option><option value=\"13\" selected>D7</option><option value=\"14\">D5</option><option value=\"15\">D8</option><option value=\"16\">D0</option><option value=\""+ String(Not_used_pin_number) +"\">NC</option>";
          break;
        case 14:
         DropDown += "<option value=\"0\">D3</option><option value=\"1\">D10</option><option value=\"2\">D4</option><option value=\"3\">D9</option><option value=\"4\">D2</option><option value=\"5\">D1</option><option value=\"12\">D6</option><option value=\"13\">D7</option><option value=\"14\" selected>D5</option><option value=\"15\">D8</option><option value=\"16\">D0</option><option value=\""+ String(Not_used_pin_number) +"\">NC</option>";
          break;
        case 15:
         DropDown += "<option value=\"0\">D3</option><option value=\"1\">D10</option><option value=\"2\">D4</option><option value=\"3\">D9</option><option value=\"4\">D2</option><option value=\"5\">D1</option><option value=\"12\">D6</option><option value=\"13\">D7</option><option value=\"14\">D5</option><option value=\"15\" selected>D8</option><option value=\"16\">D0</option><option value=\""+ String(Not_used_pin_number) +"\">NC</option>";
          break;
        case 16:
         DropDown += "<option value=\"0\">D3</option><option value=\"1\">D10</option><option value=\"2\">D4</option><option value=\"3\">D9</option><option value=\"4\">D2</option><option value=\"5\">D1</option><option value=\"12\">D6</option><option value=\"13\">D7</option><option value=\"14\">D5</option><option value=\"15\">D8</option><option value=\"16\" selected>D0</option><option value=\""+ String(Not_used_pin_number) +"\">NC</option>";
          break;
        default:
         DropDown += "<option value=\"0\">D3</option><option value=\"1\">D10</option><option value=\"2\">D4</option><option value=\"3\">D9</option><option value=\"4\">D2</option><option value=\"5\">D1</option><option value=\"12\">D6</option><option value=\"13\">D7</option><option value=\"14\">D5</option><option value=\"15\">D8</option><option value=\"16\">D0</option><option value=\""+ String(Not_used_pin_number) +"\" selected>NC</option>";
        break;
      }
      DropDown += "</select>";
      return DropDown;
    }

    String Pin_num_to_name(int Pin) {
      // D3=0, D10=1, D4=2, D9=3, D2=4, D1=5, D6=12, D7=13, D5=14, D8=15, D0=16
      String Name;
      switch (Pin) {
        case 0:
         Name = "D3";
          break;
        case 1:
         Name = "D10";
          break;
        case 2:
         Name = "D4";
          break;  
        case 3:
         Name = "D9";
          break;
        case 4:
         Name = "D2";
          break;
        case 5:
         Name = "D1";
          break;
        case 12:
         Name = "D6";
          break;  
        case 13:
         Name = "D7";
          break; 
        case 14:
         Name = "D5";
          break;
        case 15:
         Name = "D8";
          break;
        case 16:
         Name = "D0";
          break;  
        case Not_used_pin_number:
         Name = "NC";
          break; 
        default:
         Name = "NC";
          break;
      }
      return Name;
    }
    
   // Kreira dropdown meni za resolucijo
    String http_DS18B20_resolution_dropdown(String Name) {   
      // resolution: 9bit, 10bit, 11bit, 12bit  
      String DropDown = "<select name=\"" + String(Name) + "\"><option value=\"9\">9bit</option><option value=\"10\" selected>10bit</option><option value=\"11\">11bit</option><option value=\"12\">12bit</option></select>";
      return DropDown;
    }

    // izpise nastavitve DHT
    String http_DHT_config(void) {
      String html = "<table><tr><th colspan=\"6\">DHT sensor settings</th></tr>";
      html += "<tr><th>#</th><th>identificator</th><th>pin</th><th>type</th><th>value</th><th>idx</th></tr>";
      for(int n = 0; n < SETTINGS.DHT_num_configured; n++) {
        html += "<tr><td>" + String(n) + "</td><td>" + String(SETTINGS.DHT_ident[n]) + "</td><td>" + Pin_num_to_name(SETTINGS.DHT_pin[n]) + "</td><td>" + DHT_type_to_name(SETTINGS.DHT_type[n]) + "</td><td>" + String(DHT_All_Sensor_Values_Temp[n]) + "°C/" + String(DHT_All_Sensor_Values_Hum[n]) + "%</td><td>" + String(SETTINGS.DHT_domoticz_idx[n]) + "</td></tr>";
      }
      //html += "</table>"; // nadaljujem s formo za vnos!!!
      return html;
    }
    
   // Kreira dropdown meni za type
    String http_DHT_type(String Name) {   
      String DropDown = "<select name=\"" + String(Name) + "\"><option value=\"11\">DHT11</option><option value=\"22\" selected>DHT22</option><option value=\"21\">DHT21/AM2302</option></select>";
      return DropDown;
    }

   // Kreira dropdown meni za type
    String DHT_type_to_name(int type) {
      String Name;   
      switch (type) {
          case 11:
           Name = "DHT11";
            break;
          case 22:
           Name = "DHT22";
            break;
          case 21:
           Name = "DHT21/AM2302";
      }
      return Name;
    }

    // izpise nastavitve BME280
    String http_BME280_config(void) {
      String html = "<table><tr><th colspan=\"6\">BME280 sensor settings</th></tr>";
      html += "<tr><th>#</th><th>identificator</th><th>pin SDA</th><th>pin SCL</th><th>value</th><th>idx</th></tr>";
      for(int n = 0; n < SETTINGS.BME280_num_configured; n++) {
        html += "<tr><td>" + String(n) + "</td><td>" + String(SETTINGS.BME280_ident[n]) + "</td><td>" + Pin_num_to_name(SETTINGS.BME280_pin_sda[n]) + "</td><td>" + Pin_num_to_name(SETTINGS.BME280_pin_scl[n]) + "</td><td>" + String(BME280_All_Sensor_Values_Temp[n]) + "°C/" + String(BME280_All_Sensor_Values_Hum[n]) + "%/" + String(BME280_All_Sensor_Values_Press[n]) + "mBar</td><td>" + String(SETTINGS.BME280_domoticz_idx[n]) + "</td></tr>";
      }
      //html += "</table>"; // nadaljujem s formo za vnos!!!
      return html;
    }

    // izpise nastavitve MAX6675
    String http_MAX6675_config(void) {
      String html = "<table><tr><th colspan=\"7\">MAX6675 K-type sensor settings</th></tr>";
      html += "<tr><th>#</th><th>identificator</th><th>pin CLK</th><th>pin CS</th><th>pin DO</th><th>value</th><th>idx</th></tr>";
      for(int n = 0; n < SETTINGS.MAX6675_num_configured; n++) {
        html += "<tr><td>" + String(n) + "</td><td>" + String(SETTINGS.MAX6675_ident[n]) + "</td><td>" + Pin_num_to_name(SETTINGS.MAX6675_pin_clk[n]) + "</td><td>" + Pin_num_to_name(SETTINGS.MAX6675_pin_cs[n]) + "</td><td>" + Pin_num_to_name(SETTINGS.MAX6675_pin_do[n]) + "</td><td>" + String(MAX6675_All_Sensor_Values_Temp[n]) + "°C</td><td>" + String(SETTINGS.MAX6675_domoticz_idx[n]) + "</td></tr>";
      }
      //html += "</table>"; // nadaljujem s formo za vnos!!!
      return html;
    }
   
    int domoticz_hum_stat_converter(float h) {
      if ( h > 70 ) {
        return 3;
      } else if ( h < 30 ) {
        return 2; 
      } else if ( h >= 30 & h <= 45 ) {
        return 0;
      } else if ( h > 45 & h <= 70 ) {
        return 1;
      } 
    }

    int domoticz_press_stat_converter(float pa) {
      if ( pa > 1030 ) {
        return 1;  
      } else if ( pa > 1010 & pa <= 1030 ) {
        return 2;
      } else if ( pa > 990 & pa <= 1010 ) {
        return 3;
      } else if ( pa > 970 & pa < 990 ) {
        return 4;
      }
    }
   
    // domoticz update client
    void domoticz_update_client() {
      if (SETTINGS.domoticz_update_enabled) {
        if (millis() - previousDomoticzUpdate > SETTINGS.domoticz_update_interval * 1000) {
          previousDomoticzUpdate = millis();
          yield();
          if (SETTINGS.domoticz_idx_voltage != 0) {
            String url = "/json.htm?type=command&param=udevice&idx=";
            url += SETTINGS.domoticz_idx_voltage;
            url += "&nvalue=0&svalue=";
            url += String((float)ESP.getVcc()/890);
            domoticz_update(url);
          }
          if (SETTINGS.domoticz_idx_uptime != 0) {
            String url = "/json.htm?type=command&param=udevice&idx=";
            url += SETTINGS.domoticz_idx_uptime;
            url += "&nvalue=0&svalue=";
            url += String((unsigned long)millis());
            domoticz_update(url);
          }
          if (SETTINGS.domoticz_idx_wifi != 0) {
            String url = "/json.htm?type=command&param=udevice&idx=";
            url += SETTINGS.domoticz_idx_wifi;
            url += "&nvalue=0&svalue=";
            url += String(WiFi.RSSI());
            domoticz_update(url);
          }                    
          for(int n = 0; n < SETTINGS.DS18B20_num_configured; n++) {
            // temp domoticz api url: /json.htm?type=command&param=udevice&idx=IDX&nvalue=0&svalue=TEMP
            if (!isnan(DS18B20_All_Sensor_Values[n])) {
              String url = "/json.htm?type=command&param=udevice&idx=";
              url += SETTINGS.DS18B20_domoticz_idx[n];
              url += "&nvalue=0&svalue=";
              url += String(DS18B20_All_Sensor_Values[n]);
              domoticz_update(url);
            }
         }
          for(int n = 0; n < SETTINGS.DHT_num_configured; n++) {
            // temp/hum domoticz api url: /json.htm?type=command&param=udevice&idx=IDX&nvalue=0&svalue=TEMP;HUM;HUM_STAT
            if (!isnan(DHT_All_Sensor_Values_Temp[n]) && !isnan(DHT_All_Sensor_Values_Hum[n])) {
              String url = "/json.htm?type=command&param=udevice&idx=";
              url += SETTINGS.DHT_domoticz_idx[n];
              url += "&nvalue=0&svalue=";
              url += String(DHT_All_Sensor_Values_Temp[n]);
              url += ";";
              url += String(DHT_All_Sensor_Values_Hum[n]);
              url += ";";
              url += String(domoticz_hum_stat_converter(DHT_All_Sensor_Values_Hum[n]));
              domoticz_update(url);
           }
         }
          for(int n = 0; n < SETTINGS.BME280_num_configured; n++) {
            // temp/hum/bar domoticz api url: /json.htm?type=command&param=udevice&idx=IDX&nvalue=0&svalue=TEMP;HUM;HUM_STAT;BAR;BAR_FOR
            if (!isnan(BME280_All_Sensor_Values_Temp[n])) {
              String url = "/json.htm?type=command&param=udevice&idx=";
              url += SETTINGS.BME280_domoticz_idx[n];
              url += "&nvalue=0&svalue=";
              url += String(BME280_All_Sensor_Values_Temp[n]);
              url += ";";
              url += String(BME280_All_Sensor_Values_Hum[n]);
              url += ";";
              url += String(domoticz_hum_stat_converter(BME280_All_Sensor_Values_Hum[n]));
              url += ";";
              url += String(BME280_All_Sensor_Values_Press[n]);
              url += ";";
              url += String(domoticz_press_stat_converter(BME280_All_Sensor_Values_Press[n]));
              domoticz_update(url);
           }
         }
          for(int n = 0; n < SETTINGS.MAX6675_num_configured; n++) {
            // temp domoticz api url: /json.htm?type=command&param=udevice&idx=IDX&nvalue=0&svalue=TEMP
            if (!isnan(MAX6675_All_Sensor_Values_Temp[n])) {
              String url = "/json.htm?type=command&param=udevice&idx=";
              url += SETTINGS.MAX6675_domoticz_idx[n];
              url += "&nvalue=0&svalue=";
              url += String(MAX6675_All_Sensor_Values_Temp[n]);
              domoticz_update(url);
            }
         }
          for(int n = 0; n < SETTINGS.Ultrasonic_num_configured; n++) {
            // Distance sensor api url: /json.htm?type=command&param=udevice&idx=IDX&nvalue=0&svalue=DISTANCE
            String url = "/json.htm?type=command&param=udevice&idx=";
            url += SETTINGS.Ultrasonic_domoticz_idx[n];
            url += "&nvalue=0&svalue=";
            url += String(Ultrasonic_All_Sensor_Values_Distance[n]);
            domoticz_update(url);
         }
       }
     }
   }
   
    // domoticz update client
    void domoticz_update(String url) {
      WiFiClient domoticz_client;
      domoticz_client.setTimeout(100);
      if (SETTINGS.domoticz_update_enabled) {
        if (!domoticz_client.connect(SETTINGS.domoticz_ip, SETTINGS.domoticz_port)) {
          SendLog(" ERROR: domoticz (" + String(SETTINGS.domoticz_ip) + ":" + String(SETTINGS.domoticz_port) + ") connection failed.");
          return;
        }
        yield();
        domoticz_client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + SETTINGS.domoticz_ip + "\r\n" + "Connection: close\r\n\r\n");
        domoticz_client.stop();
        domoticz_client.flush();
      }
    }

//    // domoticz update client
//    void domoticz_update(String url) {
//      if (WiFi.status() == WL_CONNECTED) {
//        if (SETTINGS.domoticz_update_enabled) {
//          if (!domoticz_client.connect(SETTINGS.domoticz_ip, SETTINGS.domoticz_port)) {
//            //Serial.println("connection failed");
//            SendLog(" ERROR: Error connecting to domoticz."); 
//            return;
//          }
//          //Serial.print("Requesting URL: ");
//          //Serial.println(url);
//          // This will send the request to the server
//          domoticz_client.print(String("GET ") + url + " HTTP/1.1\r\n" +
//                       "Host: " + SETTINGS.domoticz_ip + "\r\n" + 
//                       "Connection: close\r\n\r\n");
//          unsigned long timeout = millis();
//          while (domoticz_client.available() == 0) {
//            if (millis() - timeout > 5000) {
//              //Serial.println(">>> Client Timeout !");
//              SendLog(" ERROR: domoticz connection timeout.");
//              domoticz_client.stop();
//              return;
//            }
//          }
//        }
//      }
//    }   

   // Kreira dropdown meni za IO mode
    String http_IO_mode (String Name) {   
      // 1 = INPUT, 2 = INPUT_PULLUP, 3 = INPUT_PULLDOWN_16, 4 = OUTPUT (TOGGLE BUTTON), 5 = OUTPUT (PUSH BUTTON), 6 = OUTPUT (INVERTED PUSH BUTTON)
      String DropDown = "<select name=\"" + String(Name) + "\"><option value=\"1\">INPUT</option><option value=\"2\" selected>INPUT_PULLUP</option><option value=\"3\">INPUT_PULLDOWN_16</option><option value=\"4\">OUTPUT (TOGGLE BUTTON)</option><option value=\"5\">OUTPUT (PUSH BUTTON)</option><option value=\"6\">OUTPUT (INVERTED PUSH BUTTON)</option></select>";
      return DropDown;
    }

   // Kreira dropdown meni za type
    String IO_mode_num_to_name(int type) {
      // 1 = INPUT, 2 = INPUT_PULLUP, 3 = INPUT_PULLDOWN_16, 4 = OUTPUT (TOGGLE BUTTON), 5 = OUTPUT (PUSH BUTTON), 6 = OUTPUT (INVERTED PUSH BUTTON), 
      String Name;   
      switch (type) {
          case 1:
           Name = "INPUT";
            break;
          case 2:
           Name = "INPUT_PULLUP";
            break;
          case 3:
           Name = "INPUT_PULLDOWN_16";
             break;
          case 4:
           Name = "OUTPUT (TOGGLE BUTTON)";
             break;
          case 5:
           Name = "OUTPUT (PUSH BUTTON)";
             break;
          case 6:
           Name = "OUTPUT (INVERTED PUSH BUTTON)";
             break;
      }
      return Name;
    }

    void IO_init_ports() {
      for (int n = 0; n < SETTINGS.IO_num_configured; n++) {
        if (SETTINGS.IO_pin[n] != Not_used_pin_number) { // ce je pin=100 potem je disabled
          // 1 = INPUT, 2 = INPUT_PULLUP, 3 = INPUT_PULLDOWN_16, 4 = OUTPUT (TOGGLE BUTTON), 5 = OUTPUT (PUSH BUTTON), 
          if (SETTINGS.IO_mode[n] == 1) {
            pinMode(SETTINGS.IO_pin[n], INPUT);
          }
          else if (SETTINGS.IO_mode[n] == 2) {
            pinMode(SETTINGS.IO_pin[n], INPUT_PULLUP);
          }
          else if (SETTINGS.IO_mode[n] == 3) {
            pinMode(SETTINGS.IO_pin[n], INPUT_PULLDOWN_16);
          }
          else { // Write mode
            pinMode(SETTINGS.IO_pin[n], OUTPUT);
          }
        }
      }
    }

    void IO_set_pin_to_low_after_sometime() {
      for (int n = 0; n < SETTINGS.IO_num_configured; n++) {
        if (SETTINGS.IO_pin[n] != Not_used_pin_number) { // ce je pin=100 potem je disabled
          // 1 = INPUT, 2 = INPUT_PULLUP, 3 = INPUT_PULLDOWN_16, 4 = OUTPUT (TOGGLE BUTTON), 5 = OUTPUT (PUSH BUTTON), 6 = OUTPUT (INVERTED PUSH BUTTON)
          if (SETTINGS.IO_mode[n] == 5 && digitalRead(SETTINGS.IO_pin[n]) == 1) {
            if (millis() - previousIOmillis[n] >= SETTINGS.IO_timer[n] * 1000) {
              digitalWrite(SETTINGS.IO_pin[n], 0); 
            }
          }
          if (SETTINGS.IO_mode[n] == 6 && digitalRead(SETTINGS.IO_pin[n]) == 0) { // 6 = OUTPUT (INVERTED PUSH BUTTON)
            if (millis() - previousIOmillis[n] >= SETTINGS.IO_timer[n] * 1000) {
              digitalWrite(SETTINGS.IO_pin[n], 1); 
            }
          }
        }
      }
    }

    // izpise nastavitve IO porte
    String http_IO_config(void) {
      String html = "<table><tr><th colspan=\"7\">IO settings</th></tr>";
      html += "<tr><th>#</th><th>identificator</th><th>pin</th><th>mode</th><th>push button time[sec]</th><th>value</th><th>idx</th></tr>";
      for(int n = 0; n < SETTINGS.IO_num_configured; n++) {
        html += "<tr><td>" + String(n) + "</td><td>" + String(SETTINGS.IO_ident[n]) + "</td><td>" + Pin_num_to_name(SETTINGS.IO_pin[n]) + "</td><td>" + IO_mode_num_to_name(SETTINGS.IO_mode[n]) + "</td><td>" + String(SETTINGS.IO_timer[n]) + "</td><td>" + String(digitalRead(SETTINGS.IO_pin[n])) + "</td><td>" + String(SETTINGS.IO_domoticz_idx[n]) + "</td></tr>";
      }
      //html += "</table>"; // nadaljujem s formo za vnos!!!
      return html;
    }

   void http_IO_remove_pin(void) {
      if (SETTINGS.IO_num_configured > 0) {
        SETTINGS.IO_num_configured--; 
      }
      String html = BackURLSystemSettings;
      server.send(200, "text/html", html);      
    }

    // izpise nastavitve Ultrasonic porte
    String http_Ultrasonic_config(void) {
      String html = "<table><tr><th colspan=\"8\">Ultrasonic sensor settings</th></tr>";
      html += "<tr><th>#</th><th>identificator</th><th>pin sig/trig</th><th>pin echo</th><th>max distance[cm]</th><th>offset[cm]</th><th>value</th><th>idx</th></tr>";
      for(int n = 0; n < SETTINGS.Ultrasonic_num_configured; n++) {
        html += "<tr><td>" + String(n) + "</td><td>" + String(SETTINGS.Ultrasonic_ident[n]) + "</td><td>" + Pin_num_to_name(SETTINGS.Ultrasonic_pin_trigger[n]) + "</td><td>" + Pin_num_to_name(SETTINGS.Ultrasonic_pin_echo[n]) + "</td><td>" + String(SETTINGS.Ultrasonic_max_distance[n]) + "</td><td>" + String(SETTINGS.Ultrasonic_offset[n]) + "</td><td>" + String(Ultrasonic_All_Sensor_Values_Distance[n]) + "</td><td>" + String(SETTINGS.Ultrasonic_domoticz_idx[n]) + "</td></tr>";
      }
      //html += "</table>"; // nadaljujem s formo za vnos!!!
      return html;
    }
    
    void Ultrasonic_read_data() {
      for (int n = 0; n < SETTINGS.Ultrasonic_num_configured; n++) {
        if (SETTINGS.Ultrasonic_pin_trigger[n] != Not_used_pin_number) {
          NewPing sonar(SETTINGS.Ultrasonic_pin_trigger[n], SETTINGS.Ultrasonic_pin_echo[n], SETTINGS.Ultrasonic_max_distance[n]); // NewPing setup of pins and maximum distance.
//          Serial.print("Ping: ");
//          Serial.print(sonar.ping_cm()); // Send ping, get distance in cm and print result (0 = outside set distance range)
//          Serial.println("cm");
          Ultrasonic_All_Sensor_Values_Distance[n] = abs(SETTINGS.Ultrasonic_offset[n] - sonar.ping_cm());
        }
      }
    }

   void http_Ultrasonic_remove_pin(void) {
      if (SETTINGS.Ultrasonic_num_configured > 0) {
        SETTINGS.Ultrasonic_num_configured--; 
      }
      String html = BackURLSystemSettings;
      server.send(200, "text/html", html);      
    }


    void mqtt_init() {
      MQTT_Client.setTimeout(50);
      // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported by Arduino.
      // You need to set the IP address directly.
      mqtt_client.begin(SETTINGS.mqtt_server, SETTINGS.mqtt_port, MQTT_Client);
      mqtt_client.onMessage(mqtt_receive_data);
      mqtt_connect();
    }

    void mqtt_connect() {
      if (SETTINGS.mqtt_enabled) {
        if (!mqtt_client.connect(SETTINGS.hostname, SETTINGS.mqtt_key ,SETTINGS.mqtt_secret)) {
          if (millis() - previousMQTTReconnect > 3000) {
            previousMQTTReconnect = millis();
            yield();
            mqtt_client.connect(SETTINGS.hostname, SETTINGS.mqtt_key ,SETTINGS.mqtt_secret);
            //Serial.print("mqtt-connecting...");
            SendLog(" ERROR: mqtt connecting...");
          }
        }
        else {
          for(int n = 0; n < SETTINGS.IO_num_configured; n++) { // narocim se na IO porte!!!
            mqtt_client.subscribe(SETTINGS.IO_ident[n]);
          }
        }
      }
    }

    void mqtt_loop() {
      if (SETTINGS.mqtt_enabled) {
        mqtt_client.loop();
        delay(10);  // <- fixes some issues with WiFi stability
        if (!mqtt_client.connected()) {
          mqtt_connect();
        }  
      }
    }
    
    void mqtt_receive_data(String &topic, String &payload) {
      //Serial.println("incoming: " + topic + " - " + payload);
      for(int n = 0; n < SETTINGS.IO_num_configured; n++) {
        if (SETTINGS.IO_mode[n] > 3) { // only OUTPUT ports
          if (topic == String(SETTINGS.IO_ident[n])) {
            if (SETTINGS.IO_mode[n] == 5 || SETTINGS.IO_mode[n] == 6) {
              previousIOmillis[n] = millis(); //PUSH button activation
            }
            if (payload.toInt() == 1 ) { 
              digitalWrite(SETTINGS.IO_pin[n], true);
            }
            if (payload.toInt() == 0 ) { 
              digitalWrite(SETTINGS.IO_pin[n], false);
            }          
          }
        }
      }
    }

    void mqtt_send_log(String log_msg) {
      if (SETTINGS.mqtt_enabled) {
        String MyTopic = SETTINGS.hostname + String("/system/log");
        mqtt_client.publish(MyTopic.c_str(), log_msg);
      }
    }


    void mqtt_send_data() {
      if (SETTINGS.mqtt_enabled) {
        if (millis() - previousMQTTInterval >= SETTINGS.mqtt_interval * 1000) {
          previousMQTTInterval = millis();
          char myfloat_in_Char[8];
          String MyTopic = SETTINGS.hostname + String("/system/voltage");
          dtostrf((float)ESP.getVcc()/890, 6, 2, myfloat_in_Char); 
          mqtt_client.publish(MyTopic.c_str(), myfloat_in_Char);
          MyTopic = SETTINGS.hostname + String("/system/uptime");
          dtostrf((unsigned long)millis(), 6, 0, myfloat_in_Char); 
          mqtt_client.publish(MyTopic.c_str(), myfloat_in_Char);
          MyTopic = SETTINGS.hostname + String("/system/wifi");
          dtostrf(WiFi.RSSI(), 6, 2, myfloat_in_Char); 
          mqtt_client.publish(MyTopic.c_str(), myfloat_in_Char);   
          MyTopic = SETTINGS.hostname + String("/system/freeheap");
          dtostrf(ESP.getFreeHeap(), 6, 0, myfloat_in_Char); 
          mqtt_client.publish(MyTopic.c_str(), myfloat_in_Char);        
          for (int n = 0; n < SETTINGS.IO_num_configured; n++) {
            if (SETTINGS.IO_pin[n] != Not_used_pin_number) { // ce je pin=100 potem je disabled
              MyTopic = SETTINGS.hostname + String("/") + SETTINGS.IO_ident[n] + String("/state");
              dtostrf(digitalRead(SETTINGS.IO_pin[n]), 6, 2, myfloat_in_Char); 
              mqtt_client.publish(MyTopic.c_str(), myfloat_in_Char);
            }
          }
          if (SETTINGS.DS18B20_pin != Not_used_pin_number) { // ce je pin=100 potem je disabled
            for (int n = 0; n < SETTINGS.DS18B20_num_configured; n++) {
              if (!isnan(DS18B20_All_Sensor_Values[n])) {
                MyTopic = SETTINGS.hostname + String("/") + SETTINGS.DS18B20_ident[n] + String("/temperature");
                dtostrf(DS18B20_All_Sensor_Values[n], 6, 2, myfloat_in_Char); 
                mqtt_client.publish(MyTopic.c_str(), myfloat_in_Char);
              }
            }
          }
          for (int n = 0; n < SETTINGS.DHT_num_configured; n++) {
            if (SETTINGS.DHT_pin[n] != Not_used_pin_number) { // ce je pin=100 potem je disabled
              if (!isnan(DHT_All_Sensor_Values_Temp[n]) && !isnan(DHT_All_Sensor_Values_Hum[n])) {
                MyTopic = SETTINGS.hostname + String("/") + SETTINGS.DHT_ident[n] + String("/temperature");
                dtostrf(DHT_All_Sensor_Values_Temp[n], 6, 2, myfloat_in_Char); 
                mqtt_client.publish(MyTopic.c_str(), myfloat_in_Char);
                MyTopic = SETTINGS.hostname + String("/") + SETTINGS.DHT_ident[n] + String("/humidity");
                dtostrf(DHT_All_Sensor_Values_Hum[n], 6, 2, myfloat_in_Char); 
                mqtt_client.publish(MyTopic.c_str(), myfloat_in_Char);
                MyTopic = SETTINGS.hostname + String("/") + SETTINGS.DHT_ident[n] + String("/heatindex");
                dtostrf(DHT_All_Sensor_Values_Heat[n], 6, 2, myfloat_in_Char); 
                mqtt_client.publish(MyTopic.c_str(), myfloat_in_Char);
                MyTopic = SETTINGS.hostname + String("/") + SETTINGS.DHT_ident[n] + String("/dewpoint");
                dtostrf(DHT_All_Sensor_Values_Dew[n], 6, 2, myfloat_in_Char); 
                mqtt_client.publish(MyTopic.c_str(), myfloat_in_Char);
              }
            }
          }
          for (int n = 0; n < SETTINGS.BME280_num_configured; n++) {
            if (SETTINGS.BME280_pin_sda[n] != Not_used_pin_number) { // ce je pin=100 potem je disabled
              if (!isnan(BME280_All_Sensor_Values_Temp[n])) {
                dtostrf(BME280_All_Sensor_Values_Temp[n], 6, 2, myfloat_in_Char);
                MyTopic = SETTINGS.hostname + String("/") + SETTINGS.BME280_ident[n] + String("/temperature");
                mqtt_client.publish(MyTopic.c_str(), myfloat_in_Char);
                dtostrf(BME280_All_Sensor_Values_Hum[n], 6, 2, myfloat_in_Char);
                MyTopic = SETTINGS.hostname + String("/") + SETTINGS.BME280_ident[n] + String("/humidity"); 
                mqtt_client.publish(MyTopic.c_str(), myfloat_in_Char);
                dtostrf(BME280_All_Sensor_Values_Press[n], 6, 2, myfloat_in_Char);
                MyTopic = SETTINGS.hostname + String("/") + SETTINGS.BME280_ident[n] + String("/pressure");
                mqtt_client.publish(MyTopic.c_str(), myfloat_in_Char);
                dtostrf(BME280_All_Sensor_Values_Heat[n], 6, 2, myfloat_in_Char);
                MyTopic = SETTINGS.hostname + String("/") + SETTINGS.BME280_ident[n] + String("/heatindex");
                mqtt_client.publish(MyTopic.c_str(), myfloat_in_Char);
                dtostrf(BME280_All_Sensor_Values_Dew[n], 6, 2, myfloat_in_Char);
                MyTopic = SETTINGS.hostname + String("/") + SETTINGS.BME280_ident[n] + String("/dewpoint");
                mqtt_client.publish(MyTopic.c_str(), myfloat_in_Char);
              }
            }
          }
          for (int n = 0; n < SETTINGS.MAX6675_num_configured; n++) {
            if (SETTINGS.MAX6675_pin_clk[n] != Not_used_pin_number) { // ce je pin=100 potem je disabled
              if (!isnan(MAX6675_All_Sensor_Values_Temp[n])) {
                dtostrf(MAX6675_All_Sensor_Values_Temp[n], 6, 2, myfloat_in_Char);
                MyTopic = SETTINGS.hostname + String("/") + SETTINGS.MAX6675_ident[n] + String("/temperature");
                mqtt_client.publish(MyTopic.c_str(), myfloat_in_Char);
              }
            }
          }
          for (int n = 0; n < SETTINGS.Ultrasonic_num_configured; n++) {
            if (SETTINGS.Ultrasonic_pin_trigger[n] != Not_used_pin_number) {
              dtostrf(Ultrasonic_All_Sensor_Values_Distance[n], 6, 2, myfloat_in_Char);
              MyTopic = SETTINGS.hostname + String("/") + SETTINGS.Ultrasonic_ident[n] + String("/distance");
              mqtt_client.publish(MyTopic.c_str(), myfloat_in_Char);
            }
          }
        }
      }
    }
    
    String http_data_collector_config(void) {
      String html = "";
      for(int n = 0; n < SETTINGS.data_collector_num_configured; n++) {
        if (SETTINGS.data_collector_type[n] == 1) {
          html += "<tr><td>" + String(n) + "</td><td>influxdb_tcp</td><td>" + String(SETTINGS.data_collector_host[n]) + "</td><td>" + String(SETTINGS.data_collector_port[n]) + "</td><td>" + String(SETTINGS.data_collector_push_interval[n]) + "</td><td>" + String(SETTINGS.data_collector_db_name[n]) + "</td><td>" + String(SETTINGS.data_collector_db_username[n]) + "</td><td>" + String(SETTINGS.data_collector_db_password[n]) + "</td></tr>"; 
        }
        if (SETTINGS.data_collector_type[n] == 2) {
          html += "<tr><td>" + String(n) + "</td><td>carbon_udp</td><td>" + String(SETTINGS.data_collector_host[n]) + "</td><td>" + String(SETTINGS.data_collector_port[n]) + "</td><td>" + String(SETTINGS.data_collector_push_interval[n]) + "</td><td>" + String(SETTINGS.data_collector_db_name[n]) + "</td><td>" + String(SETTINGS.data_collector_db_username[n]) + "</td><td>" + String(SETTINGS.data_collector_db_password[n]) + "</td></tr>"; 
        }
        if (SETTINGS.data_collector_type[n] == 3) {
          html += "<tr><td>" + String(n) + "</td><td>redis_tcp</td><td>" + String(SETTINGS.data_collector_host[n]) + "</td><td>" + String(SETTINGS.data_collector_port[n]) + "</td><td>" + String(SETTINGS.data_collector_push_interval[n]) + "</td><td>" + String(SETTINGS.data_collector_db_name[n]) + "</td><td>" + String(SETTINGS.data_collector_db_username[n]) + "</td><td>" + String(SETTINGS.data_collector_db_password[n]) + "</td></tr>"; 
        }
        if (SETTINGS.data_collector_type[n] == 4) {
          html += "<tr><td>" + String(n) + "</td><td>influxdb_udp</td><td>" + String(SETTINGS.data_collector_host[n]) + "</td><td>" + String(SETTINGS.data_collector_port[n]) + "</td><td>" + String(SETTINGS.data_collector_push_interval[n]) + "</td><td>" + String(SETTINGS.data_collector_db_name[n]) + "</td><td>" + String(SETTINGS.data_collector_db_username[n]) + "</td><td>" + String(SETTINGS.data_collector_db_password[n]) + "</td></tr>"; 
        }
      }    
      return html;
    }

    void http_data_collector_remove_client() {
      if (SETTINGS.data_collector_num_configured > 0) {
        SETTINGS.data_collector_num_configured--; 
      }
      String html = BackURLSystemSettings;
      server.send(200, "text/html", html);
    }

    void data_collector_send_data() {
      for(int x = 0; x < SETTINGS.data_collector_num_configured; x++) {
        if (millis() - previousDataCollector[x] >= SETTINGS.data_collector_push_interval[x] * 1000) {
          previousDataCollector[x] = millis();
          String InfluxDataStructure = "";
          String CarbonDataStructure = "";
          String RedisDataStructure = "";
          InfluxDataStructure += "voltage,host=" + String(SETTINGS.hostname) + ",name=system value=" + String((float)ESP.getVcc()/890) + "\n";
          InfluxDataStructure += "uptime,host=" + String(SETTINGS.hostname) + ",name=system value=" + String((unsigned long)millis()) + "\n";
          InfluxDataStructure += "wifi,host=" + String(SETTINGS.hostname) + ",name=system value=" + String(WiFi.RSSI()) + "\n";
          InfluxDataStructure += "freeheap,host=" + String(SETTINGS.hostname) + ",name=system value=" + String(ESP.getFreeHeap()) + "\n";
          CarbonDataStructure += String(SETTINGS.hostname) + ".system.voltage " + String((float)ESP.getVcc()/890) + " " + now() + "\n";
          CarbonDataStructure += String(SETTINGS.hostname) + ".system.uptime " + String((unsigned long)millis()) + " " + now() + "\n";
          CarbonDataStructure += String(SETTINGS.hostname) + ".system.wifi " + String(WiFi.RSSI()) + " " + now() + "\n";
          CarbonDataStructure += String(SETTINGS.hostname) + ".system.freeheap " + String(ESP.getFreeHeap()) + " " + now() + "\n";
          String RedisTmpString = ""; 
          RedisDataStructure += String("*3\r\n") + "$5\r\n" + "LPUSH\r\n";
          RedisTmpString += "{\"system\": {\"voltage\": \"" + String((float)ESP.getVcc()/890)+ "\", ";
          RedisTmpString += "\"uptime\": \"" + String((unsigned long)millis()) + "\", ";
          RedisTmpString += "\"wifi\": \"" + String(WiFi.RSSI()) + "\", ";
          RedisTmpString += "\"freeheap\": \"" + String(ESP.getFreeHeap()) + "\"}}";
          RedisDataStructure += "$" + String(strlen(SETTINGS.hostname)) + "\r\n" + String(SETTINGS.hostname) + "\r\n" + "$" + String(RedisTmpString.length()) + "\r\n" + RedisTmpString + "\r\n";
          if ( SETTINGS.data_collector_type[x] == 1 ) {
            send_to_influxdb_tcp(InfluxDataStructure, x);
          }
          else if ( SETTINGS.data_collector_type[x] == 4 ) {
            send_to_influxdb_udp(InfluxDataStructure, x);
          }
          else if ( SETTINGS.data_collector_type[x] == 2 ) {
            send_to_carbon(CarbonDataStructure, x);
          }
          else if (SETTINGS.data_collector_type[x] == 3) {
            send_to_redis(RedisDataStructure, x);
          }
          InfluxDataStructure = "";
          CarbonDataStructure = "";
          RedisDataStructure = "";
          for (int n = 0; n < SETTINGS.IO_num_configured; n++) {
            if (SETTINGS.IO_pin[n] != Not_used_pin_number) {
              InfluxDataStructure += "state,host=" + String(SETTINGS.hostname) + ",name=" + String(SETTINGS.IO_ident[n]) + " value=" + String(digitalRead(SETTINGS.IO_pin[n])) + "\n";
              CarbonDataStructure += String(SETTINGS.hostname) + "." + String(SETTINGS.IO_ident[n]) + ".state " + String(digitalRead(SETTINGS.IO_pin[n])) + " " + now() + "\n";
              if ( SETTINGS.data_collector_type[x] == 3 ) {
                redis_data_prepair(String (SETTINGS.IO_ident[n]), "state", String(digitalRead(SETTINGS.IO_pin[n])), x);
              }
            }
          }
          if (SETTINGS.IO_num_configured  > 0) {
            if ( SETTINGS.data_collector_type[x] == 1 ) {
              send_to_influxdb_tcp(InfluxDataStructure, x);
            }
            else if ( SETTINGS.data_collector_type[x] == 4 ) {
              send_to_influxdb_udp(InfluxDataStructure, x);
            }
            else if ( SETTINGS.data_collector_type[x] == 2 ) {
              send_to_carbon(CarbonDataStructure, x);
            }
            InfluxDataStructure = "";
            CarbonDataStructure = "";
          }
          if (SETTINGS.DS18B20_pin != Not_used_pin_number) {
            for (int n = 0; n < SETTINGS.DS18B20_num_configured; n++) {
              if (!isnan(DS18B20_All_Sensor_Values[n])) {
                InfluxDataStructure += "temperature,host=" + String(SETTINGS.hostname) + ",name=" + String(SETTINGS.DS18B20_ident[n]) + " value=" + String(DS18B20_All_Sensor_Values[n]) + "\n";
                CarbonDataStructure += String(SETTINGS.hostname) + "." + String(SETTINGS.DS18B20_ident[n]) + ".temperature " + String(DS18B20_All_Sensor_Values[n]) + " " + now() + "\n";
                if ( SETTINGS.data_collector_type[x] == 3 ) {
                  redis_data_prepair(String (SETTINGS.DS18B20_ident[n]), "temperature", String(DS18B20_All_Sensor_Values[n]), x);
                  yield();
                }
              }
            }
          }
          if (SETTINGS.DS18B20_num_configured  > 0) {
            if ( SETTINGS.data_collector_type[x] == 1 ) {
              send_to_influxdb_tcp(InfluxDataStructure, x);
            }
            else if ( SETTINGS.data_collector_type[x] == 4 ) {
              send_to_influxdb_udp(InfluxDataStructure, x);
            }
            else if ( SETTINGS.data_collector_type[x] == 2 ) {
              send_to_carbon(CarbonDataStructure, x);
            }
            InfluxDataStructure = "";
            CarbonDataStructure = "";
          }
          for (int n = 0; n < SETTINGS.DHT_num_configured; n++) {
            if (SETTINGS.DHT_pin[n] != Not_used_pin_number) {
              if (!isnan(DHT_All_Sensor_Values_Temp[n]) && !isnan(DHT_All_Sensor_Values_Hum[n])) {
                InfluxDataStructure += "temperature,host=" + String(SETTINGS.hostname) + ",name=" + String(SETTINGS.DHT_ident[n]) + " value=" + String(DHT_All_Sensor_Values_Temp[n]) + "\n";
                InfluxDataStructure += "humidity,host=" + String(SETTINGS.hostname) + ",name=" + String(SETTINGS.DHT_ident[n]) + " value=" + String(DHT_All_Sensor_Values_Hum[n]) + "\n";
                InfluxDataStructure += "heatindex,host=" + String(SETTINGS.hostname) + ",name=" + String(SETTINGS.DHT_ident[n]) + " value=" + String(DHT_All_Sensor_Values_Heat[n]) + "\n";
                InfluxDataStructure += "dewpoint,host=" + String(SETTINGS.hostname) + ",name=" + String(SETTINGS.DHT_ident[n]) + " value=" + String(DHT_All_Sensor_Values_Dew[n]) + "\n";
                CarbonDataStructure += String(SETTINGS.hostname) + "." + String(SETTINGS.DHT_ident[n]) + ".temperature " + String(DHT_All_Sensor_Values_Temp[n]) + " " + now() + "\n";
                CarbonDataStructure += String(SETTINGS.hostname) + "." + String(SETTINGS.DHT_ident[n]) + ".pressure " + String(DHT_All_Sensor_Values_Hum[n]) + " " + now() + "\n";
                CarbonDataStructure += String(SETTINGS.hostname) + "." + String(SETTINGS.DHT_ident[n]) + ".heat " + String(DHT_All_Sensor_Values_Heat[n]) + " " + now() + "\n";
                CarbonDataStructure += String(SETTINGS.hostname) + "." + String(SETTINGS.DHT_ident[n]) + ".dew " + String(DHT_All_Sensor_Values_Dew[n]) + " " + now() + "\n";
                if ( SETTINGS.data_collector_type[x] == 3 ) {
                  String RedisTmpString = ""; 
                  RedisDataStructure = String("*3\r\n") + "$5\r\n" + "LPUSH\r\n";
                  RedisTmpString += "{\"" + String(SETTINGS.DHT_ident[n]) + "\": {\"temperature\": \"" + String(DHT_All_Sensor_Values_Temp[n]) + "\", ";
                  RedisTmpString += "\"humidity\": \"" + String(DHT_All_Sensor_Values_Hum[n]) + "\", ";
                  RedisTmpString += "\"heatindex\": \"" + String(DHT_All_Sensor_Values_Heat[n]) + "\", ";
                  RedisTmpString += "\"dewpoint\": \"" + String(DHT_All_Sensor_Values_Dew[n]) + "\"}}";
                  RedisDataStructure += "$" + String(strlen(SETTINGS.hostname)) + "\r\n" + String(SETTINGS.hostname) + "\r\n" + "$" + String(RedisTmpString.length()) + "\r\n" + RedisTmpString + "\r\n";
                  send_to_redis(RedisDataStructure, x);
                  yield();
                }
              }   
            }
          }
          if (SETTINGS.DHT_num_configured  > 0) {
            if ( SETTINGS.data_collector_type[x] == 1 ) {
              send_to_influxdb_tcp(InfluxDataStructure, x);
            }
            else if ( SETTINGS.data_collector_type[x] == 4 ) {
              send_to_influxdb_udp(InfluxDataStructure, x);
            }
            else if ( SETTINGS.data_collector_type[x] == 2 ) {
              send_to_carbon(CarbonDataStructure, x);
            }
            InfluxDataStructure = "";
            CarbonDataStructure = "";
          }
          for (int n = 0; n < SETTINGS.BME280_num_configured; n++) {
            if (SETTINGS.BME280_pin_sda[n] != Not_used_pin_number) {
              if (!isnan(BME280_All_Sensor_Values_Temp[n])) {
                InfluxDataStructure += "temperature,host=" + String(SETTINGS.hostname) + ",name=" + String(SETTINGS.BME280_ident[n]) + " value=" + String(BME280_All_Sensor_Values_Temp[n]) + "\n";
                InfluxDataStructure += "humidity,host=" + String(SETTINGS.hostname) + ",name=" + String(SETTINGS.BME280_ident[n]) + " value=" + String(BME280_All_Sensor_Values_Hum[n]) + "\n";
                InfluxDataStructure += "pressure,host=" + String(SETTINGS.hostname) + ",name=" + String(SETTINGS.BME280_ident[n]) + " value=" + String(BME280_All_Sensor_Values_Press[n]) + "\n";
                InfluxDataStructure += "heatindex,host=" + String(SETTINGS.hostname) + ",name=" + String(SETTINGS.BME280_ident[n]) + " value=" + String(BME280_All_Sensor_Values_Heat[n]) + "\n";
                InfluxDataStructure += "dewpoint,host=" + String(SETTINGS.hostname) + ",name=" + String(SETTINGS.BME280_ident[n]) + " value=" + String(BME280_All_Sensor_Values_Dew[n]) + "\n";     
                CarbonDataStructure += String(SETTINGS.hostname) + "." + String(SETTINGS.BME280_ident[n]) + ".temperature " + String(BME280_All_Sensor_Values_Temp[n]) + " " + now() + "\n";
                CarbonDataStructure += String(SETTINGS.hostname) + "." + String(SETTINGS.BME280_ident[n]) + ".humidity " + String(BME280_All_Sensor_Values_Hum[n]) + " " + now() + "\n";
                CarbonDataStructure += String(SETTINGS.hostname) + "." + String(SETTINGS.BME280_ident[n]) + ".pressure " + String(BME280_All_Sensor_Values_Press[n]) + " " + now() + "\n";
                CarbonDataStructure += String(SETTINGS.hostname) + "." + String(SETTINGS.BME280_ident[n]) + ".heatindex " + String(BME280_All_Sensor_Values_Heat[n]) + " " + now() + "\n";
                CarbonDataStructure += String(SETTINGS.hostname) + "." + String(SETTINGS.BME280_ident[n]) + ".dewpoint " + String(BME280_All_Sensor_Values_Dew[n]) + " " + now() + "\n";
                if ( SETTINGS.data_collector_type[x] == 3 ) {
                  String RedisTmpString = ""; 
                  RedisDataStructure = String("*3\r\n") + "$5\r\n" + "LPUSH\r\n";
                  RedisTmpString += "{\"" + String(SETTINGS.BME280_ident[n]) + "\": {\"temperature\": \"" + String(BME280_All_Sensor_Values_Temp[n]) + "\", ";
                  RedisTmpString += "\"humidity\": \"" + String(BME280_All_Sensor_Values_Hum[n]) + "\", ";
                  RedisTmpString += "\"heatindex\": \"" + String(BME280_All_Sensor_Values_Heat[n]) + "\", ";
                  RedisTmpString += "\"dewpoint\": \"" + String(BME280_All_Sensor_Values_Dew[n]) + "\"}}";
                  RedisDataStructure += "$" + String(strlen(SETTINGS.hostname)) + "\r\n" + String(SETTINGS.hostname) + "\r\n" + "$" + String(RedisTmpString.length()) + "\r\n" + RedisTmpString + "\r\n";
                  send_to_redis(RedisDataStructure, x);
                  yield();
                }
              }
            }
          }
          if (SETTINGS.BME280_num_configured  > 0) {
            if ( SETTINGS.data_collector_type[x] == 1 ) {
              send_to_influxdb_tcp(InfluxDataStructure, x);
            }
            else if ( SETTINGS.data_collector_type[x] == 4 ) {
              send_to_influxdb_udp(InfluxDataStructure, x);
            }
            else if ( SETTINGS.data_collector_type[x] == 2 ) {
              send_to_carbon(CarbonDataStructure, x);
            }
            InfluxDataStructure = "";
            CarbonDataStructure = "";
          }
          for (int n = 0; n < SETTINGS.MAX6675_num_configured; n++) {
            if (SETTINGS.MAX6675_pin_clk[n] != Not_used_pin_number) {
              if (!isnan(MAX6675_All_Sensor_Values_Temp[n])) {
                InfluxDataStructure += "temperature,host=" + String(SETTINGS.hostname) + ",name=" + String(SETTINGS.MAX6675_ident[n]) + " value=" + String(MAX6675_All_Sensor_Values_Temp[n]) + "\n";
                CarbonDataStructure += String(SETTINGS.hostname) + "." + String(SETTINGS.MAX6675_ident[n]) + ".temperature " + String(MAX6675_All_Sensor_Values_Temp[n]) + " " + now() + "\n";
                if ( SETTINGS.data_collector_type[x] == 3 ) {
                  redis_data_prepair(String (SETTINGS.MAX6675_ident[n]), "temperature", String(MAX6675_All_Sensor_Values_Temp[n]), x);
                  yield();
                }
              }
            }
          }
          if (SETTINGS.MAX6675_num_configured  > 0) {
            if ( SETTINGS.data_collector_type[x] == 1 ) {
              send_to_influxdb_tcp(InfluxDataStructure, x);
            }
            else if ( SETTINGS.data_collector_type[x] == 4 ) {
              send_to_influxdb_udp(InfluxDataStructure, x);
            }
            else if ( SETTINGS.data_collector_type[x] == 2 ) {
              send_to_carbon(CarbonDataStructure, x);
            }
            InfluxDataStructure = "";
            CarbonDataStructure = "";
          }
          for (int n = 0; n < SETTINGS.Ultrasonic_num_configured; n++) {
            if (SETTINGS.Ultrasonic_pin_trigger[n] != Not_used_pin_number) {
              InfluxDataStructure += "distance,host=" + String(SETTINGS.hostname) + ",name=" + String(SETTINGS.Ultrasonic_ident[n]) + " value=" + String(Ultrasonic_All_Sensor_Values_Distance[n]) + "\n";
              CarbonDataStructure += String(SETTINGS.hostname) + "." + String(SETTINGS.Ultrasonic_ident[n]) + ".distance " + String(Ultrasonic_All_Sensor_Values_Distance[n]) + " " + now() + "\n";
              if ( SETTINGS.data_collector_type[x] == 3 ) {
                redis_data_prepair(String (SETTINGS.Ultrasonic_ident[n]), "distance", String(Ultrasonic_All_Sensor_Values_Distance[n]), x);
                yield();
              }
            }
          }
          if (SETTINGS.Ultrasonic_num_configured > 0) {
            if ( SETTINGS.data_collector_type[x] == 1 ) {
              send_to_influxdb_tcp(InfluxDataStructure, x);
            }
            else if ( SETTINGS.data_collector_type[x] == 4 ) {
              send_to_influxdb_udp(InfluxDataStructure, x);
            }
            else if ( SETTINGS.data_collector_type[x] == 2 ) {
              send_to_carbon(CarbonDataStructure, x);
            }
            InfluxDataStructure = "";
            CarbonDataStructure = "";
          }
        }
      }
    }
    
    void redis_data_prepair(String MetricIdent, String MetricType, String MetricValue, int x) {
      String RedisTmpString = ""; 
      String RedisDataStructure = "";
      RedisDataStructure = String("*3\r\n") + "$5\r\n" + "LPUSH\r\n";
      RedisTmpString += "{\"" + MetricIdent + "\": {\"" + MetricType + "\": \"" + MetricValue + "\"}}";
      RedisDataStructure += "$" + String(strlen(SETTINGS.hostname)) + "\r\n" + String(SETTINGS.hostname) + "\r\n" + "$" + String(RedisTmpString.length()) + "\r\n" + RedisTmpString + "\r\n";
      send_to_redis(RedisDataStructure, x);
    }
    
    void send_to_redis(String content, int x) {
      WiFiClient redis_client;
      redis_client.setTimeout(50);
      if (!redis_client.connect(SETTINGS.data_collector_host[x], SETTINGS.data_collector_port[x])) {
        SendLog(" ERROR: redis (" + String(SETTINGS.data_collector_host[x]) + ":" + String(SETTINGS.data_collector_port[x]) + ") connection failed.");
        return;
      }
      yield();
      redis_client.print(content);
      redis_client.stop();
    }

    void send_to_influxdb_udp (String msgtosend, int x) {
        unsigned int msg_length = msgtosend.length();
        byte* p = (byte*)malloc(msg_length);
        memcpy(p, (char*) msgtosend.c_str(), msg_length);
        udp.beginPacket(SETTINGS.data_collector_host[x], SETTINGS.data_collector_port[x]);
        udp.write(p, msg_length);
        udp.endPacket();
        free(p);
        yield();
    }

    void send_to_influxdb_tcp (String content, int x) {
      WiFiClient influxdb_client;
      influxdb_client.setTimeout(50);
      if (!influxdb_client.connect(SETTINGS.data_collector_host[x], SETTINGS.data_collector_port[x])) {
        SendLog(" ERROR: influxdb (" + String(SETTINGS.data_collector_host[x]) + ":" + String(SETTINGS.data_collector_port[x]) + ") connection failed.");
        return;
      }
      String Payload = "";
      if (SETTINGS.data_collector_db_username[x][0] != '\0' && SETTINGS.data_collector_db_password[x][0] != '\0' ) {
        Payload += "POST /write?db=" + String(SETTINGS.data_collector_db_name[x]) + "&u=" + String(SETTINGS.data_collector_db_username[x]) + "&p=" + String(SETTINGS.data_collector_db_password[x]) + " HTTP/1.1\r\n";
      }
      else {
        Payload += "POST /write?db=" + String(SETTINGS.data_collector_db_name[x]) + " HTTP/1.1\r\n";
      }
      Payload += "User-Agent: " + String(SETTINGS.hostname) + "/0.1\r\n";
      Payload += "Host: localhost:" + String(SETTINGS.data_collector_port[x]) + "\r\n";
      Payload += "Accept: */*\r\n";
      Payload += "Content-Length: " + String(content.length()) + "\r\n";
      Payload += "Content-Type: application/x-www-form-urlencoded\r\n";
      Payload += "\r\n";
      Payload +=  content + "\r\n";
      influxdb_client.print(Payload);
      influxdb_client.stop();
    }

    void send_to_carbon (String msgtosend, int x) {
      if (WiFi.status() == WL_CONNECTED) {
        unsigned int msg_length = msgtosend.length();
        byte* p = (byte*)malloc(msg_length);
        memcpy(p, (char*) msgtosend.c_str(), msg_length);
        udp.beginPacket(SETTINGS.data_collector_host[x], SETTINGS.data_collector_port[x]);
        //udp.write(SETTINGS.hostname);
        udp.write(p, msg_length);
        udp.endPacket();
        free(p);
        yield();
      }
    }

    void http_crash_info() {
      String text;
      text += "ResetInfo: " + String(ESP.getResetInfo()) + "<br>";
      text += "BootMode: " + String(ESP.getBootMode()) + "<br>";
      text += "BootVersion: " + String(ESP.getBootVersion()) + "<br>";
      text += "SdkVersion: " + String(ESP.getSdkVersion()) + "<br>";
      char tmp[8]; 
      // Note that 'EEPROM.begin' method is reserving a RAM buffer
      // The buffer size is SAVE_CRASH_EEPROM_OFFSET + SAVE_CRASH_SPACE_SIZE
      EEPROM.begin(SAVE_CRASH_EEPROM_OFFSET + SAVE_CRASH_SPACE_SIZE);
    
      byte crashCounter = EEPROM.read(SAVE_CRASH_EEPROM_OFFSET + SAVE_CRASH_COUNTER);
      if (crashCounter == 0)
      {
        text += "No any crashes saved";
        server.send(200, "text/html", text);
        yield();
        return;
      }
    
      text += "Crash information recovered from EEPROM<br>";
      int16_t readFrom = SAVE_CRASH_EEPROM_OFFSET + SAVE_CRASH_DATA_SETS;
      for (byte k = 0; k < crashCounter; k++)
      {
        uint32_t crashTime;
        EEPROM.get(readFrom + SAVE_CRASH_CRASH_TIME, crashTime);
        text += "Crash #" + String(k + 1) + " at " + String(crashTime) + "ms<br>";
    
        text += "Reason of restart: " + String(EEPROM.read(readFrom + SAVE_CRASH_RESTART_REASON)) + "<br>";
        text += "Exception cause: " + String(EEPROM.read(readFrom + SAVE_CRASH_EXCEPTION_CAUSE)) + "<br>";
    
        uint32_t epc1, epc2, epc3, excvaddr, depc;
        EEPROM.get(readFrom + SAVE_CRASH_EPC1, epc1);
        EEPROM.get(readFrom + SAVE_CRASH_EPC2, epc2);
        EEPROM.get(readFrom + SAVE_CRASH_EPC3, epc3);
        EEPROM.get(readFrom + SAVE_CRASH_EXCVADDR, excvaddr);
        EEPROM.get(readFrom + SAVE_CRASH_DEPC, depc);
        sprintf(tmp, "epc1=0x%08x ", epc1);
        text += tmp;
        sprintf(tmp, "epc2=0x%08x ", epc2);
        text += tmp;
        sprintf(tmp, "epc3=0x%08x ", epc3);
        text += tmp;
        sprintf(tmp, "excvaddr=0x%08x ", excvaddr);
        text += tmp;
        sprintf(tmp, "depc=0x%08x ", depc);
        text += tmp;       
        text += "<br>";
        uint32_t stackStart, stackEnd;
        EEPROM.get(readFrom + SAVE_CRASH_STACK_START, stackStart);
        EEPROM.get(readFrom + SAVE_CRASH_STACK_END, stackEnd);
        text += ">>>stack>>><br>";
        int16_t currentAddress = readFrom + SAVE_CRASH_STACK_TRACE;
        int16_t stackLength = stackEnd - stackStart;
        uint32_t stackTrace;
        for (int16_t i = 0; i < stackLength; i += 0x10)
        {
          sprintf(tmp, "%08x ", stackStart + i);
          text += tmp;
          for (byte j = 0; j < 4; j++)
          {
            EEPROM.get(currentAddress, stackTrace);
            sprintf(tmp, "%08x ", stackTrace);
            text += tmp;
            currentAddress += 4;
            if (currentAddress - SAVE_CRASH_EEPROM_OFFSET > SAVE_CRASH_SPACE_SIZE)
            {
              text += "<br>Incomplete stack trace saved!";
              goto eepromSpaceEnd;
            }
          }
          text += "<br>";
        }
        eepromSpaceEnd:
        text += "&lt&lt&ltstack<<<";
        readFrom = readFrom + SAVE_CRASH_STACK_TRACE + stackLength;
      }
      int16_t writeFrom;
      EEPROM.get(SAVE_CRASH_EEPROM_OFFSET + SAVE_CRASH_WRITE_FROM, writeFrom);
      EEPROM.end();
    
      // is there free EEPROM space avialable to save data for next crash?
      if (writeFrom + SAVE_CRASH_STACK_TRACE > SAVE_CRASH_SPACE_SIZE)
      {
        text += "<br>No more EEPROM space available to save crash information!<br>";
      }
      else
      {
        sprintf(tmp, "<br>EEPROM space available: 0x%04x bytes<br>", SAVE_CRASH_SPACE_SIZE - writeFrom);
        text += tmp;
      }
      server.send(200, "text/html", text);
      yield();
    }
      
    void http_clear_crash_info() {
      EEPROM.begin(SAVE_CRASH_EEPROM_OFFSET + SAVE_CRASH_SPACE_SIZE);
      // clear the crash counter
      EEPROM.write(SAVE_CRASH_EEPROM_OFFSET + SAVE_CRASH_COUNTER, 0);
      EEPROM.end();
      SendLog(" INFO: Crash info cleared.");
      String html = BackURLSystemSettings;
      server.send(200, "text/html", html);
    }

    
