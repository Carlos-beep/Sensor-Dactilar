#include <InnovaS_Dactilar.h>
#include <SoftwareSerial.h>
SoftwareSerial MySerial(2, 3);
char opcion, n, v, leer = 0, op = 0, p = -1, band = 1;
int my_id, id = 0, abrir, chapa = 13, boton = 6, estado = 0, t = 100, sos = 12;
uint8_t my_id1;
uint32_t ClaveSensorHuella = 0;
char inChar, option = "";
String idl = "", reg = "";
unsigned long tiempo, compi;
InnovaS_Dactilar MySensorDactilar = InnovaS_Dactilar(&MySerial, ClaveSensorHuella);
    byte TotalRegistrados = (byte)MySensorDactilar.ContarRegistrados();

void setup() {
  pinMode (boton, INPUT);
  pinMode (chapa, OUTPUT);
  pinMode (sos, OUTPUT);

  Serial.begin(57600);
  Serial.println("Sensor de Huella, los baudios deben estar a 57600");
  MySensorDactilar.begin(57600);

  if (MySensorDactilar.VerificarClave()) {
    Serial.println("listo para usar :)");
  } else {
    Serial.println("Cierre el puerto y vuelva a abrir");
    while (1);
  }
}

void loop() {
  //---------------RECIBE UN ALTO DE EL BOTON INTERNO--------------
      digitalWrite (chapa, LOW) ;
      Serial.println(" ");
      Serial.println("Menu de administrador: ");
      Serial.println ("1.- NUEVO REGISTRO");
      Serial.println ("2.- ELIMINAR USUARIO");
      Serial.println ("3.- LIMPIAR BD");
      
      //--RECIBE EL NUMERO DE LA OPCION----------------
      if (Serial.available()>0) {
        option = Serial.read();    //Recibe la opcion de el menu
        switch (option) {
          case '1':
            Serial.print("El sensor tiene ");
            Serial.print(TotalRegistrados);
            Serial.println(" usuarios registrados");
            //-------------------REGISTRAR HUELLA-----------------
            my_id = 0;
            Serial.println(" ");
            Serial.println ("Añadir usuario");
            Serial.println(" ");
            Serial.println("Ingrese el Numero del ID de la huella a guardar (1 -127): ");

              while (! Serial.available());
              n = Serial.read();
              if (! isdigit(n)) break;
              my_id *= 10;
              my_id += n - '0';

            Serial.print("Guardando huella en ID #");
            Serial.println(my_id);
            p = -1;
            Serial.println("INGRESANDO");
            Serial.println("Coloque una huella valida para guardar");
            //------------------------------------------------------------------
            while (p != SDACTILAR_OK) {
              p = MySensorDactilar.CapturarImagen();
              switch (p) {
                case SDACTILAR_OK:
                  Serial.println(" ");
                  Serial.println("Imagen Tomada");
                  break;
                case SDACTILAR_D_NO_DETECTADO:
                  break;
                case SDACTILAR_PAQUETE_IN_ERROR:
                  Serial.println("Error al recibir el paquete");
                  break;
                case SDACTILAR_IMG_ERROR:
                  Serial.println("Error al determinar la imagen");
                  break;
                default:
                  Serial.print("Error Desconocido: 0x"); Serial.println(p, HEX);
                  break;
              }
            }
            // OK success!
            p = -1;
            p = MySensorDactilar.GenerarImg2Tz(1);
            switch (p) {
              case SDACTILAR_OK:
                Serial.println("Imagen Convertida");
                break;
              case SDACTILAR_IMGCONFUSA:
                Serial.println("Image muy confusa");
                break;
              case SDACTILAR_PAQUETE_IN_ERROR:
                Serial.println("Paquetes Errados");
                break;
              case SDACTILAR_RASGOSERROR:
                Serial.println("No es posible detectar los rasgos caracteriticos");
                break;
              case SDACTILAR_IMGINVALIDA:
                Serial.println("Imagen invalida");
                break;
              default:
                Serial.print("Error Desconocido: 0x"); Serial.println(p, HEX);
            }
            //------------------------------------
            Serial.println("Remueva la huella");
            delay(2000);
            p = -1;
            while (p != SDACTILAR_D_NO_DETECTADO) {
              p = MySensorDactilar.CapturarImagen();
            }
            p = -1;
            Serial.println("Por favor vuelva a poner nuevamente la huella");
            while (p != SDACTILAR_OK) {
              p = MySensorDactilar.CapturarImagen();
              switch (p) {
                case SDACTILAR_OK:
                  Serial.println(" ");
                  Serial.println("Imagen Tomada");
                  break;
                case SDACTILAR_D_NO_DETECTADO:
                  break;
                case SDACTILAR_PAQUETE_IN_ERROR:
                  Serial.println("Error al recibir el paquete");
                  break;
                case SDACTILAR_IMG_ERROR:
                  Serial.println("Error al determinar la imagen");
                  break;
                default:
                  Serial.print("Error Desconocido: 0x"); Serial.println(p, HEX);
                  break;
              }
            }
            // OK!
            p = -1;
            p = MySensorDactilar.GenerarImg2Tz(2);
            switch (p) {
              case SDACTILAR_OK:
                Serial.println("Imagen Convertida");
                break;
              case SDACTILAR_IMGCONFUSA:
                Serial.println("Image muy confusa");
                break;
              case SDACTILAR_PAQUETE_IN_ERROR:
                Serial.println("Paquetes Errados");
                break;
              case SDACTILAR_RASGOSERROR:
                Serial.println("No es posible detectar los rasgos caracteriticos");
                break;
              case SDACTILAR_IMGINVALIDA:
                Serial.println("Imagen invalida");
                break;
              default:
                Serial.print("Error Desconocido: 0x"); Serial.println(p, HEX);
            }
            // OK converted!
            p = -1;
            p = MySensorDactilar.CrearModelo();
            if (p == SDACTILAR_OK)
            { 
              Serial.println("Muestras de Huellas Registradas!");
            } else if (p == SDACTILAR_PAQUETE_IN_ERROR) {
              Serial.println("Error de comunicacion");
            } else if (p == SDACTILAR_ENROLL_MISMATCH) {
              Serial.println("Muestras de Huellas NO Emparejadas!");
            } else {
              Serial.print("Error Desconocido: 0x"); Serial.println(p, HEX);
            }
            Serial.print("ID "); Serial.println(my_id);
            p = MySensorDactilar.GuardarModelo(my_id);
            if (p == SDACTILAR_OK) {
              Serial.println("EXITO - Huella Guardada!");
            } else if (p == SDACTILAR_PAQUETE_IN_ERROR) {
              Serial.println("Error de comunicacion");
            } else if (p == SDACTILAR_ERROR_UBICACION) {
              Serial.println("No se puede ubicar en la ubicacion asignada");
            } else if (p == SDACTILAR_ERROR_FLASH) {
              Serial.println("Error escribiendo en la flash");
            } else {
              Serial.print("Error Desconocido: 0x"); Serial.println(p, HEX);
            }

            return false;
            //------------------------------------------------------------------
            break;
          case '2':
            //-------------ELIMINAR HUELLA-----------------------
            Serial.println(" ");
            Serial.print("El sensor tiene ");
            Serial.print(TotalRegistrados);
            Serial.println(" usuarios registrados");
            Serial.println ("Elimiar huellas");
            Serial.println("Ingrese el numero del ID que desea eliminar: ");

            my_id1 = 0;
              while (! Serial.available());
              v = Serial.read();
              if (! isdigit(v)) break;
              my_id1 *= 10;
              my_id1 += v - '0';
              
            Serial.print("Eliminando ID #"); Serial.println(my_id1);
            p = -1;
            p = MySensorDactilar.EliminarModelo(my_id1);

            if (p == SDACTILAR_OK) {
              Serial.println("ID Eliminado!");
            }
            Serial.print("El sensor tiene ");
            Serial.print(TotalRegistrados);
            Serial.println(" usuarios registrados");
            return false;
            break;
     //-----------------LIMPIAR BASE DE DATOS--------------
          case '3':
            Serial.println("Limpiando la base de datos");
            p = MySensorDactilar.VaciarBaseDatos();
            Serial.print("El sensor tiene ");
            Serial.print(TotalRegistrados);
            Serial.println(" usuarios registrados");
            switch (p) {
              case SDACTILAR_BD_OK:
                Serial.println("Exito - Base de Datos Eliminada");
                break;
              default:
                Serial.print("Exito - Base de Datos Eliminada");
                break;
            }
            //--------LA PROXIMA HUELLA EN REGISTRAR ES DEL ADMINISTRADOR---------
            my_id = 1;
            Serial.println(" ");
            Serial.println("Coloque una huella para registrar");
            while (true) {
              n = 0;
              if (! isdigit(n)) break;
              my_id *= 10;
              my_id += n - '0';
              Serial.println("Ingrese p para continuar");
            }
            Serial.print("Guardando huella del administrador ...");
            Serial.println(my_id);
            p = -1;
            Serial.println("Esperando una huella valida para guardar");
            //------------------------------------------------------------------
            while (p != SDACTILAR_OK) {
              p = MySensorDactilar.CapturarImagen();
              switch (p) {
                case SDACTILAR_OK:
                  Serial.println(" ");
                  Serial.println("Imagen Tomada");
                  break;
                case SDACTILAR_D_NO_DETECTADO:
                  break;
                case SDACTILAR_PAQUETE_IN_ERROR:
                  Serial.println("Error al recibir el paquete");
                  break;
                case SDACTILAR_IMG_ERROR:
                  Serial.println("Error al determinar la imagen");
                  break;
                default:
                  Serial.print("Error Desconocido: 0x"); Serial.println(p, HEX);
                  break;
              }
            }
            p = -1;
            p = MySensorDactilar.GenerarImg2Tz(1);
            switch (p) {
              case SDACTILAR_OK:
                Serial.println("Imagen Convertida");
                break;
              case SDACTILAR_IMGCONFUSA:
                Serial.println("Image muy confusa");
                break;
              case SDACTILAR_PAQUETE_IN_ERROR:
                Serial.println("Paquetes Errados");
                break;
              case SDACTILAR_RASGOSERROR:
                Serial.println("No es posible detectar los rasgos caracteriticos");
                break;
              case SDACTILAR_IMGINVALIDA:
                Serial.println("Imagen invalida");
                break;
              default:
                Serial.print("Error Desconocido: 0x"); Serial.println(p, HEX);
            }
            //------------------------------------
            Serial.println("Remueva la huella");
            delay(2000);
            p = -1;
            while (p != SDACTILAR_D_NO_DETECTADO) {
              p = MySensorDactilar.CapturarImagen();
            }
            p = -1;
            Serial.println("Por favor vuelva a poner nuevamente la huella");
            while (p != SDACTILAR_OK) {
              p = MySensorDactilar.CapturarImagen();
              switch (p) {
                case SDACTILAR_OK:
                  Serial.println(" ");
                  Serial.println("Imagen Tomada");
                  break;
                case SDACTILAR_D_NO_DETECTADO:
                  break;
                case SDACTILAR_PAQUETE_IN_ERROR:
                  Serial.println("Error al recibir el paquete");
                  break;
                case SDACTILAR_IMG_ERROR:
                  Serial.println("Error al determinar la imagen");
                  break;
                default:
                  Serial.print("Error Desconocido: 0x"); Serial.println(p, HEX);
                  break;
              }
            }
            p = -1;
            p = MySensorDactilar.GenerarImg2Tz(2);
            switch (p) {
              case SDACTILAR_OK:
                Serial.println("Imagen Convertida");
                break;
              case SDACTILAR_IMGCONFUSA:
                Serial.println("Image muy confusa");
                break;
              case SDACTILAR_PAQUETE_IN_ERROR:
                Serial.println("Paquetes Errados");
                break;
              case SDACTILAR_RASGOSERROR:
                Serial.println("No es posible detectar los rasgos caracteriticos");
                break;
              case SDACTILAR_IMGINVALIDA:
                Serial.println("Imagen invalida");
                break;
              default:
                Serial.print("Error Desconocido: 0x"); Serial.println(p, HEX);
            }
            p = -1;
            p = MySensorDactilar.CrearModelo();
            if (p == SDACTILAR_OK)
            {
              Serial.println("Muestras de Huellas Emparejadas!");
            } else if (p == SDACTILAR_PAQUETE_IN_ERROR) {
              Serial.println("Error de comunicacion");
            } else if (p == SDACTILAR_ENROLL_MISMATCH) {
              Serial.println("Muestras de Huellas NO Emparejadas!");
            } else {
              Serial.print("Error Desconocido: 0x"); Serial.println(p, HEX);
            }
            Serial.print("ID "); Serial.println(my_id);
            p = MySensorDactilar.GuardarModelo(my_id);
            if (p == SDACTILAR_OK) {
              Serial.println("EXITO - Huella Guardada!");
            } else if (p == SDACTILAR_PAQUETE_IN_ERROR) {
              Serial.println("Error de comunicacion");
            } else if (p == SDACTILAR_ERROR_UBICACION) {
              Serial.println("No se puede ubicar en la ubicacion asignada");
            } else if (p == SDACTILAR_ERROR_FLASH) {
              Serial.println("Error escribiendo en la flash");
            } else {
              Serial.print("Error Desconocido: 0x"); Serial.println(p, HEX);
            }
            
            return false;
            break;
          default:
          band = 2;
          Serial.println(" ");
            Serial.println ("Opcion no valida en este menu");
            return false;
            break;
        }
      }
    }
