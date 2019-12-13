// Teensy 3.2 uC @ 24MHz

// Variables
int Cartridge = 0;
int CartType[] = {0,1,1,0}; //0 = Support, 1 = Model
int delaytimeout = 1;

// Stock Requests
byte RequestInitial[] = {0x02, 0x56};
byte RequestCrt0[] = {0x03, 0xAF, 0x00};             // Request Cartridge #0
byte RequestCrt1[] = {0x03, 0xAF, 0x04};             //                   #1
byte RequestCrt2[] = {0x03, 0xAF, 0x08};             //                   #2
byte RequestCrt3[] = {0x03, 0xAF, 0x0C};             //                   #3
byte RequestCrtInfo[] = {0x03, 0x79, 0x01};          // Request Cartridge Info
byte RequestCrtStatus[] = {0x07, 0x7A};              //                   Status
byte RequestCrtType[] = {0x04, 0x50, 0x00, 0x08};    //                   Type
byte RequestCrtExpiry[] = {0x04, 0x50, 0x00, 0x0D};  //                   Expiry Date
byte RequestUnknown0[] = {0x04, 0x50, 0x00, 0x01};   // Request Unknown #0
byte RequestUnknown1[] = {0x04, 0x50, 0x00, 0x0C};   //                 #1
byte RequestUnknown2[] = {0x04, 0x50, 0x00, 0x0E};   //                 #2

// Stock Responses
byte ResponseInitial[] = {0x1D,  0x0, 0x56, 0x36, 0x2E, 0x30, 0x31, 0x2E, 0x35, 0x78,
                          0x30, 0x31, 0x2D, 0x30, 0x31, 0x2D, 0x30, 0x31, 0x41, 0x42,
                          0x43, 0x44, 0x00, 0x00, 0x00, 0x31, 0x32, 0x33, 0x34, 0x30};
byte ResponseCrtInfo[] = {0x7, 0x0, 0x01, 0x0, 0x0, 0x01, 0x0, 0x00};
byte ResponseCrtStatusPresent[] = {0x6, 0x0, 0xCA, 0x0, 0x0, 0xAA, 0x66};
byte ResponseError[] = {0x2, 0xFF, 0xFD};
byte ResponseNoop[] = {0x2, 0x0, 0x2};
byte ResponseSupport[] = {0x6, 0x0, 0x1, 0x0, 0xC2, 0x2, 0xC7};
byte ResponseModel[] = {0x6, 0x0, 0x1, 0x0, 0x62, 0x2, 0x67};
// SUP705 - {0x6, 0x0, 0x1, 0x0, 0xC1, 0x2, 0xC4}
// SUP706 - {0x6, 0x0, 0x1, 0x0, 0xC2, 0x2, 0xC7}
// MED610 - {0x6, 0x0, 0x1, 0x0, 0x62, 0x2, 0x67}

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() >= 3) {
    byte comData[Serial.available()-1]; // Create Length+Payload array.
    byte comChck[1]; // Create Checksum array.
    Serial.readBytes((char *) comData, sizeof(comData)); // Read data into arrays.
    Serial.readBytes((char *) comChck, sizeof(comChck));
    
    if(sizeof(comData) != comData[0]){ // Reject incorrect data length.
      for (unsigned int i = 0; i < sizeof(ResponseError); i++) {
        Serial.write(ResponseError[i]); // Write bytes out.
        delay(delaytimeout); // Delay helps wait for bytes to be accepted by uC.
      }
      return; // Exit.
    }

    byte chksm[1] = {0}; // Create checksum byte = 0
    // For each byte in the string (including the data length byte).
    for (unsigned int i = 0; i < sizeof(comData); i++) {
      chksm[0] = chksm[0] ^ comData[i]; // Xor result with next byte.
    }

    // Reject incorrect data checksum.
    if(chksm[0] != comChck[0]){
      for (unsigned int i = 0; i < sizeof(ResponseError); i++) {
        Serial.write(ResponseError[i]);
        delay(delaytimeout);
      }
      return;
    }
    
    // RequestInitial
    if(chckBytes(comData, RequestInitial, sizeof(RequestInitial))){
      for (unsigned int i = 0; i < sizeof(ResponseInitial); i++) {
        Serial.write(ResponseInitial[i]);
        delay(delaytimeout);
      }
      return;
    }
    
    // RequestCrt0
    if(chckBytes(comData, RequestCrt0, sizeof(RequestCrt0))){
      Cartridge = 0;
      for (unsigned int i = 0; i < sizeof(ResponseNoop); i++) {
        Serial.write(ResponseNoop[i]); //
        delay(delaytimeout);
      }
      return;
    }
    
    // RequestCrt1
    if(chckBytes(comData, RequestCrt1, sizeof(RequestCrt1))){
      Cartridge = 1;
      for (unsigned int i = 0; i < sizeof(ResponseNoop); i++) {
        Serial.write(ResponseNoop[i]);
        delay(delaytimeout);
      }
      return;
    }    
    
    // RequestCrt2
    if(chckBytes(comData, RequestCrt2, sizeof(RequestCrt2))){
      Cartridge = 2;
      for (unsigned int i = 0; i < sizeof(ResponseNoop); i++) {
        Serial.write(ResponseNoop[i]);
        delay(delaytimeout);
      }
      return;
    }
    
    // RequestCrt3
    if(chckBytes(comData, RequestCrt3, sizeof(RequestCrt3))){
      Cartridge = 3;
      for (unsigned int i = 0; i < sizeof(ResponseNoop); i++) {
        Serial.write(ResponseNoop[i]);
        delay(delaytimeout);
      }
      return;
    }

    // RequestCrtInfo
    if(chckBytes(comData, RequestCrtInfo, sizeof(RequestCrtInfo))){
      for (unsigned int i = 0; i < sizeof(ResponseCrtInfo); i++) {
        Serial.write(ResponseCrtInfo[i]);
        delay(delaytimeout);
      }
      return;
    }

    // RequestCrtStatus
    if(chckBytes(comData, RequestCrtStatus, sizeof(RequestCrtStatus))){
      for (unsigned int i = 0; i < sizeof(ResponseCrtStatusPresent); i++) {
        Serial.write(ResponseCrtStatusPresent[i]);
        delay(delaytimeout);
      }
      return;
    }

    // RequestCrtType
    if(chckBytes(comData, RequestCrtType, sizeof(RequestCrtType))){
      if(CartType[Cartridge] == 0) {
        for (unsigned int i = 0; i < sizeof(ResponseSupport); i++) {
          Serial.write(ResponseSupport[i]);
          delay(delaytimeout);
        }  
      }
      if(CartType[Cartridge] == 1) {
        for (unsigned int i = 0; i < sizeof(ResponseModel); i++) {
          Serial.write(ResponseModel[i]);
          delay(delaytimeout);
        }  
      }
      return;
    }

    // RequestCrtexpiry
    if(chckBytes(comData, RequestCrtExpiry, sizeof(RequestCrtExpiry))){
      for (unsigned int i = 0; i < sizeof(ResponseError); i++) {
        Serial.write(ResponseError[i]);
        delay(delaytimeout);
      }
      return;
    }

    // Unknown0
    if(chckBytes(comData, RequestUnknown0, sizeof(RequestUnknown0))){
      for (unsigned int i = 0; i < sizeof(ResponseCrtStatusPresent); i++) {
        Serial.write(ResponseCrtStatusPresent[i]);
        delay(delaytimeout);
      }
      return;
    }

    // Unknown1
    if(chckBytes(comData, RequestUnknown1, sizeof(RequestUnknown1))){
      for (unsigned int i = 0; i < sizeof(ResponseError); i++) {
        Serial.write(ResponseError[i]);
        delay(delaytimeout);
      }
      return;
    }

    // Unknown2
    if(chckBytes(comData, RequestUnknown2, sizeof(RequestUnknown2))){
      for (unsigned int i = 0; i < sizeof(ResponseError); i++) {
        Serial.write(ResponseError[i]); // Write err.
        delay(delaytimeout);
      }
      return;
    }
  }
}

void writeObjet(byte msg[]) {
  for (int i = 0; i < sizeof(msg); i++) {
    Serial.write(msg[i]);
    delay(delaytimeout);
  }
}

boolean chckBytes(byte bytesA[],byte bytesB[], long bytes) {
  boolean data = true;
  long i = 0;
  while(i<bytes && data) { 
    data = bytesA[i] == bytesB[i];
    i++;
  }
  return data;
}
