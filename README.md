# Raytrace

## Instalar dependências:

```sh
$ sudo apt-get update
$ sudo apt-get install build-essential
$ sudo apt-get install libgl1-mesa-dev
$ sudo apt-get install libglew1.5-dev freeglut3-dev libglm-dev
```

## Build:

```sh
$ make clean
$ make
```

## Executar:

```sh
$ ./main
```

### Desenvolvimento

```sh
$ nodemon --ext cpp --exec "make && ./main"
```
