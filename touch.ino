/************************************************************
■参考 : No Device
	■Arduinoでタッチセンサ（静電容量ライブラリ）を使う方法、スイッチレスで外部入力
		https://voltechno.com/blog/post-20037/
		
		contents
			分銅
	
	■さわるだけでON/OFFできるタッチセンサーを作ろう
		https://www.mirai-kougaku.jp/laboratory/pages/200911.php
	
	■Arduinoを使ってタッチセンサー
		https://inter-arteq.com/2014/11/13/arduino%E3%82%92%E4%BD%BF%E3%81%A3%E3%81%A6%E3%82%BF%E3%83%83%E3%83%81%E3%82%BB%E3%83%B3%E3%82%B5%E3%83%BC/
		
	★【Arduino】抵抗１本で作る静電容量式のタッチセンサ
		https://nn-hokuson.hatenablog.com/entry/2017/03/22/200454
		
	■Arduino タッチセンサ
		https://kousaku-kousaku.blogspot.com/2008/10/arduino.html
	
	■CapSense (静電容量センサ)
		http://www.musashinodenpa.com/arduino/ref/index.php?f=1&pos=2165
		
	■タッチセンサをスイッチとして使うプログラム
		https://ehbtj.com/electronics/touch-sensor-as-switch/
		
	■[電子工作]Arduinoを使ったタッチセンサの作り方
		https://inter-arteq.com/2012/10/07/%e9%9b%bb%e5%ad%90%e5%b7%a5%e4%bd%9carduino%e3%82%92%e4%bd%bf%e3%81%a3%e3%81%9f%e3%82%bf%e3%83%83%e3%83%81%e3%82%bb%e3%83%b3%e3%82%b5%e3%81%ae%e4%bd%9c%e3%82%8a%e6%96%b9/
	
■参考 : Device
	■タッチセンサ（Grove）をArduinoで使ってみる（ついでにちょっと【Looking Glass】で遊ぶ）
		https://qiita.com/get_itchy_feet/items/86fdadca309a7ffe74a9
************************************************************/

/************************************************************
************************************************************/
// #define PLOT

/************************************************************
************************************************************/
void StateChart__TouchRelease();
void StateChart__OnOff();

/************************************************************
************************************************************/
enum STATE0{
	STATE0__RELEASE,
	STATE0__TOUCH,
};

enum STATE1{
	STATE1__OFF,
	STATE1__ON,
};

int t_prev = 0;
int t = 0;
int thresh_0 = 10;
int thresh_1 = 7;

boolean b_touch = false;
STATE0 State0 = STATE0__RELEASE;
STATE1 State1 = STATE1__OFF;

/************************************************************
************************************************************/

/******************************
******************************/
void setup()
{
	/********************
	********************/
	Serial.begin(9600);
	
	/********************
	********************/
	pinMode(8,OUTPUT);
	pinMode(9,INPUT);
	
	pinMode(13,OUTPUT);
	pinMode(12,OUTPUT);
}

/******************************
******************************/
void loop()
{ 
	/********************
	********************/
	t = 0;
	digitalWrite(8, HIGH);
	while (digitalRead(9)!=HIGH) t++;
	
	/********************
	放電するまで待つ
	********************/
	digitalWrite(8, LOW);  
	delay(1); // ms
	
	/********************
	Low pass Filter
	********************/
	t = 0.8 * t_prev + 0.2 * t;
	t_prev = t;
	
#ifdef PLOT
	Serial.print(millis() / 1000 % 2 * 100);
	Serial.print(",");
	Serial.println(t);
#endif
	
	/********************
	StateChart
	********************/
	StateChart__TouchRelease();
	StateChart__OnOff();
	
	/********************
	Led
	********************/
	if(State0 == STATE0__TOUCH){
		digitalWrite(13, HIGH);
	}else{
		digitalWrite(13, LOW);
	}
	
	if(State1 == STATE1__ON){
		digitalWrite(12, HIGH);
	}else{
		digitalWrite(12, LOW);
	}
	
	/********************
	********************/
#ifdef PLOT
	delay(10);
#else
	delay(1);
#endif
}

/******************************
******************************/
void StateChart__TouchRelease()
{
	switch(State0){
		case STATE0__RELEASE:
			if(thresh_0 < t){
				b_touch = true;
				State0 = STATE0__TOUCH;
			}
			break;
			
		case STATE0__TOUCH:
			if(t <thresh_1){
				State0 = STATE0__RELEASE;
			}
			break;
	}
}

/******************************
******************************/
void StateChart__OnOff()
{
	switch(State1){
		case STATE1__OFF:
			if(b_touch){
				State1 = STATE1__ON;
				b_touch = false;
			}
			break;
		case STATE1__ON:
			if(b_touch){
				State1 = STATE1__OFF;
				b_touch = false;
			}
			break;
	}
}
