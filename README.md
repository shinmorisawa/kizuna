# kizuna
## a web server
written in c++.
has a simple api to go with it. it comes with an app (that uses the api) to serve static files at /var/www/kizuna. make sure to give the user used to run kizuna perms for /var/www/kizuna.

features
- web server
- parser
- api

## building
all u need is openssl, just grab it from ur distro's repos
#### actually buliding
```
git clone https://github.com/shinmorisawa/kizuna
cd kizuna
make
./build/kizuna
```
optional, but recommended:  
`sudo make install`  
and now u can run kizuna with `kizuna`!
