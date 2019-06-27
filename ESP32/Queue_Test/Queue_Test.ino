QueueHandle_t queue0;
int queue_cnt = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  queue0 = xQueueCreate(10, sizeof(byte));

  if (queue0 == NULL){
      Serial.println("The Queue Wasn't Successfully Created");
    }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (queue0 == NULL) return;

  // Write 0~9 Into the queue0
  for (int i = 0;i<10;i++){
      xQueueSend(queue0, &i, portMAX_DELAY);
      queue_cnt = queue_cnt + 1;
    }
    
  int element;
  //Read From the queue
  for (int i = 0;i<10;i++){
      xQueueReceive(queue0, &element, portMAX_DELAY);
      queue_cnt = queue_cnt - 1;
      Serial.print(element);
      Serial.print("|");
    }
    
   Serial.println();
   delay(500);
  
}
