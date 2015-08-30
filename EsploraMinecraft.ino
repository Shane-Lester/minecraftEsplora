
/*
  Esplora Minecraft-

 Based on EsploraKart 
  by Enrico Gueli <enrico.gueli@gmail.com>
  
  Editted with other fragments from examples by
  Shane Lester 30th August 2015
*/


#include <Esplora.h>

/*
  You're going to handle eight different buttons. You'll use arrays,
  which are ordered lists of variables with a fixed size. Each array
  has an index (counting from 0) to keep track of the position
  you're reading in the array, and each position can contain a number.
  
  I have split the arrays- the first one will handle 6 keypresses. The last 2 buttons
  are separate as they handle mouse button presses

  This code uses four different arrays: one for the buttons you'll read;
  a second to hold the current states of those buttons; and a third to hold
  the keystrokes associated with each button, the fourth is the slider value,
  mapped to a value of 0-8 and the array selects the character for keypress-
  I unsucessfully tried casting to Char.
 */

/*
  This array holds the last sensed state of each of the buttons
  you're reading.
  Later in the code, you'll read the button states, and compare them
  to the previous states that are stored in this array. If the two
  states are different, it means that the button was either
  pressed or released.
 */
boolean buttonStates[8];
//use for buttons to simulate mouse buttons (switch 2 and 4)
int mouseButton;
int sliderValue;
int value=1;
/*
  This array holds the names of the buttons being read.
  Later in the sketch, you'll use these names with
  the method Esplora.readButton(x), where x
  is one of these buttons.
 */
const byte buttons[] = {
  JOYSTICK_DOWN,
  JOYSTICK_LEFT,
  JOYSTICK_UP,
  JOYSTICK_RIGHT,
  SWITCH_UP, // inventory
  SWITCH_DOWN, // jump
  SWITCH_RIGHT, // right mouse button
  SWITCH_LEFT // left mouse button
};

/*
  This array tells what keystroke to send to the PC when a
  button is pressed.
  If you look at this array and the above one, you can see that
  the "cursor down" keystroke is sent when the joystick is moved
  down, the "cursor up" keystroke when the joystick is moved up
  and so on.
*/
const char keystrokes[] = {
  's',
  'a',
  'w',
  'd',
  'e',
  ' ',
  '1',
  '0'
};
/*
This array holds the characters that correspond to the slider 
values

*/
const char inventory[] ={
  '1',
  '2',
  '3',
  '4',
  '5',
  '6',
  '7',
  '8',
  '9'
  };


/*
  This is code is run only at startup, to initialize the
  virtual USB keyboard.
*/
void setup() {
  Keyboard.begin();
  Mouse.begin();            // take control of the mouse
}

/*
  After setup() is finished, this code is run continuously.
  Here we continuously check if something happened with the
  buttons.W
*/
void loop() {
  
  /*
  Read the slider- it's reversed so map it to 0 to 8
  This value corresponds to the inventory array char.
  Only send the keypress when you move the slider.
  If it's stable the ensure the key is released
  */
  int value = map(Esplora.readSlider(),1023,0,0,8);
  if (value != sliderValue){
    Keyboard.press(inventory[value]);
    sliderValue = value;
    }
   else{
    Keyboard.release(inventory[value]);
   }
    
  
  
  int yAxis = Esplora.readAccelerometer(Y_AXIS);    // read the Y axis- from accelerometer demo code
  int mouseY = map(-yAxis, -200, 200, -30, 30);  // map the Y value to a range of movement for the mouse Y
  if (abs(mouseY)<10){
    mouseY =0;
  }
  
  int xAxis = Esplora.readAccelerometer(X_AXIS);    // read the X axis
  int mouseX = map(-xAxis, -200, 200, -30, 30);  // map the X value to a range of movement for the mouse Y
 /*
 Create a neutral area otherwise it's impossible to keep your head stil
 Change the value below if you want less of a neutral area
 */
 
  if (abs(mouseX)<10){
    mouseX =0;
  }
  
    Mouse.move(mouseX, mouseY, 0);                 // move the mouse- move Head position
    
   int button = Esplora.readJoystickSwitch();   // read the joystick pushbutton

    if (button == 0) {                           // if the Wjoystick button is pressed
    Keyboard.press(KEY_LEFT_CTRL);                             // send a left ctrl- sprint
  } else {
    Keyboard.release(KEY_LEFT_CTRL);                            // if it's not pressed, stop sprinting 
  }

     
  // Iterate through  the first 6  buttons- joystick and 2 switches:
  for (byte thisButton = 0; thisButton < 6; thisButton++) {
    boolean lastState = buttonStates[thisButton];
    boolean newState = Esplora.readButton(buttons[thisButton]);
    if (lastState != newState) { // Something changed!
      /*
        The Keyboard library allows you to "press" and "release" the
        keys as two distinct actions. These actions can be
        linked to the buttons we're handling.
       */
      if (newState == PRESSED) {
        Keyboard.press(keystrokes[thisButton]);
      }
      else if (newState == RELEASED) {
        Keyboard.release(keystrokes[thisButton]);
      }
    }
    // Store the new button state, so you can sense a difference later:
    buttonStates[thisButton] = newState;   
  }
  

// Iterate through last 2 the buttons- mouse button presses:
  for (byte thisButton = 6; thisButton < 8; thisButton++) {
    boolean lastState = buttonStates[thisButton];
    boolean newState = Esplora.readButton(buttons[thisButton]);
    if (lastState != newState) { // Something changed!
      /*
        The Keyboard library allows you to "press" and "release" the
        keys as two distinct actions. These actions can be
        linked to the buttons we're handling.
        If refers to the array
       */
       if (thisButton ==7){
        mouseButton = MOUSE_LEFT;
       }
       else{
        mouseButton = MOUSE_RIGHT;
       }
       
       if (newState == PRESSED) {
              Mouse.press(mouseButton);
             }
       else if (newState == RELEASED) {
              Mouse.release(mouseButton);
                       }
          }
   
    
    // Store the new button state, so you can sense a difference later:
    buttonStates[thisButton] = newState;   
  }
  /*
    Wait a little bit (50ms) between a check and another.
    When a mechanical switch is pressed or released, the
    contacts may bounce very rapidly. If the check is done too
    fast, these bounces may be confused as multiple presses and
    may lead to unexpected behaviour.
   */
  delay(50);
}

