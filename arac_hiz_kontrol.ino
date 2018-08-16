//pin tanımlamaları
#define pedal_olcum A0
#define sanal_pedal 11
#define artir 2
#define azalt 3
#define hiz_sabit 4

//serial ayarlar
boolean ok=false;
String real_data,data;
unsigned long dati;
unsigned int pwm;//olculen pwm
unsigned int pwm_real; //sanal gonderilen gercek pwm

//zaman ayarlari
unsigned long gecmis_zaman;
unsigned sure =5; //akim için pwm hassasiyeti ters yönlü

//buton ayarları
boolean  art=LOW;
boolean  azal=LOW;
boolean  sabit=LOW;

//aktif-pasif mod kontrol
boolean  sabit_durumu=LOW;

//hiz modlari
int sabitlenicek_hiz =50;
unsigned long gecmis_zaman_hiz;
unsigned int sure_hiz =3;

void setup() {
    Serial.begin(9600);
    pinMode(pedal_olcum, INPUT);
    pinMode(sanal_pedal, OUTPUT);
    pinMode(artir, INPUT);
    pinMode(azalt, INPUT);
    pinMode(hiz_sabit, INPUT);

}

void loop() {
  //simdiki zaman
  unsigned long zaman = millis();
  //pedal olcum
  pwm = map(analogRead(pedal_olcum), 0, 1023, 0, 255);
  //akim pwm sınırlayıcı
  if (ok) {dati=real_data.toInt(); real_data = ""; ok = false;}
    if(dati>=0 and dati<=300){
      int akim = dati;
      if(akim>=85){
        pwm=200;
          if((gecmis_zaman+sure)<=zaman){
             if(pwm != pwm_real){
                if(pwm_real > pwm){
                    pwm_real--;
            }
          }
          gecmis_zaman=zaman; 
        }
    }
    //hiz ayarları
    else if(dati>=1000 and dati<=1999){                     //hiz 1180 ise 180 dir
      data=String(dati); dati=data.substring(1,4).toInt();  //veri parçalanıp datiye geliyor
      //hiz sabitleme kontrolü
        sabit = digitalRead(hiz_sabit);
        if(sabit==HIGH){
           sabit_durumu=!sabit_durumu;
           sabitlenicek_hiz=dati;
           delay(100);
          }else{
            //diğer modlar aktif edilsin hız sabitleme
              if(sabit_durumu==HIGH){
                //hiz sabitleme modu
                if((gecmis_zaman_hiz+sure_hiz)<=zaman){
                         if(dati != sabitlenicek_hiz){
                            if(dati < sabitlenicek_hiz){
                                if(pwm_real!=255){pwm_real++;}
                             }else{
                              if(pwm_real!=0){pwm_real--;}
                              }
                          }
                        gecmis_zaman_hiz=zaman; 
                      }
                }else{
                  //diğer modlar
                 
                  //hiz arttirma
                  art = digitalRead(artir);
                    if(art==HIGH){
                      if((gecmis_zaman+sure)<=zaman){
                         if(255 != pwm_real){
                            if(pwm_real < 255){
                                pwm_real++;
                             }
                          }
                        gecmis_zaman=zaman; 
                      }
                    }
                    //pwm hiz azaltma
                     azal = digitalRead(azalt);
                    if(azal==HIGH){
                        if((gecmis_zaman+sure)<=zaman){
                           if(0 != pwm_real){
                              if(pwm_real > 0){
                                  pwm_real--;
                               }
                            }
                          gecmis_zaman=zaman; 
                        }
                      } 
            }  
        }
      }
}}
void serialEvent(){
  while (Serial.available()){
    char temp = (char)Serial.read();
    real_data += temp;
    if (temp == '\n'){ok = true;}}}
