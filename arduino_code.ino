#include <Servo.h>

//Servo motors (in the hand)
Servo thumb_servo;
Servo index_servo;
Servo middle_servo;
Servo ring_servo;
Servo little_servo;

//Mode pins
int demo = 2;
int playback = 4;
int button = 19;

//Hand pins
int thumb = 7;
int index = 13;
int middle = 12;
int ring = 11;
int little = 8;

//LED pins
int thumb_led =10;
int index_led = 9;
int middle_led = 6;
int ring_led = 5;
int little_led = 3;

//Photodiode pins
int thumb_pd_pin = A4;
int index_pd_pin = A3;
int middle_pd_pin = A2;
int ring_pd_pin = A1;
int little_pd_pin = A0;

//Maximum fingers opening (stretched fingers)
const int max_thumb = 170;
const int max_index = 135;
const int max_middle = 130;
const int max_ring = 165;
const int max_little = 155;

//Minimum fingers opening (closed fingers)
const int min_thumb = 100;
const int min_index = 50;
const int min_middle = 40;
const int min_ring = 80;
const int min_little = 50;

//Fingers opening halfway
//(used to show hand coarse movement for when skull is over the PCB)
const int mid_thumb = 130;
const int mid_index = 95;
const int mid_middle = 85;
const int mid_ring = 115;
const int mid_little = 95;

//Resting shape in rock paper scissors 
//(because otherwise will get confused with the rock shape)
const int rest_thumb = 117;
const int rest_index = 75;
const int rest_middle = 65;
const int rest_ring = 100;
const int rest_little = 75;

//Photodiode initial values
int thumb_pd = 0;
int index_pd = 0;
int middle_pd = 0;
int ring_pd = 0;
int little_pd = 0;

//Laser detection thresholds
int upper_threshold = 200; // for when the skull is off
int lower_threshold = 85; // for when the skull is on
// if system not detecting laser through skull, the lighting conditions may have changed, so adjust the lower_threshold:
// - lower for darker conditions
// - higher for brighter conditions
// if when adjusting the threshold, the fingers start wiggling fast, youre very close to the ideal threshold for the environment.

//Operation switches - all initially disabled
int switch1 = 0;
int switch2 = 0;
int button_state = 0;

void setup()
{
  Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
  
  //attach fingers to pin
  thumb_servo.attach(thumb);  
  index_servo.attach(index);  
  middle_servo.attach(middle);
  ring_servo.attach(ring); 
  little_servo.attach(little);     

  //Photodiode setup
  pinMode(thumb_pd_pin, INPUT); 
  pinMode(index_pd_pin, INPUT); 
  pinMode(middle_pd_pin, INPUT); 
  pinMode(ring_pd_pin, INPUT); 
  pinMode(little_pd_pin, INPUT); 

  //LED setup
  pinMode(thumb_led, OUTPUT);
  pinMode(index_led, OUTPUT);
  pinMode(middle_led, OUTPUT);
  pinMode(ring_led, OUTPUT);
  pinMode(little_led, OUTPUT);

  //Switch & button setup
  pinMode(demo, INPUT);
  pinMode(playback, INPUT);
  pinMode(button, INPUT);

  //Hand starts closed
  thumb_servo.write(min_thumb);
  index_servo.write(min_index);
  middle_servo.write(min_middle);
  ring_servo.write(min_ring);
  little_servo.write(min_little);

  //LEDs start off
  digitalWrite(thumb_led, HIGH);
  digitalWrite(index_led, HIGH);
  digitalWrite(middle_led, HIGH);
  digitalWrite(ring_led, HIGH);
  digitalWrite(little_led, HIGH);
}

void controlDigits(int val, int led, const int straightFinger, const int bentFinger, const int midFinger, Servo servo){
  //One finger and pdvalue read into this function at a time
  //Reads in the value being read by the photodiode and according to the value assigns a certain movement:
  // - above the laser threshold, the laser has been detected and finger extends
  // - 25 < val < threshold means a small amount of light has been detected, this occurs when laser is shined through the skull
  
  if(val >= upper_threshold){
    //Photodiodes have detected the laser
    servo.write(straightFinger); // finger extends
    digitalWrite(led, LOW); // LED turns on
    delay(750);
  }
  else{
    //No light detected- fingers bent   
    servo.write(bentFinger); // finger bends again
    digitalWrite(led, HIGH); // LED turns off
  }
}
void controlDigits_withSkull(int val, int led, const int straightFinger, const int bentFinger, const int midFinger, Servo servo){
  //One finger and pdvalue read into this function at a time
  //Reads in the value being read by the photodiode and according to the value assigns a certain movement:
  // - above the laser threshold, the laser has been detected and finger extends
  // - 25 < val < threshold means a small amount of light has been detected, this occurs when laser is shined through the skull
  
  if(val >= lower_threshold){
    //Photodiodes have detected the laser
    servo.write(midFinger); // finger extends
    digitalWrite(led, LOW); // LED turns on
  }
  else{
    //No light detected- fingers bent   
    servo.write(bentFinger); // finger bends again
    digitalWrite(led, HIGH); // LED turns off
  }
}

void one_at_a_time(){
  //Each finger extends and then returns to resting one at a time
  //Corresponding LED lights up as an indication
  //Delay needed before finger bending as otherwise Hand won't do anything
  
  //thumb
  thumb_servo.write(max_thumb);
  digitalWrite(thumb_led, LOW);
  delay(500);
  thumb_servo.write(min_thumb);
  digitalWrite(thumb_led, HIGH);
  delay(500);

  //Index finger
  index_servo.write(max_index);
  digitalWrite(index_led, LOW);
  delay(500);
  index_servo.write(min_index);
  digitalWrite(index_led, HIGH);
  delay(500);

  //Middle finger
  middle_servo.write(max_middle);
  digitalWrite(middle_led, LOW);
  delay(500);
  middle_servo.write(min_middle);
  digitalWrite(middle_led, HIGH);
  delay(500);

  //Ring finger
  ring_servo.write(max_ring);
  digitalWrite(ring_led, LOW);
  delay(500);
  ring_servo.write(min_ring);
  digitalWrite(ring_led, HIGH);
  delay(500);

  // little finger
  little_servo.write(max_little);
  digitalWrite(little_led, LOW);
  delay(500);
  little_servo.write(min_little);
  digitalWrite(little_led, HIGH);
  delay(1000);
}

void count(){
  //Each finger extends one at a time, mimicking counting
  //Corresponding LED lights up as an indication
  //Delay between each finger movement otherwise all will move together
  
  // 1
  thumb_servo.write(max_thumb);
  digitalWrite(thumb_led, LOW);
  delay(500);

  // 2
  index_servo.write(max_index);
  digitalWrite(index_led, LOW);
  delay(500);

  // 3
  middle_servo.write(max_middle);
  digitalWrite(middle_led, LOW);
  delay(500);

  // 4
  ring_servo.write(max_ring);
  digitalWrite(ring_led, LOW);
  delay(500);

  // 5
  little_servo.write(max_little);
  digitalWrite(little_led, LOW);
  delay(500);

  // off
  thumb_servo.write(min_thumb);
  index_servo.write(min_index);
  middle_servo.write(min_middle);
  ring_servo.write(min_ring);
  little_servo.write(min_little);
  digitalWrite(thumb_led, HIGH);
  digitalWrite(index_led, HIGH);
  digitalWrite(middle_led, HIGH);
  digitalWrite(ring_led, HIGH);
  digitalWrite(little_led, HIGH);
  delay(1000);
  
}

void rock_star(){
  //Hand makes the 'rock' gesture
  //Corresponding LEDs light up

  //thumb
  thumb_servo.write(max_thumb);
  digitalWrite(thumb_led, LOW);

  //index finger
  index_servo.write(max_index);
  digitalWrite(index_led, LOW);
  
  //little finger
  little_servo.write(max_little);
  digitalWrite(little_led, LOW);
  
  delay(500);

  //off
  thumb_servo.write(min_thumb);
  index_servo.write(min_index);
  little_servo.write(min_little);
  digitalWrite(thumb_led, HIGH);
  digitalWrite(index_led, HIGH);
  digitalWrite(little_led, HIGH);
  delay(1000);
}

void rock(){
  //Hand makes the 'rock' gesture from rock paper scissors

  thumb_servo.write(min_thumb);
  index_servo.write(min_index);
  middle_servo.write(min_middle);
  ring_servo.write(min_ring);
  little_servo.write(min_little);
  digitalWrite(thumb_led, LOW);
  digitalWrite(index_led, LOW);
  digitalWrite(middle_led, LOW);
  digitalWrite(ring_led, LOW);
  digitalWrite(little_led, LOW);

  delay(1000);

  // back to resting
  thumb_servo.write(rest_thumb);
  index_servo.write(rest_index);
  middle_servo.write(rest_middle);
  ring_servo.write(rest_ring);
  little_servo.write(rest_little);
  digitalWrite(thumb_led, HIGH);
  digitalWrite(index_led, HIGH);
  digitalWrite(middle_led, HIGH);
  digitalWrite(ring_led, HIGH);
  digitalWrite(little_led, HIGH);
}

void paper(){
  //Hand makes the 'paper' gesture from rock paper scissors
  
  thumb_servo.write(max_thumb);
  index_servo.write(max_index);
  middle_servo.write(max_middle);
  ring_servo.write(max_ring);
  little_servo.write(max_little);
  digitalWrite(thumb_led, LOW);
  digitalWrite(index_led, LOW);
  digitalWrite(middle_led, LOW);
  digitalWrite(ring_led, LOW);
  digitalWrite(little_led, LOW);

  delay(1000);

  // back to resting
  thumb_servo.write(rest_thumb);
  index_servo.write(rest_index);
  middle_servo.write(rest_middle);
  ring_servo.write(rest_ring);
  little_servo.write(rest_little);
  digitalWrite(thumb_led, HIGH);
  digitalWrite(index_led, HIGH);
  digitalWrite(middle_led, HIGH);
  digitalWrite(ring_led, HIGH);
  digitalWrite(little_led, HIGH);
}

void scissors(){
  //makes the 'scissors' gesture from rock paper scissors
  
  index_servo.write(max_index);
  middle_servo.write(max_middle);
  digitalWrite(index_led, LOW);
  digitalWrite(middle_led, LOW);

  delay(1000);

  // back to resting
  thumb_servo.write(rest_thumb);
  index_servo.write(rest_index);
  middle_servo.write(rest_middle);
  ring_servo.write(rest_ring);
  little_servo.write(rest_little);
  digitalWrite(index_led, HIGH);
  digitalWrite(middle_led, HIGH);
}

void hand_control(){
  //Reads values from Photodiodes and checks if each finger should extend (Demo mode)

  //Read values
  thumb_pd = analogRead(thumb_pd_pin);
  index_pd = analogRead(index_pd_pin);
  middle_pd = analogRead(middle_pd_pin);
  ring_pd = analogRead(ring_pd_pin);
  little_pd = analogRead(little_pd_pin);
  
  controlDigits(thumb_pd, thumb_led, max_thumb, min_thumb, mid_thumb, thumb_servo);
  controlDigits(index_pd, index_led, max_index, min_index, mid_index, index_servo);
  controlDigits(middle_pd, middle_led, max_middle, min_middle, mid_middle, middle_servo);
  controlDigits(ring_pd, ring_led, max_ring, min_ring, mid_ring, ring_servo);
  controlDigits(little_pd, little_led, max_little, min_little, mid_little, little_servo);
}

void hand_control_with_skull(){
  //Reads values from Photodiodes and checks if each finger should extend (Demo mode)

  //Read values
  thumb_pd = analogRead(thumb_pd_pin);
  index_pd = analogRead(index_pd_pin);
  middle_pd = analogRead(middle_pd_pin);
  ring_pd = analogRead(ring_pd_pin);
  little_pd = analogRead(little_pd_pin);
  
  controlDigits_withSkull(thumb_pd, thumb_led, max_thumb, min_thumb, mid_thumb, thumb_servo);
  controlDigits_withSkull(index_pd, index_led, max_index, min_index, mid_index, index_servo);
  controlDigits_withSkull(middle_pd, middle_led, max_middle, min_middle, mid_middle, middle_servo);
  controlDigits_withSkull(ring_pd, ring_led, max_ring, min_ring, mid_ring, ring_servo);
  controlDigits_withSkull(little_pd, little_led, max_little, min_little, mid_little, little_servo);
}

void movements(){
  //Playback mode movements
  one_at_a_time();
  delay(500);
  count();
  delay(500);
  rock_star(); 
}

void rock_paper_scissors(){
  //Rock paper scissors interactive mode!
  //Activated when = Switch 1 OFF & Switch 2 OFF
  //Hand randomly chooses one of rock paper or scissors when button pressed

  button_state = digitalRead(button); //Has button been pressed?
  int random_num = random(1,4);       //Rock = 1, Paper = 2, Scissors = 3

  // check if the pushbutton is pressed. If it is, the button_state is HIGH:
  if (button_state == HIGH) {
    // play Rock, Paper, Scissors once
    if(random_num == 1){
      //Rock!
      rock();
    }
    else if(random_num == 2){
      //Paper!
      paper();
    }
    else{
      //Scissors!
      scissors();
    }
  } else {
    //Do nothing
  }
}

void loop()
{
  //max_thumbin function, continuously loops through this

  //Monitor switches to check what mode it should be in
  switch1 = digitalRead(demo);
  switch2 = digitalRead(playback);

  if(switch1 == 1 && switch2 == 1){
    //Laser pointer demonstration mode WITH the skull
    //Switch 1 ON & Switch 2 ON
    hand_control_with_skull(); 
  }
  else if(switch1 == 1 && switch2 == 0){
    //Laser pointer demonstration mode without the skull
    //Switch 1 ON & Switch 2 OFF
    hand_control(); 
  }
  else if(switch1 == 0 && switch2 == 1){
    //Playback mode
    //Switch 1 OFF & Switch 2 ON
    movements();
  }
  else if(switch1 == 0 && switch2 == 0){
    //Rock Paper Scissors gamax_little mode
    //Switch 1 OFF & Switch 2 OFF
    //Further controlled with a button
    //Start in resting positoin
    thumb_servo.write(rest_thumb);
    index_servo.write(rest_index);
    middle_servo.write(rest_middle);
    ring_servo.write(rest_ring);
    little_servo.write(rest_little);
    rock_paper_scissors();
  }
  else{
    //Wait for mode to change
  }

}
