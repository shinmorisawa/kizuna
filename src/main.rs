mod http;
mod app;
mod network;

use logc::*;

#[tokio::main]
async fn main() {
    info!("welcome to kizuna.rs");
    network::start_tcp().await;
}
