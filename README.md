# Trabajo Práctico — Programación con hilos

Información de equipo

Nombres completos:

- Nombre: Ricardo Medina Herrera
  - Correo: ricardo.medinah@udea.edu.co
  - Número de documento: _______________________

- Nombre: Santiago Villegas Naranjo
  - Correo: santiago.villegasn@udea.edu.co
  - Número de documento: _______________________

Enlace al video de exposición (10 minutos): _______________________

Descripción general

Este repositorio contiene implementaciones en C de dos problemas clásicos —cómputo de π por integración numérica y generación de la secuencia de Fibonacci— con versiones secuenciales y con hilos (pthread).

Archivos relevantes

- fibonacci.c — Generación de Fibonacci usando pthread (almacenado en long long).
- fibonacci_s.c — Generación de Fibonacci usando la librería GMP (enteros arbitrarios) en versión secuencial.
- pi.c — Cálculo de π secuencial por integración numérica (método de punto medio).
- pi_p.c — Cálculo de π paralelo usando pthreads y particionado de intervalos.

Documentación de funciones y estructuras

fibonacci.c

FibonacciData (struct)
- Descripción: Contenedor que pasa el buffer y el tamaño al hilo.
- Campos:
  - long long *fib: puntero al arreglo donde se almacenan los términos.
  - int N: número de términos a generar.

GetTime
- Firma: double GetTime()
- Descripción: Devuelve el tiempo actual en segundos usando CLOCK_MONOTONIC.
- Parámetros: ninguno.
- Retorno: tiempo en segundos como double.

GenerateFibonacci
- Firma: void *GenerateFibonacci(void *arg)
- Descripción: Rutina que corre en un hilo y llena el arreglo con los N primeros términos de Fibonacci (tipo long long).
- Parámetros:
  - void *arg: puntero a FibonacciData.
- Retorno: NULL (necesario por la interfaz pthread).
- Escribe en el arreglo data->fib.

main (fibonacci.c)
- Firma: int main(int argc, char *argv[])
- Descripción: Programa cliente que valida argumentos, reserva memoria, crea un hilo para generar Fibonacci, espera su terminación, imprime la secuencia y el tiempo de ejecución.
- Parámetros:
  - argc, argv: argv[1] debe ser N (número de términos).
- Retorno: 0 en éxito, 1 en error (uso inválido, memoria o creación de hilo falla).

fibonacci_s.c

GetTime
- Firma: double GetTime()
- Descripción: Igual que en fibonacci.c —mide tiempo con CLOCK_MONOTONIC.

fibo
- Firma: void fibo(mpz_t *nums, long long n_fibbo)
- Descripción: Calcula los primeros n_fibbo términos de Fibonacci usando mpz_t (GMP), y los almacena en el arreglo nums.
- Parámetros:
  - mpz_t *nums: arreglo pre-inicializado de enteros GMP donde se escriben los términos.
  - long long n_fibbo: número de términos a generar.
- Retorno: void.
- Escribe en los mpz_t de nums.

main (fibonacci_s.c)
- Firma: int main(int argc, char *argv[])
- Descripción: Programa que reserva un arreglo mpz_t, inicializa cada elemento, llama a fibo, imprime el tiempo de ejecución y los valores, limpia la memoria de GMP y libera el arreglo.
- Parámetros:
  - argc, argv: argv[1] opcional para tamaño n.
- Requisitos: enlazar con -lgmp al compilar.

pi.c

GetTime
- Firma: double GetTime()
- Descripción: Mide tiempo con CLOCK_MONOTONIC.

f
- Firma: double f(double x)
- Descripción: Función integrando 4/(1+x^2) para el cálculo de π.
- Parámetros: x: punto donde se evalúa.
- Retorno: valor de la función f en x.

CalcPi
- Firma: double CalcPi(long long n)
- Descripción: Aproxima π usando suma de puntos medios con n intervalos.
- Parámetros: n: número de sub-intervalos.
- Retorno: aproximación de π (double).

main (pi.c)
- Firma: int main(int argc, char *argv[])
- Descripción: Lee n desde argv (opcional), mide el tiempo de CalcPi y muestra la aproximación y el tiempo.

pi_p.c

ThreadData (struct)
- Descripción: Datos por hilo para calcular una porción de la suma de integrales.
- Campos:
  - long long start: índice inicial (inclusive).
  - long long end: índice final (exclusive).
  - double fH: ancho del intervalo 1.0/n.
  - double partial_sum: acumulador local del hilo.

GetTime
- Firma: double GetTime()
- Descripción: Igual que en los otros archivos.

f
- Firma: double f(double x)
- Descripción: Integrando 4/(1+x^2).

ThreadCalcPi
- Firma: void *ThreadCalcPi(void *arg)
- Descripción: Rutina de hilo que acumula la suma de f(x) para i en [start, end) y guarda el resultado en partial_sum del ThreadData.
- Parámetros: void *arg apuntando a ThreadData.
- Retorno: NULL.

main (pi_p.c)
- Firma: int main(int argc, char *argv[])
- Descripción: Programa que recibe n (intervalos) y n_threads (opcional), divide el trabajo entre hilos, crea hilos con ThreadCalcPi, junta las sumas parciales y calcula π final. Imprime pi aproximado, número de hilos y tiempo de ejecución.
- Parámetros: argv[1] = n (opcional), argv[2] = n_threads (opcional).
- Notas: Se reserva memoria para threads y thread_data; ThreadData usa attribute aligned(64) para reducir false sharing.

Problemas encontrados y soluciones

- Overflow en Fibonacci con long long
  - Problema: los términos de Fibonacci crecen exponencialmente; para N relativamente grandes los valores sobrepasan el rango de long long y producían resultados incorrectos.
  - Solución: usar GMP (fibonacci_s.c) y el tipo mpz_t para enteros arbitrariamente grandes. Compilar con -lgmp.

- Manejo de memoria y validaciones
  - Problema: posibles fallos por falta de validación al reservar memoria o parámetros inválidos.
  - Solución: añadir comprobaciones de retorno de malloc, validar N>0 y n_threads>0 y retornar códigos de error claros.

- Contención y false sharing en suma paralela de π
  - Problema: cuando múltiples hilos escriben en campos próximos de una estructura, el rendimiento puede degradarse por false sharing.
  - Solución: ThreadData se declara con __attribute__((aligned(64))) para alinear las estructuras y reducir el false sharing. Además cada hilo acumula en partial_sum local y se combina al final.

- Medición de tiempo consistente
  - Problema: uso de time() o clock() introduce variación medible en benchmarking.
  - Solución: se usa clock_gettime(CLOCK_MONOTONIC) encapsulado en GetTime() para obtener medidas más estables.

Pruebas sugeridas

- Prueba: Compilar y ejecutar Fibonacci con hilos (N pequeño)
  - Comando: gcc -o fibonacci fibonacci.c -lpthread && ./fibonacci 20
  - Resultado:

- Prueba: Fibonacci con GMP (N moderado)
  - Comando: gcc -o fibonacci_s fibonacci_s.c -lgmp && ./fibonacci_s 500
  - Resultado:

- Prueba: Comparar tiempos secuencial vs paralelo para π
  - Comando: gcc -o pi pi.c && gcc -o pi_p pi_p.c -lpthread && ./pi 10000000 && ./pi_p 10000000 4
  - Resultado:

- Prueba: Variar número de hilos para pi_p y observar escalabilidad
  - Comando: ./pi_p 100000000 1
  - Comando: ./pi_p 100000000 2
  - Comando: ./pi_p 100000000 4
  - Resultado:

Instrucciones de compilación rápidas

- Compilar Fibonacci con hilos:
  - gcc -o fibonacci fibonacci.c -lpthread
- Compilar Fibonacci con GMP:
  - gcc -o fibonacci_s fibonacci_s.c -lgmp
- Compilar π secuencial:
  - gcc -o pi pi.c
- Compilar π paralelo:
  - gcc -o pi_p pi_p.c -lpthread

Manifiesto de transparencia — uso de IA generativa

Se declara explícitamente que se utilizó IA generativa en los siguientes puntos del desarrollo:

- Consultas sobre el uso correcto de hilos: Pedimos ejemplos y pautas para utilizar pthreads correctamente (crear hilos, pasar datos mediante structs, usar pthread_join, evitar condiciones de carrera y false sharing). La IA nos sirvió como referencia para buenas prácticas y patrones de uso.
- Solución al problema del overflow en Fibonacci: La IA nos ayudó a identificar que el tipo long long no era suficiente para N grandes y nos sugirió la librería GMP (libgmp) como solución; implementamos fibonacci_s.c usando mpz_t y enlazamos con -lgmp.
- Revisión de redacción y coherencia: Se usó IA para revisar y mejorar la redacción del notebook de análisis y del README final, asegurando coherencia y claridad en las explicaciones.

Se deja constancia de estas ayudas; el código fuente final y las decisiones de diseño (por ejemplo usar GMP, checks de memoria, alineación para evitar false sharing) fueron implementadas por los integrantes del equipo. No se incorporaron fragmentos de código directamente copiados de la IA sin entenderlos ni adaptarlos al contexto del proyecto.

Conclusiones

- El uso de hilos con pthread permite paralelizar trabajos embarazosos como la integración numérica para aproximar π; con un particionado apropiado se obtiene reducción de tiempo de ejecución en máquinas con múltiples núcleos.
- Para problemas numéricos con crecimientos grandes (Fibonacci) es imprescindible considerar tipos de datos adecuados; GMP proporciona una solución robusta para enteros arbitrarios.
- Medir correctamente el tiempo (CLOCK_MONOTONIC) y aislar efectos de false sharing mejora la calidad de los benchmarks.
- La paralelización añade complejidad (sincronización, particionado, gestión de memoria), pero aplicando buenas prácticas (validaciones, estructuras por hilo, evitar escrituras compartidas frecuentes) los beneficios superan los costes en problemas paralelizables.
- Las pruebas automatizadas y sencillas (scripts que guardan salidas y tiempos) facilitan la comparación entre versiones y la replicabilidad de resultados.

Nota

- Recordatorio de compilación de GMP: instalar libgmp-dev (o equivalente) en el sistema antes de compilar fibonacci_s.c.
