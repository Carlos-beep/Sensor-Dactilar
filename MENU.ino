#include <InnovaS_Dactilar.h>
#include <SoftwareSerial.h>
SoftwareSerial MySerial(2,3);
char option, n, c;
uint8_t myid = 0, r;
uint32_t ClaveSensorHuella = 0;
InnovaS_Dactilar MySensorDactilar = InnovaS_Dactilar(&MySerial, ClaveSensorHuella);
int id, my_id;
int p = -1;

void setup(){ 
    Serial.begin(57600);
    Serial.println("Sensor de Huella");
    MySensorDactilar.begin(57600);
    if (MySensorDactilar.VerificarClave()) {
    Serial.println("EMPIECE");
     Serial.println(" ");
      Serial.println("MENU DE ADMINISTRADOR: ");
      Serial.println(" ");
      Serial.println ("1.- NUEVO REGISTRO");
      Serial.println ("2.- ELIMINAR USUARIO");
      Serial.println ("3.- LIMPIAR BD");
      Serial.println ("4.- MOSTRAR MENU");      
      Serial.println(" ");
  } else {
    Serial.println("Reinicie el monitor serie");
        Serial.println("Revise los baudios");

    while (1);
  }
}
 //byte TotalRegistrados = (byte)MySensorDactilar.ContarRegistrados();

void loop(){           
 
       if (Serial.available()>0) {
        option = Serial.read();    //Recibe la opcion de el menu
        switch (option) {
  case '1':
  Serial.println("Registrar una huella");
  //Serial.print("El sensor tiene "); Serial.print(TotalRegistrados); Serial.println(" usuarios registrados");
  my_id = 0;
  Serial.println("Ingrese el Numero del ID de la huella a guardar (1 -127): ");
             
  while (true) {
              while (! Serial.available());
              n = Serial.read();
              if (! isdigit(n)) break;
              my_id *= 10;
              my_id += n - '0';
              Serial.println("Pulse p para continiuar ...");
  }
  Serial.print("Enrolando ID #");
  Serial.println(my_id);
  Serial.println("INGRESANDO");
  Serial.println("Esperando una huella valida para registrar");
  
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
      return false; 
    case SDACTILAR_PAQUETE_IN_ERROR:
      Serial.println("Paquetes Errados");
      return false; 
    case SDACTILAR_RASGOSERROR:
      Serial.println("No es posible detectar los rasgos caracteriticos");
      return false; 
    case SDACTILAR_IMGINVALIDA:
      Serial.println("Imagen invalida");
      return false; 
    default:
      Serial.print("Error Desconocido: 0x"); Serial.println(p, HEX);
      return false; 
  }
//------------------------------------
  Serial.println("Remover huella");
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
  // OK success!
  p = -1;
  p = MySensorDactilar.GenerarImg2Tz(2);
  switch (p) {
    case SDACTILAR_OK:
      Serial.println("Imagen Convertida");
      break;
    case SDACTILAR_IMGCONFUSA:
      Serial.println("Image muy confusa");
      return false; 
    case SDACTILAR_PAQUETE_IN_ERROR:
      Serial.println("Paquetes Errados");
      return false; 
    case SDACTILAR_RASGOSERROR:
      Serial.println("No es posible detectar los rasgos caracteriticos");
      return false; 
    case SDACTILAR_IMGINVALIDA:
      Serial.println("Imagen invalida");
      return false; 
    default:
      Serial.print("Error Desconocido: 0x"); Serial.println(p, HEX);
      return false; 
  }
  // OK converted!
  p = -1;
  p = MySensorDactilar.CrearModelo();
  if (p == SDACTILAR_OK) 
  {
    Serial.println("Muestras de Huellas Emparejadas!");
  } else if (p == SDACTILAR_PAQUETE_IN_ERROR) {
    Serial.println("Error de comunicacion");
    return false; 
  } else if (p == SDACTILAR_ENROLL_MISMATCH) {
    Serial.println("Muestras de Huellas NO Emparejadas!");
    return false; 
  } else {
    Serial.print("Error Desconocido: 0x"); Serial.println(p, HEX);
    return false; 
  }   
  Serial.print("ID "); Serial.println(my_id);
  p = MySensorDactilar.GuardarModelo(my_id);
  if (p == SDACTILAR_OK) {
    Serial.println("EXITO - Huella Guardada!");
  } else if (p == SDACTILAR_PAQUETE_IN_ERROR) {
    Serial.println("Error de comunicacion");
    return false; 
  } else if (p == SDACTILAR_ERROR_UBICACION) {
    Serial.println("No se puede ubicar en la ubicacion asignada");
    return false; 
  } else if (p == SDACTILAR_ERROR_FLASH) {
    Serial.println("Error escribiendo en la flash");
    return false; 
  } else {
    Serial.print("Error Desconocido: 0x"); Serial.println(p, HEX);
    return false; 
  }   
  return false; 
  break;

  case '2':
  Serial.println("Eliminar una huella");
  //Serial.print("El sensor tiene "); Serial.print(TotalRegistrados); Serial.println(" usuarios registrados");
  Serial.println("Ingrese el numero del ID que desea eliminar: ");
  while (true) {
    while (! Serial.available());
    char c = Serial.read();
    if (! isdigit(c)) break;
    myid *= 10;
    myid += c - '0'; 
  Serial.print("Caracter p para continuar ....");
  }
  Serial.print("Eliminando ID #");
  Serial.println(myid);
  p = -1;
  p = MySensorDactilar.EliminarModelo(myid);
  if (p == SDACTILAR_OK) {
    Serial.println("ID Eliminado!");
  }
  break;

  case '3':
  //Serial.print("El sensor tiene "); Serial.print(TotalRegistrados); Serial.println(" usuarios registrados");
        Serial.println("Eliminando todas las huellas ...");
  r = MySensorDactilar.VaciarBaseDatos();
    switch (r) {
    case SDACTILAR_BD_OK:
      Serial.println("Exito - Base de Datos Eliminada");
      break;
    default:
      Serial.println("Exito - Base de Datos Eliminada");
      break;
    }
  break;

  default:
  Serial.println(" ");
      Serial.println("MENU DE ADMINISTRADOR ");
      Serial.println(" ");
      Serial.println ("1.- NUEVO REGISTRO");
      Serial.println ("2.- ELIMINAR USUARIO");
      Serial.println ("3.- LIMPIAR BD"); 
      Serial.println ("4.- MOSTRAR MENU");
      Serial.println(" ");

}
}
}

