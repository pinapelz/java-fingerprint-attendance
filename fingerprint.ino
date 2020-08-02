
# include <Adafruit_Fingerprint.h>
 


SoftwareSerial mySerial(2, 3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
uint8_t id;
int t = -1;

void setup()  
{

    digitalWrite(10, HIGH);
  Serial.begin(9600);
  while (!Serial); 
  delay(100);
  finger.begin(57600);
  
    if (finger.verifyPassword()) {
   // Serial.println("Found fingerprint sensor!");
  } else {
   // Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  finger.getTemplateCount();
  //Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");


}

void loop()                   
{
  
char c = Serial.read();
if(c=='k'){
    for(int i  =0;i<1000;i++){
   Serial.println("");
 }
}
if(c=='e'){
  Serial.flush();
   
  id = readnumber();
  if (id == 0) {//ID #0 will cause errors so it is commented out
     return;
  }
  //Serial.print("Enrolling ID #");
 // Serial.println(id);
  getFingerprintEnroll();





}



if(c=='f'){

  Serial.flush();
    for(int i  =0;i<100;i++){
   Serial.println("");
 }
 int attempts = 0;
  while(t==-1){
    t =getFingerprintIDez();
    attempts = attempts + 1;
  }
  attempts = 0;
 for(int i  =0;i<100;i++){
   Serial.println(t);
 }
  t = -1;
 
  digitalWrite(10, LOW);

}



if(c=='m'){
  Serial.flush();
   // Serial.println("Please type in the ID # (from 1 to 127) you want to delete...");
  uint8_t id = readnumber();
  if (id == 0) {// again id 0 causes errors
     return;
  }

//  Serial.print("Deleting ID #");
 // Serial.println(id);
  
  deleteFingerprint(id);


}


if(c=='d'){
  finger.emptyDatabase();
    for(int i = 0;i<5;i++){

  }
}
  delay(50);            
}





int getFingerprintIDez() {

  delay(2000);
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
//  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
 // Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}





uint8_t getFingerprintEnroll() {
  

 int p = -1;
 // Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
   //   Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
    //  Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
    //  Serial.println("Imaging error");
      break;
    default:
     // Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
    //  Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
     // Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
    //  Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
    //  Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
     // Serial.println("Could not find fingerprint features");
      return p;
    default:
     // Serial.println("Unknown error");
      return p;
  }
  
  //Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  //Serial.print("ID "); Serial.println(id);
  p = -1;
  //Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
    //  Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
    //  Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
    //  Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
    //  Serial.println("Imaging error");
      break;
    default:
    //  Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
     // Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
    //  Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
     // Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
     // Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
    //  Serial.println("Could not find fingerprint features");
      return p;
    default:
    //  Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
 // Serial.print("Creating model for #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
   // Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
  //  Serial.println("Fingerprints did not match");
    return p;
  } else {
   // Serial.println("Unknown error");
    return p;
  }   
  
 // Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
  //  Serial.println("Stored!");
    
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
   // Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
  //  Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
   // Serial.println("Error writing to flash");
    return p;
  } else {
  //  Serial.println("Unknown error");
    return p;
  }  
} 





uint8_t readnumber(void) {
  uint8_t num = 0;
  
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}





uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;
  
  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
//    Serial.println("Deleted!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
   // Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
  //  Serial.println("Could not delete in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
   // Serial.println("Error writing to flash");
    return p;
  } else {
  //  Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
    return p;
  }   
}  
