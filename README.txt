# Volante

 Autor : Santiago Barrenechea
 Descripción : Contara con un arduino UNO que por medio de sensores mida la velocida, voltaje y amperaje, este los mande por bt a un telefono con una aplicacion

Pin out :

	Vel
           	* 2  Sensor del velocimetro (Agregar a este pin una res pull down 10 k y un capacitor ceramico en paralelo al sensor)
	 	* Poner el diametro de la rueda en metros

	Vol
		* A1 divisor de tension para la bateria
		* res poner las resistencias 
		(poner el numero real de las resistencias en la cuenta para cambiar)

	Amp 
		* A0 Sensor amperaje
		* Ajustar valores del sensor por codigo
		(Medir con un voltimetro de presicion cuanto da la salida del sensor sin tener ningun cable)
		(Poner el sentor a trabajar teniendo un cable con 10A y luego ajustar la ajuste_sensor con la correspondiente para que el programa tire 10A)

	Bat
		* Declara el maximo y minimo de la bateria cuando esta en trabajo
		(hacer la prueba de bateria en su maxima carga estando en trabajo con el motor)
		(hacer la prueba de bateria en su minima carga estando en trabajo con el motor)