# kizuna
## a web server
written in c++ (current branch is for porting to rust).
has a simple api to go with it. it comes with an app (that uses the api) to serve static files at /var/www/kizuna. make sure to give the user used to run kizuna perms for /var/www/kizuna.

features
- web server
- parser
- api
- tls/ssl (via openssl)

## building
#### deps
make sure u have cargo or u gon be ded :skull:
#### actually building
```
git clone https://github.com/shinmorisawa/kizuna
cd kizuna
cargo build --release
./target/release/kizuna
```
