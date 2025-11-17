# Simulador de Manejo de Memoria

Tarea #3 para el curso *Principios de Sistemas Operativos* —
Simulación de administración dinámica de memoria utilizando un heap personalizado, con soporte para **malloc**, **calloc**, **realloc**, **free**, y tres algoritmos de asignación: **First-Fit**, **Best-Fit**, y **Worst-Fit**.

## Autores

* Steven Sequeira Araya

* Jefferson Salas Cordero

## Compilación y Ejecución

**Nota:** se debe usar una terminal compatible con Linux

### Compilar el proyecto

Desde la raíz del repositorio:

```bash
make
```

Esto genera el ejecutable `memsim`.

### Ejecutar archivos de prueba

#### Pruebas básicas
```bash
./memsim tests/basic_test.txt
```

#### Pruebas de REALLOC
```bash
./memsim tests/edge_cases.txt
```

#### Pruebas de menejo de errores
```bash
./memsim tests/realloc_test.txt
```

El programa:

* Inicializa la arena de memoria
* Lee y ejecuta cada comando del archivo (`ALLOC`, `FREE`, `REALLOC`, `PRINT`)
* Muestra el estado del heap
* Detecta fugas de memoria al finalizar mediante `var_print_leaks()`

### Cambiar algoritmo de asignación

En `src/main.c`, después de iniciar las variables `vars_init()`:

```c
allocator_set_algorithm(ALLOC_FIRST_FIT);
allocator_set_algorithm(ALLOC_BEST_FIT);
allocator_set_algorithm(ALLOC_WORST_FIT);
```
Descomenta el algoritmo que deseas usar

**Nota**: En caso de no descomentar alguno, se asume por defecto el algoritmo de First Fit

## Arquitectura del Proyecto

La arquitectura se diseñó siguiendo principios **SOLID**, alta modularidad, separación de responsabilidades y claridad estructural.

## Árbol de directorios

``` bash
memsim/
│
├── src/
│   ├── main.c
│   │
│   ├── core/
│   │   ├── memory.c
│   │   ├── allocator.c
│   │   ├── blocks.c
│   │   ├── variables.c
│   │   ├── memory_ops.c
│   │   ├── print.c
│   │   └── parser.c
│   │
│   └── utils/
│       ├── list.c
│       ├── string_utils.c
│       └── log.c
│
├── include/
│   ├── memory.h
│   ├── allocator.h
│   ├── blocks.h
│   ├── variables.h
│   ├── parser.h
│   ├── list.h
│   ├── string_utils.h
│   ├── memory_ops.h
│   ├── print.h
│   └── log.h
│
├── tests/
│   ├── basic_input.txt
│   ├── fragmentation_cases.txt
│   └── realloc_edge_cases.txt
│
├── Makefile
└── README.md
```

## **src/**

Código fuente principal del simulador.


### **src/main.c**

Punto de entrada del programa.
Inicializa memoria, variables, selecciona algoritmo y ejecuta un archivo de comandos mediante el parser.

---

## **src/core/**

Componentes centrales del simulador: arena, bloques, asignador, operaciones de alto nivel y parser.

---

### **memory.c**

Implementa:

* Inicialización y destrucción de la arena simulada (`memory_init`, `memory_destroy`)
* Manejo del bloque inicial
* Acceso al puntero de arena

---

### **allocator.c**

Implementa los algoritmos de asignación:

* **First-Fit**
* **Best-Fit**
* **Worst-Fit**

Se encarga de seleccionar el bloque libre más adecuado para ALLOC/REALLOC.

---

### **blocks.c**

Administra la **lista doblemente enlazada de bloques**.
Funciones principales:

* `block_create()`
* `block_split()`
* `block_merge()`
* `blocks_first()`
* `blocks_destroy()`

Responsable de la estructura del heap y fragmentación.

---

### **variables.c**

Implementa la tabla hash simple (lista enlazada) que asocia:

```
nombre de variable → bloque asignado
```

Soporta:

* Registrar variable (`var_set`)
* Eliminar variable (`var_remove`)
* Obtener bloque (`var_get`)
* Detectar fugas (`var_print_leaks`)

---

### **memory_ops.c**

Capa de operaciones de alto nivel:

* `mem_alloc`
* `mem_free`
* `mem_realloc`

Coordina allocator, bloques, arena y tabla de variables.

---

### **print.c**

Genera una visualización del heap:

* Lista completa de bloques
* Resumen: memoria total / libre / usada / bloques libres

---

### **parser.c**

Lee archivos de comandos y ejecuta:

```
ALLOC <nom> <size>
FREE <nom>
REALLOC <nom> <size>
PRINT
```

Gestiona errores de sintaxis y líneas inválidas.

---

## **src/utils/**

Funciones utilitarias independientes.

---

### **list.c**

Implementación simple de listas enlazadas.
Usada para tablas intermedias si se requieren.

---

### **string_utils.c**

Funciones auxiliares para:

* Trim de espacios
* Detección de prefijos
* Normalización de líneas

---

### **log.c**

Sistema básico de logging vía:

* `log_info`
* `log_error`

---

## **include/**

Headers del proyecto.

Cada archivo `.h` declara la interfaz pública del módulo correspondiente:

* **memory.h** — API para la arena simulada
* **allocator.h** — enum y funciones de asignación
* **blocks.h** — estructuras y operaciones sobre bloques
* **variables.h** — tabla nombre → bloque
* **parser.h** — ejecución de archivos
* **list.h** — utilidades de lista
* **string_utils.h** — utilidades de string
* **memory_ops.h** — ALLOC, FREE, REALLOC
* **print.h** — visualización del heap
* **log.h** — logging

---

## **tests/**

Colección de archivos de prueba diseñados para demostrar cada aspecto del simulador.

---

### **basic_input.txt**

Prueba simple de ALLOC/FREE/PRINT.

### **edge_cases.txt**

Crea fragmentación real y demuestra el efecto de liberar bloques intermedios.

### **realloc_test.txt**

Prueba los 5 casos de REALLOC:

* Igual tamaño
* Reducir
* Expandir in-place
* Mover a nuevo bloque
* Tamaño cero

**Nota** Por cada uno se puede ajustar cual algoritmo usar. Para más detalles ver la la sección de compilación y ejecución.

---

## **Makefile**

Sistema de compilación.
Genera `memsim`, compila cada módulo y gestiona reglas:

* `make`
* `make clean`

---

## **README.md**

Este archivo: documentación completa del proyecto.

---





