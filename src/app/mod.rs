use crate::http;
use logc::*;
use tokio::fs;
use tokio::io;
use std::path::PathBuf;
use std::collections::HashMap;

struct File {
    base_path: PathBuf,
}


impl File {
    pub fn new<P: Into<PathBuf>>(base_path: P) -> Self {
        File {
            base_path: base_path.into(),
        }
    }

    pub async fn get_things_in_folder(&self, path: &str) -> io::Result<Vec<String>> {
        let mut path = path;
        if path.starts_with('/') {
            path = &path[1..];
        }
        let full_path = self.base_path.join(path).canonicalize()?;

        let mut things = Vec::new();
        let mut dir = fs::read_dir(full_path).await?;

        while let Some(entry) = dir.next_entry().await? {
            if let Some(name) = entry.file_name().to_str() {
                things.push(name.to_string());
            }
        }

        things.sort();
        Ok(things)
    }

    pub async fn is_directory(&self, path: &str) -> io::Result<bool> {
        let mut path = path;
        if path.starts_with('/') {
            path = &path[1..];
        }
        let full_path = self.base_path.join(path).canonicalize()?;
        Ok(full_path.is_dir())
    }

    pub async fn get_file(&self, path: &str) -> Result<Vec<u8>, std::io::Error> {
        let mut path = path;
        if path.starts_with('/') {
            path = &path[1..];
        }
        let full_path = self.base_path.join(path).canonicalize()?;

        fs::read(full_path).await
    }

    pub async fn size_of_file(&self, path: &str) -> std::io::Result<u64> {
        let clean_path = if path.starts_with('/') {
            &path[1..]
        } else {
            path
        };

        let full_path = self.base_path.join(clean_path).canonicalize()?;

        let metadata = std::fs::metadata(full_path)?;
        Ok(metadata.len())
    }
}

pub async fn return_response(r: http::Request) -> http::Response {
    let mut response = http::Response {
        version: "HTTP/1.1".to_string(),
        status_code: 200,
        status_text: "OK".to_string(),
        headers: HashMap::new(),
        body: vec![0],
    };

    let file = File::new("/var/www/kizuna");
    match file.get_file(&r.path).await {
        Ok(thing) => {
            info!("got file {}", r.path);
            response.body = thing;
        },
        Err(e) => error!("error {}", e),
    }

    response.headers.insert("Content-Length".to_string(), file.size_of_file(&r.path).await.unwrap().to_string());
    response.headers.insert("Server".to_string(), "kizuna-rs/pre-release".to_string());

    response
}
        
