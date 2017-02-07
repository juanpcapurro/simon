#include "Arduino.h"

#define BUZZER_PIN 11
#define PRIMER_BOTON 14
#define ULTIMO_BOTON 17
#define CANTIDAD_PINES 4

//#define PRIMER_LED 2
//#define ULTIMO_LED 5

class LED_output{
	protected:
		size_t PRIMER_LED;
		size_t ULTIMO_LED;
		size_t CANTIDAD_LED;
		char states[10];

	public:
		LED_output(size_t primerLed, size_t ultimoLed){
			PRIMER_LED = primerLed;
			ULTIMO_LED = ultimoLed;
			CANTIDAD_LED = ultimoLed - primerLed +1;
			for (size_t i = 0; i< CANTIDAD_LED; i++){
				pinMode(i+primerLed, OUTPUT);
				states[i]=LOW;
			}
		}
		void ledOn(int ledNumber){
			states[ledNumber]=HIGH;
			digitalWrite(ledNumber+PRIMER_LED, HIGH);
		}
		void ledOff(int ledNumber){
			states[ledNumber]=LOW;
			digitalWrite(ledNumber+PRIMER_LED, LOW);
		}
		void ledToggle(int ledNumber){
			if(states[ledNumber]==LOW)
				ledOn(ledNumber);
			else
				ledOff(ledNumber);
		}
		char getLedState(int ledNumber){
			return states[ledNumber];
		}
		size_t size(){
			return CANTIDAD_LED;
		}
};

class Botonera{
	protected:
		const static int ARRAY_LENGHT=10;
		const static int DELAY_TIME= 40;
		size_t primerBoton;
		size_t ultimoBoton;
		size_t cantidad_botones;
		char ultimoEstado[ARRAY_LENGHT];
		size_t tiempo_ultimo_estado[ARRAY_LENGHT];
	public:
		Botonera(size_t primer_pin, size_t ultimo_pin){
			primerBoton= primer_pin;
			ultimoBoton= ultimo_pin;
			cantidad_botones = ultimo_pin - primer_pin+1;
			for(size_t i = 0; i< cantidad_botones; i++){
				pinMode(i+primerBoton, INPUT_PULLUP);
				ultimoEstado[i]=HIGH;
				tiempo_ultimo_estado[i]=millis();
			}
		}
		void update(){
			for(size_t i = 0; i< cantidad_botones; i++){
				char estadoActual = digitalRead(i+primerBoton);
				size_t ahora = millis();
				if(estadoActual != ultimoEstado[i])
					if(ahora - tiempo_ultimo_estado[i] > DELAY_TIME){
						tiempo_ultimo_estado[i]= ahora;
						ultimoEstado[i]= estadoActual;
					}
			}
		}
		char getEstado(size_t queBoton){
			return ultimoEstado[queBoton];
		}

};

class Contador: public LED_output{
	private:
		size_t valor;
	public:
		Contador(size_t primer_pin, size_t ultimo_pin):LED_output(primer_pin, ultimo_pin){
 			valor=0;
		}
	void setValue(size_t value){
		valor = value;
	}
	void display(){//muestra el valor actual, no lo borra
		flush();
		for(size_t i=0; (size_t)(1<<i) <=valor && i<CANTIDAD_LED;i++){
			size_t potencia = 1<<i;
			if(valor & potencia){
				ledOn(i);
			}
		}
	}
	void flush(){//apaga todos los LEDs
		for(size_t i=0;i<CANTIDAD_LED;i++)
			ledOff(i);
	}

};

void setup(){
	Botonera botonera(14,17);
	Contador contador(2,5);

	size_t unNumero=0;
	Serial.begin(9600);

	while(1){

		if(millis()%1000==0 ){
			unNumero++;
			contador.setValue(unNumero);
			Serial.println(unNumero);
			contador.display();
			delay(10);
		}
	}
}

void loop(){

}
