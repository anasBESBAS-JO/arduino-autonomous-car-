void speedFMax(){
  for (int i = 1380; i > 1250; --i) { 
    motor.writeMicroseconds(i);
    delay(30);
  }
}


void speedFMin(){
  motor.writeMicroseconds(1380);
}


void slowDownSpeed(){
  for (int i = 1250; i < 1380; ++i) { 
    motor.writeMicroseconds(i);
    delay(30);
  }
}


void breaking(){ // Function called to stop the car 
  digitalWrite(LEDB,HIGH);
  motor.writeMicroseconds(reverseSpeed);
  delay(500);
  speedCenterN(); 
  Serial.println("Breaking done ! ");
}


void speedBMax(){
  motor.writeMicroseconds(1630);
}


void speedBMin(){
  for (int i = 1550; i < 1630; ++i) {
    motor.writeMicroseconds(i);
    delay(30);
  }
}


void speedCenterN(){
  motor.writeMicroseconds(speedCenter);
}
