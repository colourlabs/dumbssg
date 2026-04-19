# dumbssg

a static site generator built in C++.

## build

1. install dependencies 

Fedora / DNF distros:
```sh
sudo dnf install inja-devel yaml-cpp-devel md4c-devel json-devel
```

FreeBSD:
```sh
# enable the FreeBSD ports repo, most/all of these libraries come from there
pkg install inja yaml-cpp md4c nlohmann-json
```

2. make build folder and build
```sh
mkdir build
cd build && cmake .. 
make
```

4. ????

3. profit
```sh
./dumbssg
```