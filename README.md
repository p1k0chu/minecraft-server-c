# minecraft-server-c

this is a minimal Minecraft server impl in C.  
it only has a working MOTD and nothing else (yet)

It depends on:
- OpenSSL for libcrypto.so.  
- [cJSON](https://github.com/DaveGamble/cJSON)

<details>
  <summary>How to install dependencies</summary>

Example for Arch linux because i use arch btw (i believe you can find the packages yourself)

```sh
# openssl
pacman -S openssl

# cJSON
git clone https://github.com/DaveGamble/cJSON
mkdir cJSON/build
cd cJSON/build
cmake ..
sudo make install
```
</details>

## Building

Install dependencies.

And then:
```sh
git clone https://github.com/p1k0chu/minecraft-server-c
cd minecraft-server-c
mkdir build
cd build
cmake ..
make
```

now you can run the executable
```sh
./main
```

## License

Unless otherwise noted in individual file headers, all code in this repository is licensed under the MIT License (see `LICENSE` file).

