use crate::http;
use crate::app::*;
use logc::*;
use tokio::net::{TcpStream, TcpListener};
use tokio::io::{AsyncReadExt, AsyncWriteExt};
use rustls::ServerConfig;
use rustls::pki_types::{CertificateDer, PrivatePkcs8KeyDer};
use tokio_rustls::{TlsAcceptor, server::TlsStream};
use std::io;
use std::sync::Arc;
use std::{fs::File, io::BufReader};

pub async fn start_tcp() -> io::Result<()> {
    let listener = TcpListener::bind("0.0.0.0:80").await?;

    loop {
        let (socket, _) = listener.accept().await?;
        process_request(socket).await;
    }
}

fn load_certs(path: &str) -> Vec<CertificateDer> {
    let cert_file = File::open(path).expect("cannot open cert.pem");
    let mut reader = BufReader::new(cert_file);
    rustls_pemfile::certs(&mut reader)
        .collect::<Result<_, _>>()
}

fn load_private_key(path: &str) -> PrivatePkcs8KeyDer<'static> {
    let key_file = File::open(path).expect("cannot open key.pem");
    let mut reader = BufReader::new(key_file);
    let keys = rustls_pemfile::pkcs8_private_keys(&mut reader).collect::<Result<_, _>>();
    keys[0].clone()
}

pub async fn start_tls() -> io::Result<()> {
    let certs = load_certs("cert.pem");
    let key = load_private_key("key.pem");

    let config = ServerConfig::builder()
        .with_safe_defaults()
        .with_no_client_auth()
        .with_single_cert(certs, key)
        .expect("invalid cert or key");

    let acceptor = TlsAcceptor::from(Arc::new(config));
    let listener = TcpListener::bind("0.0.0.0:443").await?;

    loop {
        let (stream, _) = listener.accept().await?;
        let acceptor = acceptor.clone();

        tokio::spawn(async move {
            match acceptor.accept(stream).await {
                Ok(tls_stream) => process_request_tls(tls_stream).await,
                Err(e) => error!("TLS handshake failed: {}", e),
            }
        });
    }
}

async fn process_request(mut socket: TcpStream) {
    let mut buf = vec![0; 4096];
    let mut request = Vec::new();
    let addr = socket.get_ref().0.peer_addr().unwrap();

    info!("got new request from {}", addr);

    loop {
        let n = socket.read(&mut buf).await.unwrap();
        if n == 0 {
            break;
        }

        request.extend_from_slice(&buf[..n]);

        if request.windows(4).any(|w| w == b"\r\n\r\n") {
            break;
        }
    }

    let request = String::from_utf8_lossy(&request).to_string();
    let response: http::Response = return_response(http::parse_request(request.clone())).await;
    let _ = socket.write_all(&response.to_string().as_bytes()).await;
    let _ = socket.write_all(&response.body).await;
}

async fn process_request_tls(mut socket: TlsStream<TcpStream>) {
    let mut buf = vec![0; 4096];
    let mut request = Vec::new();
    let addr = socket.peer_addr().unwrap();

    info!("got new request from {}", addr);

    loop {
        let n = socket.read(&mut buf).await.unwrap();
        if n == 0 {
            break;
        }

        request.extend_from_slice(&buf[..n]);

        if request.windows(4).any(|w| w == b"\r\n\r\n") {
            break;
        }
    }

    let request = String::from_utf8_lossy(&request).to_string();
    let response: http::Response = return_response(http::parse_request(request.clone())).await;
    let _ = socket.write_all(&response.to_string().as_bytes()).await;
    let _ = socket.write_all(&response.body).await;
}
