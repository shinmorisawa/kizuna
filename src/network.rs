use crate::http;
use crate::app::*;
use logc::*;
use tokio::net::{TcpStream, TcpListener};
use tokio::io::{AsyncReadExt, AsyncWriteExt};
use std::io;

pub async fn start_tcp() -> io::Result<()> {
    let listener = TcpListener::bind("0.0.0.0:2929").await?;

    loop {
        let (socket, _) = listener.accept().await?;
        process_request(socket).await;
    }
}

async fn process_request(mut socket: TcpStream) {
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

    let mut request = String::from_utf8_lossy(&request).to_string();
    let response: http::Response = return_response(http::parse_request(request.clone())).await;
    socket.write_all(&response.to_string().as_bytes()).await;
}
