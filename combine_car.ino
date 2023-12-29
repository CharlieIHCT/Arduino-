//多合一多功能小車

//******************************
#include <IRremote.h>  
#include <Servo.h>
//***********************馬達腳位*************************
int MotorRight2=6; //右往後
int MotorRight1=5; //右往前
int MotorLeft2=11; //左往後
int MotorLeft1=10; //左往前
int counter=0;
const int irReceiverPin = 8;//紅外線接收器 OUTPUT 訊號接Pin 8

char val;  
//***********************IRcode*************************
long IRfront=     0x00FF18E7;      //前進碼　　
long IRback=      0x00FF4AB5;      //後退
long IRturnright= 0x00FF5AA5;      //右轉
long IRturnleft=  0x00FF10EF;      //左轉
long IRstop=      0x00FF38C7;      //停止
long IRcny70=     0x00FF6897;      //CNY70自走模式　　　＊鍵　　　
long IRAutorun=   0x00FFB04F;      //超音波自走模式　　 ＃鍵　
long IRturnsmallleft= 0x00FF22DD; 

//***********************CNY70尋線腳位*************************
const int SensorRight = 7;     //左感測器輸入腳
const int SensorMiddle= 4;  //中感測器輸入腳
const int SensorLeft = 3;    //右感測器輸入腳
int SL;    //左感測器狀態
int SM;    //中感測器狀態
int SR;    //右感測器狀態
IRrecv irrecv(irReceiverPin);  // 定義 IRrecv 物件來接收紅外線訊號
decode_results results;       // 解碼結果將放在 decode_results 結構的 result 變數裏
//*************************超音波腳位******************************
int inputPin =12; // 定義超音波信號接收 Rx
int outputPin =13;// 定義超音波信號發射 Tx
int Fspeedd = 0; // 前方距離
int Rspeedd = 0; // 右方距離
int Lspeedd = 0; // 左方距離
int directionn = 0; // 前=8 後=2 左=4 右=6 　
Servo myservo; // 設 myservo
int delay_time = 1000; // 伺服馬達轉向後的穩定時間
int Fgo = 8; // 前進
int Rgo = 6; // 右轉
int Lgo = 4; // 左轉
int Bgo = 2; // 倒車
//****************************模式變更(紅綠燈)*****************************************
int GreenLight = 14;// 腳位 A0 
int YellowLight = 15;  // 腳位 A1
int RedLight = 16; // 腳位 A2 
int button = 17; // 腳位 2 
int mode = 0; //0:普通模式 1:超聲波模式 2:循跡模式
//***************************(SETUP)*****************************************
void setup()
{  
  Serial.begin(9600);
  pinMode(MotorRight2, OUTPUT);  // 腳位 6 (PWM)
  pinMode(MotorRight1, OUTPUT);  // 腳位 5 (PWM)                                                                                                                                                                
  pinMode(MotorLeft2,  OUTPUT);  // 腳位 10 (PWM)
  pinMode(MotorLeft1,  OUTPUT);  // 腳位 11 (PWM)
  irrecv.enableIRIn();     // 啟動紅外線解碼
  pinMode(SensorLeft, INPUT); //定義左感測器
  pinMode(SensorMiddle, INPUT);//定義中感測器
  pinMode(SensorRight, INPUT); //定義右感測器
  pinMode(inputPin, INPUT); // 定義超音波輸入腳位
  pinMode(outputPin, OUTPUT); // 定義超音波輸出腳位 
  myservo.attach(9); // 定義伺服馬達輸出第9腳位(PWM)
  pinMode(GreenLight, OUTPUT);  // 綠燈
  pinMode(YellowLight, OUTPUT);  // 黃燈
  pinMode(RedLight, OUTPUT);  // 紅燈
  pinMode(SensorRight, INPUT); //定義右感測器
  pinMode(inputPin, INPUT); // 定義超音波輸入腳位
  pinMode(outputPin, OUTPUT); // 定義超音波輸出腳位 
 }
//**************************(Light)*******************************************
void green_on()
{
  digitalWrite(GreenLight,HIGH);
  digitalWrite(YellowLight,LOW); 
  digitalWrite(RedLight,LOW);
}

void yellow_on()
{
  digitalWrite(GreenLight,LOW);
  digitalWrite(YellowLight,HIGH); 
  digitalWrite(RedLight,LOW);
}

void red_on()
{
  digitalWrite(GreenLight,LOW);
  digitalWrite(YellowLight,LOW); 
  digitalWrite(RedLight,HIGH);
}
//**************************(Move)*******************************************
void advance(int a) // 前進
{
        digitalWrite(MotorRight2,LOW);
        digitalWrite(MotorRight1,HIGH); //右輪順轉
        digitalWrite(MotorLeft2,LOW);
        digitalWrite(MotorLeft1,HIGH); //左輪順轉
        delay(a * 100); 
}
void right(int b) //右轉(單輪)
{
       digitalWrite(MotorRight2,LOW);
       digitalWrite(MotorRight1,LOW);
       digitalWrite(MotorLeft2,LOW);
       digitalWrite(MotorLeft1,HIGH); //左輪順轉
       delay(b * 100);
}
void left(int c) //左轉(單輪)
{
      digitalWrite(MotorRight2,LOW);
      digitalWrite(MotorRight1,HIGH); //右輪順轉
      digitalWrite(MotorLeft2,LOW);
      digitalWrite(MotorLeft1,LOW);
      delay(c * 100);
}
void turnR(int d) //右迴轉
{
      digitalWrite(MotorRight2,HIGH); //右輪逆轉
      digitalWrite(MotorRight1,LOW);
      digitalWrite(MotorLeft2,LOW);
      digitalWrite(MotorLeft1,HIGH); //左輪順轉
      delay(d * 100);
}
void turnL(int e) //左迴轉
{
      digitalWrite(MotorRight2,LOW);
      digitalWrite(MotorRight1,HIGH); //右輪順轉
      digitalWrite(MotorLeft2,HIGH); //左輪逆轉
      digitalWrite(MotorLeft1,LOW);
      delay(e * 100);
    } 
void stopp(int f) //停止
{
     digitalWrite(MotorRight2,LOW);
     digitalWrite(MotorRight1,LOW);
     digitalWrite(MotorLeft2,LOW);
     digitalWrite(MotorLeft1,LOW);
     delay(f * 100);
}
void back(int g) //後退
{
        digitalWrite(MotorRight2,HIGH); //右輪逆轉
        digitalWrite(MotorRight1,LOW);
        digitalWrite(MotorLeft2,HIGH); //左輪逆轉
        digitalWrite(MotorLeft1,LOW);;
        delay(g * 100); 
}
//*********************************************************************************
void ask_pin_F() // 量出前方距離 
{
myservo.write(90);
digitalWrite(outputPin, LOW); // 讓超聲波發射低電壓2μs
delayMicroseconds(2);
digitalWrite(outputPin, HIGH); // 讓超聲波發射高電壓10μs，這裡至少是10μs
delayMicroseconds(10);
digitalWrite(outputPin, LOW); // 維持超聲波發射低電壓
float Fdistance = pulseIn(inputPin, HIGH); // 讀差相差時間
Fdistance= Fdistance/5.8/10; // 將時間轉為距離距离（單位：公分）
Serial.print("F distance:"); //輸出距離（單位：公分）
Serial.println(Fdistance); //顯示距離
Fspeedd = Fdistance; // 將距離 讀入Fspeedd(前距)
} 
//********************************************************************************
void ask_pin_L() // 量出左邊距離 
{
myservo.write(135);
delay(delay_time);
digitalWrite(outputPin, LOW); // 讓超聲波發射低電壓2μs
delayMicroseconds(2);
digitalWrite(outputPin, HIGH); // 讓超聲波發射高電壓10μs，這裡至少是10μs
delayMicroseconds(10);
digitalWrite(outputPin, LOW); // 維持超聲波發射低電壓
float Ldistance = pulseIn(inputPin, HIGH); // 讀差相差時間
Ldistance= Ldistance/5.8/10; // 將時間轉為距離距离（單位：公分）
Serial.print("L distance:"); //輸出距離（單位：公分）
Serial.println(Ldistance); //顯示距離
Lspeedd = Ldistance; // 將距離 讀入Lspeedd(左距)
} 
//******************************************************************************
void ask_pin_R() // 量出右邊距離 
{
myservo.write(5);
delay(delay_time);
digitalWrite(outputPin, LOW); // 讓超聲波發射低電壓2μs
delayMicroseconds(2);
digitalWrite(outputPin, HIGH); // 讓超聲波發射高電壓10μs，這裡至少是10μs
delayMicroseconds(10);
digitalWrite(outputPin, LOW); // 維持超聲波發射低電壓
float Rdistance = pulseIn(inputPin, HIGH); // 讀差相差時間
Rdistance= Rdistance/5.8/10; // 將時間轉為距離距离（單位：公分）
Serial.print("R distance:"); //輸出距離（單位：公分）
Serial.println(Rdistance); //顯示距離
Rspeedd = Rdistance; // 將距離 讀入Rspeedd(右距)
} 
void detection() //測量3個角度(前.左.右)
{ 
    int delay_time = 250; // 伺服馬達轉向後的穩定時間
    ask_pin_F(); // 讀取前方距離

    if(Fspeedd < 10) // 假如前方距離小於10公分
   {
      stopp(1); // 清除輸出資料 
      back(2); // 後退 0.2秒
   }
    if(Fspeedd < 25) // 假如前方距離小於25公分
   {
      stopp(1); // 清除輸出資料 
      ask_pin_L(); // 讀取左方距離
      delay(delay_time); // 等待伺服馬達穩定
      ask_pin_R(); // 讀取右方距離 
      delay(delay_time); // 等待伺服馬達穩定 

      if(Lspeedd > Rspeedd) //假如 左邊距離大於右邊距離
     {
        directionn = Lgo; //向左走
     }

      if(Lspeedd <= Rspeedd) //假如 左邊距離小於或等於右邊距離
      {
        directionn = Rgo; //向右走
      } 

      if (Lspeedd < 10 && Rspeedd < 10) //假如 左邊距離和右邊距離皆小於10公分
     {
        directionn = Bgo; //向後走 
      } 
    }
    else //假如前方大於25公分 
   {
      directionn = Fgo; //向前走 
   }
}   
//******************************************************************************(LOOP)
void loop() 
{
  performCommand();
  if (mode == 0)
  {
    green_on();
    while(mode == 0)
    {     
      performCommand();
      //***************************************************************************正常遙控模式      
      if (irrecv.decode(&results)) 
        {         // 解碼成功，收到一組紅外線訊號
          if (results.value == IRfront)//前進
          {
            advance(10);//前進
          }

          if (results.value ==  IRback)//後退
          {
            back(10);//後退
          }

          if (results.value == IRturnright)//右轉
          {
            right(5); // 右轉
          }

          if (results.value == IRturnleft)//左轉
          {
            left(5); // 左轉);
          }

          if (results.value == IRstop)//停止
          {
            digitalWrite(MotorRight2,LOW);
            digitalWrite(MotorRight1,LOW);
            digitalWrite(MotorLeft2,LOW);
            digitalWrite(MotorLeft1,LOW);
          }

          if (results.value == IRcny70)
          {                     
            mode = 2;
          }

          if (results.value == IRAutorun)
          {
            mode = 1;
          }
          
          else
          {
            digitalWrite(MotorRight2,LOW);
            digitalWrite(MotorRight1,LOW);
            digitalWrite(MotorLeft2,LOW);
            digitalWrite(MotorLeft1,LOW);
          }
          
          irrecv.resume();    // 繼續收下一組紅外線訊號        
      }  
    /**********************************************************************/
      if(digitalRead(button) == HIGH)
      {
        if(mode == 0)
        {
          mode = 1;
          yellow_on();
        }
        else if (mode == 1)
        {
          mode = 2;
          red_on();
        }
        else if (mode = 2)
        {
          mode = 0;
          green_on();
        }
        Serial.print("mode:");
        Serial.println(mode);
      }
      delay(100);
      while(!(digitalRead(button) == LOW))
      {
        // 空迴圈直到按鈕放開
      }
      break;
    }
  }
  else if(mode == 1) // 超聲波
  {
    yellow_on();
    while(mode == 1)
        {
          performCommand();
          myservo.write(90); //讓伺服馬達回歸 預備位置 準備下一次的測量
          detection(); //測量角度 並且判斷要往哪一方向移動
          Serial.print("direction:");
          Serial.println(directionn);
          if(directionn == Fgo) //假如directionn(方向) = 8(前進) 
          { 
            if (irrecv.decode(&results))
            {
              irrecv.resume(); 
              Serial.println(results.value,HEX);
              if(results.value ==IRstop)
              { 
                digitalWrite(MotorRight2,LOW); 
                digitalWrite(MotorRight1,LOW);
                digitalWrite(MotorLeft2,LOW);
                digitalWrite(MotorLeft1,LOW);
                break;
              }
            }
          if(digitalRead(button) == HIGH)
          {
            if(mode == 0)
            {
              mode = 1;
              yellow_on();

            }
            else if (mode == 1)
            {
              mode = 2;
              red_on();
            }
            else if (mode == 2)
            {
              mode = 0;
              green_on();
            }
            Serial.print("mode:");
            Serial.println(mode);
            delay(100);
            while(!(digitalRead(button) == LOW))
            {
              // 空迴圈直到按鈕放開
            }
            break;
          }
              results.value=0;
              advance(1); // 正常前進 
              Serial.print(" Advance "); //顯示方向(前進)
              Serial.print(" "); 
          }
          if(directionn == Bgo) //假如directionn(方向) = 2(倒車) 
        {
          if (irrecv.decode(&results))
          {
            irrecv.resume(); 
            Serial.println(results.value,HEX);
            if(results.value ==IRstop)
            { 
              digitalWrite(MotorRight2,LOW); 
              digitalWrite(MotorRight1,LOW);
              digitalWrite(MotorLeft2,LOW);
              digitalWrite(MotorLeft1,LOW);
              break;
            }
          }
          if(digitalRead(button) == HIGH)
          {
            if(mode == 0)
            {
              mode = 1;
              yellow_on();

            }
            else if (mode == 1)
            {
              mode = 2;
              red_on();
            }
            else if (mode == 2)
            {
              mode = 0;
              green_on();
            }
            Serial.print("mode:");
            Serial.println(mode);
            delay(100);
            while(!(digitalRead(button) == LOW))
            {
              // 空迴圈直到按鈕放開
            }
            break;
          }
            results.value=0;
            back(4); // 倒退(車)
            left(1); //些微向左方移動(防止卡在死巷裡)
            Serial.println(" Reverse "); //顯示方向(倒退)
        }
          if(directionn == Rgo) //假如directionn(方向) = 6(右轉) 
        {
          if (irrecv.decode(&results))
          {
            irrecv.resume(); 
            Serial.println(results.value,HEX);
            if(results.value ==IRstop)
            { 
              digitalWrite(MotorRight2,LOW); 
              digitalWrite(MotorRight1,LOW);
              digitalWrite(MotorLeft2,LOW);
              digitalWrite(MotorLeft1,LOW); //
              break;
            }
          }
          if(digitalRead(button) == HIGH)
          {
            if(mode == 0)
            {
              mode = 1;
              yellow_on();

            }
            else if (mode == 1)
            {
              mode = 2;
              red_on();
            }
            else if (mode == 2)
            {
              mode = 0;
              green_on();
            }
            Serial.print("mode:");
            Serial.println(mode);
            delay(100);
            while(!(digitalRead(button) == LOW))
            {
              // 空迴圈直到按鈕放開
            }
            break;
          }
            results.value=0;
              back(2); 
              right(3); // 右轉
              Serial.print(" Right "); //顯示方向(右轉)
        }
          if(directionn == Lgo) //假如directionn(方向) = 4(左轉) 
          { 
            if (irrecv.decode(&results))
            {
              irrecv.resume(); 
              Serial.println(results.value,HEX);
              if(results.value ==IRstop)
              { 
                digitalWrite(MotorRight2,LOW); 
                digitalWrite(MotorRight1,LOW); //
                digitalWrite(MotorLeft2,LOW);
                digitalWrite(MotorLeft1,LOW);
                break;
              }
            }
            if(digitalRead(button) == HIGH)
            {
              if(mode == 0)
              {
                mode = 1;
                yellow_on();

              }
              else if (mode == 1)
              {
                mode = 2;
                red_on();
              }
              else if (mode == 2)
              {
                mode = 0;
                green_on();
              }
              Serial.print("mode:");
              Serial.println(mode);
              delay(100);
              while(!(digitalRead(button) == LOW))
              {
                // 空迴圈直到按鈕放開
              }
              break;
            }
              results.value=0;
              back(2); 
              left(3); // 左轉
              Serial.print(" Left "); //顯示方向(左轉) 
          } 
          
        if (irrecv.decode(&results))
          {
            irrecv.resume(); 
            Serial.println(results.value,HEX);
            if(results.value ==IRstop)
            { 
              digitalWrite(MotorRight2,LOW); 
              digitalWrite(MotorRight1,LOW);
              digitalWrite(MotorLeft2,LOW);
              digitalWrite(MotorLeft1,LOW);
              mode = 0;
              break;
            }
          }
        if(digitalRead(button) == HIGH)
          {
            if(mode == 0)
            {
              mode = 1;
              yellow_on();

            }
            else if (mode == 1)
            {
              mode = 2;
              red_on();
            }
            else if (mode = 2)
            {
              mode = 0;
              green_on();
            }
            Serial.print("mode:");
            Serial.println(mode);
            delay(100);
            while(!(digitalRead(button) == LOW))
            {
              // 空迴圈直到按鈕放開
            }
            break;
          }
        }
      results.value=0;
  }
  else if(mode == 2) //循跡
  {
    red_on();
    while(mode == 2)
     {  
       performCommand();
       SR = digitalRead(SensorRight);
       SM = digitalRead(SensorMiddle);
       SL = digitalRead(SensorLeft);
                   
         if (SM == HIGH)//中感測器在白色區域
       {
          if (SL == HIGH && SR == LOW) // 左白右黑, 向左轉彎
          {  
             digitalWrite(MotorRight2,LOW);
             digitalWrite(MotorRight1,HIGH);
             analogWrite(MotorLeft2,20);
             analogWrite(MotorLeft1,0);
          }
          else if (SR == HIGH && SL == LOW) //左黑右白, 向右轉彎
          {  
             analogWrite(MotorRight2,20);//右轉
             analogWrite(MotorRight1,0);
             digitalWrite(MotorLeft2,LOW);
             digitalWrite(MotorLeft1,HIGH);
          }
         else  // 兩側均為白色, 直進
          {
             digitalWrite(MotorRight1,HIGH);
             digitalWrite(MotorRight2,LOW);
             digitalWrite(MotorLeft1,HIGH);
             digitalWrite(MotorLeft2,LOW);
         }      
       }
       else // 中感測器在黑色區域
      {  
         if (SL == HIGH && SR == LOW)// 左白右黑, 快速左轉
        {  
            digitalWrite(MotorRight2,LOW);
            digitalWrite(MotorRight1,HIGH);
            digitalWrite(MotorLeft2,LOW);
            digitalWrite(MotorLeft1,LOW);
        }
         else if (SR == HIGH && SL == LOW) // 左黑右白, 快速右轉
        {  
           digitalWrite(MotorRight2,LOW);
           digitalWrite(MotorRight1,LOW);
           digitalWrite(MotorLeft2,LOW);
           digitalWrite(MotorLeft1,HIGH);
        }
         else // 都是黑色, 停止
        {   
        digitalWrite(MotorRight1,LOW);
        digitalWrite(MotorRight2,LOW);
        digitalWrite(MotorLeft1,LOW);
        digitalWrite(MotorLeft2,LOW);
        }
      }
       if (irrecv.decode(&results))
       {
             irrecv.resume();
                  Serial.println(results.value,HEX);
             if(results.value ==IRstop)
             {
               digitalWrite(MotorRight2,HIGH);
               digitalWrite(MotorRight1,HIGH);
               digitalWrite(MotorLeft2,HIGH);
               digitalWrite(MotorLeft1,HIGH);
               break;
             }
       }
       if(digitalRead(button) == HIGH)
      {
        if(mode == 0)
        {
          mode = 1;
          yellow_on();

        }
        else if (mode == 1)
        {
          mode = 2;
          red_on();
        }
        else if (mode = 2)
        {
          mode = 0;
          green_on();
        }
        Serial.print("mode:");
        Serial.println(mode);
        delay(100);
        while(!(digitalRead(button) == LOW))
        {
          // 空迴圈直到按鈕放開
        }
        break;
      }
     }
      results.value=0;
   }


}
   
void performCommand() {
  if (Serial.available()) {
    val = Serial.read();
  }
    if (val == 'f') { // Forward
      advance(1);
    } else if (val == 'z') { // Stop Forward     
      stopp(1) ;
    } else if (val == 'b') { // Backward
      back(1); 
    } else if (val == 'y') { // Stop Backward
      back(1); 
    } else if (val == 'r') { // Right
      turnR(1);
    } else if (val == 'l') { // Left
      turnL(1);
    } else if (val == 'v') { // Stop Turn
      stopp(1) ;
    } else if (val == 's') { // Stop
      stopp(1) ;
    } else if (val == '0') { // Stop
      mode = 0 ;
    } else if (val == '1') { // Stop
      mode = 1 ;
    } else if (val == '2') { // Stop
      mode = 2 ;
    } 
  
}
