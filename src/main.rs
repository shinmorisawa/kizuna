mod http;
mod app;
mod network;

use logc::*;

#[tokio::main]
async fn main() {
    info!("welcome to kizuna.rs");
    let (tcp, tls) = tokio::join!(
        network::start_tcp(),
        network::start_tls(),
    );

    if let Err(e) = tcp {
        error!("http error: {}", e);
    }

    if let Err(e) = tls {
        error!("tls error: {}", e);
    }
}
