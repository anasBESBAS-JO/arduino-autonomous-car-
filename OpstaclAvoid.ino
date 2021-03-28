void readSidesAndFront() {      //Function called to determine if any short or
                                // 45 degree turns are needed. I made this a
                                // separate function since this is called three
                                // times in the loop, and the code
  distance4 = sonarFL();             //became unnecessarily long. It reads all
                                     // of the sensors except the front sensor
                                     // and determines whether or not any minor
                                     // corrections are necessary.
  Serial.print("Front Left: ");
  Serial.println(distance4);
  delay(3);
  distance5 = sonarFR();
  Serial.print("Front Right: ");
  Serial.println(distance5);
  delay(3);
  distance2 = sonarL();
  Serial.print("Left: ");
  Serial.println(distance2);
  delay(3);
  distance3 = sonarR();
  Serial.print("Right: ");
  Serial.println(distance3);
  if (distance3 < sideThreshold && distance3 != 0) {
    distance2 = sonarL();
    Serial.print("Left: ");
    Serial.println(distance2);
    if (distance3 < sideThreshold && distance3 != 0) {
      distance2 = sonarL();
      Serial.print("Left: ");
      Serial.println(distance2);
      if (distance3 < sideThreshold && distance3 != 0) {
        leftTurn();
        //checkFront();
      } else {
        motor.writeMicroseconds(forwardSpeed);
        steer.write(neutral);
      }
    } else {
      motor.writeMicroseconds(forwardSpeed);
      steer.write(neutral);
    }
  } else if (distance2 < sideThreshold && distance2 != 0) {
    distance3 = sonarR();
    Serial.print("Right: ");
    Serial.println(distance3);
    if (distance2 < sideThreshold && distance2 != 0) {
      distance3 = sonarR();
      Serial.print("Right: ");
      Serial.println(distance3);
      if (distance2 < sideThreshold && distance2 != 0) {
        rightTurn();
       // checkFront();
      } else {
        motor.writeMicroseconds(forwardSpeed);
        steer.write(neutral);
      }
    } else {
      motor.writeMicroseconds(forwardSpeed);
      steer.write(neutral);
    }
  } else if (distance5 <= 4 && distance5 != 0) {
    //stopReverseTurn();
    //checkFront();
    steer.write(steerRight);
    motor.writeMicroseconds(speedCenter);
    delay(1000);
    motor.writeMicroseconds(reverseSpeed);
    Serial.println("Reversing");
    delay(950);
    motor.writeMicroseconds(speedCenter);
    steer.write(neutral);
  } else if (distance5 < frontSideThreshold && distance5 != 0) {
    distance5 = sonarFR();
    Serial.print("Front Right: ");
    Serial.println(distance5);
    if (distance5 < frontSideThreshold && distance5 != 0) {
      distance5 = sonarFR();
      Serial.print("Front Right: ");
      Serial.println(distance5);
      if (distance5 < frontSideThreshold && distance5 != 0) {
        frontLeftTurn();
        checkFront();
      } else {
        motor.writeMicroseconds(forwardSpeed);
        steer.write(neutral);
      }
    } else {
      motor.writeMicroseconds(forwardSpeed);
      steer.write(neutral);
    }
  } else if (distance4 < 4 && distance4 != 0) {
    steer.write(steerLeft);
    breaking(); // stop the car perparing for a reverse
    delay(500);
    //stopReverseTurn();
    //checkFront();
    motor.writeMicroseconds(reverseSpeed);
    Serial.println("Reversing  ");
    delay(950);
    motor.writeMicroseconds(speedCenter);
    steer.write(neutral);
  } else if (distance4 < frontSideThreshold && distance4 != 0) {
    distance4 = sonarFL();
    Serial.print("Front Left: ");
    Serial.println(distance4);
    if (distance4 < frontSideThreshold && distance4 != 0) {
      distance4 = sonarFL();
      Serial.print("Front Left: ");
      Serial.println(distance4);
      if (distance4 < frontSideThreshold && distance4 != 0) {
        frontRightTurn();
        //checkFront();
      } else {
        motor.writeMicroseconds(forwardSpeed);
        steer.write(neutral);
      }
    } else {
      motor.writeMicroseconds(forwardSpeed);
      steer.write(neutral);
    }
  }
}

void opstaclAvoiding() {
  steer.write(neutral);
  motor.writeMicroseconds(forwardSpeed);
  distance = sonarFront();
  Serial.print("Forward Reading: ");    //Here is an example of the stacked
                                        // if/else statements I mentioned
                                        // before. I had issues with the car
                                        // randomly turning even when no object
                                        // was present.
  Serial.println(distance);             //This was due to noise in the sonar
                                        // readings. Here, the four if
                                        // statements within each other
                                        // essentially quadruple check to make
                                        // sure that an obstacle
  delay(8);                             //is present before turning. This has a
                                        // negligible impact on reading
                                        // performance, and I used a similar
                                        // structure for the readSidesAndFront()
                                        // function above.
  if (distance < frontThreshold && distance != 0) {
    distance = sonarFront();
    Serial.print("Forward Reading: ");
    Serial.println(distance);
    delay(8);
    if (distance < frontThreshold && distance != 0) {
      distance = sonarFront();
      Serial.print("Forward Reading: ");
      Serial.println(distance);
      delay(8);
      if (distance < frontThreshold && distance != 0) {
        distance = sonarFront();
        Serial.print("Forward Reading: ");
        Serial.println(distance);
        delay(8);
        if (distance > frontThreshold || distance == 0) {
          motor.writeMicroseconds(forwardSpeed);
          steer.write(neutral);
        } else {          //Here is the code for the passage of the quadruple
                          // check, indicating an obstacle. Below, the car reads
                          // the two sides and decides which one has more room.
                          // If one sensor
          distance3 = sonarR();   //returns an error (0), the car turns in that
                                  // direction due to errors being returned for
                                  // wide open spaces.
          distance2 = sonarL();
          if (distance2 > distance3 && distance2 != 0) {
            turnSpeedDirTime(forwardSpeed, steerLeft, 475);
          } else if (distance3 > distance2 && distance3 != 0) {
            turnSpeedDirTime(forwardSpeed, steerRight, 475);
          } else if (distance2 == 0) {
            turnSpeedDirTime(forwardSpeed, steerLeft, 475);
          } else if (distance3 == 0) {
            turnSpeedDirTime(forwardSpeed, steerRight, 475);
          }
          distance = sonarFront(); //Code for backing up. If the front sonar is
                                   // too close to the wall (less than 5cm), the
                                   // car will pause, then back up and carry on
                                   // with normal function.
          if (distance <= 5 && distance != 0) {
            motor.writeMicroseconds(speedCenter);
            delay(1000);
            motor.writeMicroseconds(reverseSpeed);
            Serial.println("Reversing");
            delay(950);
            motor.writeMicroseconds(speedCenter);
            steer.write(neutral);
          }
        }
      } else {
        readSidesAndFront();    //Here, if the multiple checks for a wall in
                                // front fail at any point, the car reads the
                                // rest of the sensors for minor adjustments. It
                                // calls the function mentioned above.
      }
    } else {
      readSidesAndFront();
    }
  } else {
    readSidesAndFront();
  }
}
