const int IN1 = 9 ;
const int IN2 = 8 ;
const int IN3 = 5 ;
const int IN4 = 4 ;
const int ENA = 10 ;
const int ENB = 12;
const int buttA = A0 ;
const int aIN = 6 ;
const int aCOMP = 7;
const int defA = 100 ;
volatile bool flag = false;
volatile bool Aflag ;
volatile bool Bflag ;
volatile bool Oflag ;
int button = 3;
const int baseSpeed = 200 ;
const int baseDelay = 1000;
const int baseInc = 1;

void increaseA(int i)
{
  int leftover ;
  if (!((OCR2A + i - 255 > 0) && (OCR2B - (OCR2A + i - 255) < 0)))
  {
    bool
    leftover = OCR2A + i - 255;
    if (leftover > 0)
    {
      OCR2A = 255;
      decreaseB(leftover) ;
      return ;
    }
    OCR2A += i ;
  }
  return;
}
void decreaseA(int i)
{
  int leftover;
  if (!((OCR2A - i < 0) && (OCR2B - (OCR2A - i) > 255)))
  {
    leftover = i - OCR2A ;
    if (leftover > 0);
    {
      OCR2A = leftover;
      reverseOnA() ;
      return;
    }
    OCR2A -= i ;
    return;
  }
}
void increaseB(int i)
{
  int leftover;
  if (!((OCR2B + i - 255 > 0) && (OCR2A - (OCR2B + i - 255) < 0)))
  {
    leftover = OCR2B + i - 255;
    if (leftover > 0)
    {
      OCR2B = 255;
      decreaseA(leftover) ;
      return;
    }
    OCR2B += i;
  }
  return;
}
void decreaseB(int i)
{
  int leftover;
  // first check to see that if its possible to decrease B any further
  if (!((OCR2B - i < 0) && (OCR2A - (OCR2B - i) > 255)))
  {
    leftover = i - OCR2B;
    if (leftover > 0)
    {
      OCR2B = leftover;
      reverseOnB();
      return;
    }
    OCR2B -= i ;
  }
  return;
}
void reverseOnB()
{
  digitalWrite(IN3, LOW) ;
  digitalWrite(IN4, HIGH) ;
}

void reverseOnA()
{
  digitalWrite(IN1, LOW) ;
  digitalWrite(IN2, HIGH) ;
}
void forwardOn(int speedValue) {
  digitalWrite(IN1, HIGH) ;
  digitalWrite(IN2, LOW) ;
  analogWrite(ENA, speedValue) ;
  digitalWrite(IN3, HIGH) ;
  digitalWrite(IN4, LOW) ;
  analogWrite(ENB, speedValue) ;
}
void setup()
{

  cli() ;
  //for the timers
  TIMSK2 |= B111 ;
  TCCR2B |= B111 ;
  TIFR2 |= B111 ;
  TCNT2 = 0;
  OCR2A = 0;
  OCR2B = 0;
  // for the pin change interrupt
  EIMSK |= 0x3 ; // enables int1 and int0 interrupts
  EICRA |= 0xF ; // sets to rising edge on interrupt
  //Set write modes for the pins (1 is output, 0 input)
  DDRD = B1111111;
  DDRC = B010;
  DDRB = B111111;
  // set motor to positive direction (forward)
  PORTB = B000010 ;
  PORTD = B00100000 ;
  // both of these being true means we can use ANALOG_COMP_vect
  ADCSRA |= (1 << 7) ; // enable ADEN
  ADCSRB |= (1 << 6);           // (Enable) ACME: Analog Comparator Multiplexer Enable
  ACSR |= 27 ;  // ACIS1, ACIS0: Analog Comparator Interrupt Mode Select (trigger on rising edge)
  Serial.begin(9600) ;
  sei() ;
}
ISR(INT0_vect)
{
  flag = true ;
}
ISR(TIMER2_COMPA_vect)
{
  PORTB &= ~(B000100) ;
Aflag = true ;
}
ISR(TIMER2_COMPB_vect)
{
  PORTB &= ~(B10000);
Bflag=true;
}
ISR(TIMER2_OVF_vect)
{
  if (OCR2A > 0)
    PORTB |= B000100 ;
  if (OCR2B >0)
  PORTB |= B10000;
  


}
void loop() {
if(Aflag = true)
{
  CP(button, baseInc) ;
delay(25);
}
Serial.println(OCR2A) ;
Serial.println(OCR2B);

}

void CP(int button, int i)
{
  bool maxAflag;
  bool maxBflag;
  int leftover ;
  bool tire = false;
  switch (button)
  {
    case 0:
      break;
    case 1:
      increaseB(i);
      break;
    case 2:
      if (OCR2B + i > 255)
        OCR2B = 255;
      else
        OCR2B += (i);
      if (OCR2A + i > 255)
        OCR2A = 255;
      else
        OCR2A += i;
      break;
    case 3:
      if (OCR2B + 2 * i > 255)
        OCR2B = 255;
      else
        OCR2B += (2 * i);
      if (OCR2A + i > 255)
        OCR2A = 255;
      else
        OCR2A += i;
      break;
    case 4:
      if (OCR2B - i < 0)
      {
        reverseOnB();
        OCR2B = -(OCR2B - i);
      }
      else
        OCR2B -= i;
      if (OCR2A - i < 0)
      {
        reverseOnA();
        OCR2A = -(OCR2A - i) ;
      }
      else
        OCR2A -= i;
      break;
    case 5:
      decreaseA(i);
      break;
    case 6:
      break;
    case 7:
      increaseB(i);
    case 8:
      increaseA(i) ;
      break;
    case 9:
      break;
    case 10:
      if (OCR2A + 2 * i > 255)
        OCR2A = 255;
      else
        OCR2A += (2 * i);
      if (OCR2B + i > 255)
        OCR2B = 255;
      else
        OCR2B += i;
      break;
    case 11:
      if (OCR2B + 2 * i > 255)
        OCR2B = 255;
      else
        OCR2B += (2 * i);
      if (OCR2A + 2 * i > 255)
        OCR2A = 255;
      else
        OCR2A += 2 * i;
      break;
    case 12:
      decreaseB(i);
    case 13:
      break;
    case 14:
      increaseA(i);
    case 15:
      PORTB &= ~(B000010) ;
      PORTD &= ~(B00100000);
      break;
  }
}
int find_buttons()
{
  if (flag == true)
  {
    EIMSK &= ~1 ;
    Serial.println("Button Press Detected") ;
    button = find_buttons() ;
    PORTB = button ;
    Serial.println(button) ;
    delay(20) ;
    int temp = analogRead(buttA) ;
    Serial.println(temp);
    if (temp < defA + 4)
    {
      flag = false ;
      EIMSK |= 1 ;
      return 0;

    }
    else if (temp < 803)
      return 8;
    else if (temp < 830)
      return 4 ;
    else if (temp < 865)
      return 2;
    else if (temp < 901)
      return 1 ;
    else if (temp < 910)
      return 12 ;
    else if (temp < 923)
      return 10;
    else if (temp < 929)
      return 6 ;
    else if (temp < 939)
      return 9 ;
    else if (temp < 943)
      return 5;
    else if (temp < 950)
      return 3 ;
    else if (temp < 954)
      return 14 ;
    else if (temp < 962)
      return 13;
    else if (temp < 966)
      return 11;
    else if (temp < 969)
      return 7;
    else
      return 15;
  }
}
// 0 = 0
//1 = 899-900//
// 2 = 864//
// 3 = 949//
// 4 = 829//
// 5 = 942//
// 6 = 928//
// 7 = 968//
// 8 = 802//
// 9 = 938//
//10 = 922//
// 11 = 965//
//12 = 909//
// 13 = 961//
// 14 = 953//
// 15 = 977//
