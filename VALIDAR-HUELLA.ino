#include <InnovaS_Dactilar.h>
#include <SoftwareSerial.h>
SoftwareSerial MySerial(2, 3);
uint32_t ClaveSensorHuella = 0;
InnovaS_Dactilar MySensorDactilar = InnovaS_Dactilar(&MySerial, ClaveSensorHuella);
int p = -1, abrir, id=0, chapa = 13, boton = 6, t = 2500;
void setup()
{  
    pinMode (boton, INPUT);
    pinMode (chapa, OUTPUT);
    MySensorDactilar.begin(57600);
    Serial.begin(57600);
}
void loop()
{
  digitalWrite (chapa, LOW);
  abrir = digitalRead (boton); 
  //---------------RECIBE UN ALTO DE EL BOTON INTERNO-------------- 
  if (abrir == HIGH) {
    digitalWrite (chapa, HIGH);
    delay(t);
    digitalWrite (chapa, LOW);
  }
    //-------EMPIEZA A VALIDAR HUELLAS---------------
    digitalWrite (chapa, LOW);
    p = MySensorDactilar.CapturarImagen();
    if (p != SDACTILAR_OK)  return;

    p = MySensorDactilar.GenerarImg2Tz();
    if (p != SDACTILAR_OK)  return;

    p = MySensorDactilar.BusquedaRapida();
    if (p != SDACTILAR_OK)  return;
    id = MySensorDactilar.Id_Dactilar;
    
      //CUALQUIER OTRA HUELLA ES DE ALGUN USUARIO
      digitalWrite (chapa, HIGH);
      Serial.print(id);
      Serial.print(",");
      delay (t);
      digitalWrite (chapa, LOW);
  }
