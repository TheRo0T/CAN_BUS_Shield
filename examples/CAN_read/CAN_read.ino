#include <SPI.h>
#include <mcp_can.h>

struct can_frame canMsg;
MCP_CAN mcp2515(10, MCP_CAN::MODE_NORMAL);


void setup() {
  Serial.begin(115200);
  mcp2515.begin(CAN_125KBPS);
  Serial.println("------- CAN Read ----------");
  Serial.println("ID  DLC   DATA");
}

void loop() {
  
  if (mcp2515.readMessage(&canMsg) == MCP_CAN::ERROR_OK) {
      
    Serial.print(canMsg.can_id, HEX); // print ID
    Serial.print(" "); 
    Serial.print(canMsg.can_dlc,HEX); // print DLC
    Serial.print(" ");
    
    for (int i = 0; i<canMsg.can_dlc; i++)  {  // print the data
        
      Serial.print(canMsg.data[i],HEX);
      Serial.print(" ");

    }

    Serial.println();      
  }

}
