    /***************************************
    * Aplikasi Pengaturan Kelembapan Pada Tanaman Seledri Menggunakan Metode Prototype Berbasis Iot
    * Board  : NodeMCU ESP8266  V3
    * Input  : Sensor Soil Moisture (Sensor kelembapan Tanah), DS18B20 (Sensor Temperatur)
    * Output : Pompa Mini 5V, Relay, Blynk Android & LCD/Display 16x2
    ****************************************/


    #define BLYNK_TEMPLATE_ID "TMPL6nT7plWPi"                     //GANTI SESUAI DENGAN TEMPLATE ID
    #define BLYNK_TEMPLATE_NAME "seledri"                         //SESUAI NAMA TEMPLATE BLYNK
    #define BLYNK_AUTH_TOKEN "mzImLBz_WayyluOi6H14_yp2uboKnCdB"   //TOKEN SESUAI YANG DI DAPAT PADA SAAT PEMBUATAN TEMPLATE BLYNK 


    #include <DallasTemperature.h>
    #include <OneWire.h>
    #include <DHT.h>
    #include <LiquidCrystal_I2C.h>
    #include <Blynk.h>
    #include <ESP8266WiFi.h>
    #include <BlynkSimpleEsp8266.h>
    #define BLYNK_PRINT Serial
    #define ONE_WIRE_BUS D5  
    #define DHTPIN D3 
    #define DHTTYPE DHT11
    #define pump D6
    DHT dht(DHTPIN, DHTTYPE);
    OneWire oneWire(ONE_WIRE_BUS);
    DallasTemperature sensors(&oneWire); 
    LiquidCrystal_I2C lcd(0x27, 16, 2);  
    const int AirValue = 620;   
    const int WaterValue = 310;  
    int soilMoistureValue = 0;
    int soilmoist=0;  
    int humi, temp,fp,sistem;
    int buttonState;


    //-- GANTI SESUAI DENGAN NILAI KELEMBAPAN TANAMAN SELEDRI

    int SP_LOW=75;    //NILAI TERENDAH KELEMBAPAN TANAH
    int SP_HIGH=90;   //NILAI TERTINGGI KELEMBAPAN TANAH


    //---GANTI SESUAI DENGAN TOKEN BLYNK


    char auth[] = BLYNK_AUTH_TOKEN;
    
    
    //---GANTI SESUAI DENGAN JARINGAN WIFI
    //---GANTI SESUAI DENGAN NAMA HOTSPOT 

    char ssid[] = "Cellozz";   // Nama Hotspot/WiFi
    char pass[] = "qwerty28";  //  Password Hotspot/WiFi
    //==========================================

    //TOMBOL POMPA AUTOMATIC BLYNK

    BLYNK_WRITE(V4){
      buttonState = param.asInt();
      if(buttonState==HIGH){
        sistem=1;                          //automatic
        fp=0;
        delay(10);
      }
      else if(buttonState==LOW){
        sistem=0;                          //manual
        delay(10);    
      }
    }
    //==========================================

    //TOMBOL POMPA MANUAL BLYNK

    BLYNK_WRITE(V5){
      buttonState = param.asInt();
      if(sistem==0){
        if(buttonState==LOW){
          digitalWrite(pump,HIGH);
          lcd.setCursor(11,1);
          lcd.print("OFF"); 
          delay(10);
          }
        else if(buttonState==HIGH){
          digitalWrite(pump,LOW);
          lcd.setCursor(12,1);
          lcd.print("ON"); 
          delay(10);
          }
      }
    }
    //=====================================


    void setup(void)
    {
      lcd.begin(16,2);
      lcd.init();
      lcd.backlight();
      lcd.clear();
      lcd.print("Exel Sambow -_-");
      lcd.setCursor(0, 1);
      lcd.print("NodeMCU  ESP8266");
      Serial.begin(9600); 
      sensors.begin();
      dht.begin();   
      pinMode(pump,OUTPUT);
      digitalWrite(pump,LOW);
      delay(1200);
      digitalWrite(pump,HIGH);
      delay(1200);
      Blynk.begin(auth, ssid, pass);   
      delay(1500);
      lcd.clear();
      lcd.print("Mst=    %T=    C");
      lcd.setCursor(0,1);
      lcd.print("POM= ");

      //lcd.print("Hum=    % P= OFF");
    }
    //=====================================


    void loop(void)
    { 
      Blynk.run(); 
      sensors.requestTemperatures();  
      temp=sensors.getTempCByIndex(0);               
      Serial.print("Temp :");
      Serial.println(temp);
      lcd.setCursor(12,0);
      lcd.print(temp);  
      soilMoistureValue = analogRead(A0);  //put Sensor insert into soil
      soilmoist= map(soilMoistureValue, AirValue, WaterValue, 0, 100);
      if(soilmoist >= 100)
        {
          soilmoist=100;
        }
      else if(soilmoist <=0)
        {
          soilmoist=0;
        }
      Serial.print("Soil Moisture :");
      Serial.print(soilmoist);
      Serial.println("%");
      lcd.setCursor(4,0);
      lcd.print(soilmoist); 
      lcd.print(" "); 
      humi =dht.readHumidity();
      
      if (isnan(humi) ) {
        Serial.println("DHT11 tidak terbaca... !");
        return;
      }
      else{  
      Serial.print("Humi:");
      Serial.println(humi);  
      Serial.println("%"); 
      lcd.setCursor(4,1);
      lcd.print(" ");//TELAH DIGANTI humi
      }


      //-----control penyiraman
      
      if(sistem==1){
      if((soilmoist<SP_LOW)&&(fp==0)){
        digitalWrite(pump,LOW);
        lcd.setCursor(12,1);
        lcd.print("ON");
        fp=1;
      }
      else if((soilmoist>SP_HIGH)&&(fp==1)){
        digitalWrite(pump,HIGH);
        lcd.setCursor(11,1);
        lcd.print("OFF");
        fp=0;
      }
      }
      Blynk.virtualWrite(V1, temp);
      Blynk.virtualWrite(V2,soilmoist);
      Blynk.virtualWrite(V3,humi);

      Serial.print("Sistem=");
      Serial.println(sistem);
      Serial.print("fp=");
      Serial.println(fp);
      
      delay(1500);
    }
