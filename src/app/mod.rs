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

fn get_mime_type(path: &str) -> &str {
    if path.ends_with(".htm") || path.ends_with(".html") {
        "text/html"
    } else if path.ends_with(".css") {
        "text/css"
    } else if path.ends_with(".txt") {
        "text/plain"
    } else if path.ends_with(".csv") {
        "text/csv"
    } else if path.ends_with(".ics") {
        "text/calendar"
    } else if path.ends_with(".mjs") {
        "text/javascript"
    }
    // application/
    else if path.ends_with(".js") {
        "application/javascript"
    } else if path.ends_with(".abw") {
        "application/x-abiword"
    } else if path.ends_with(".arc") {
        "application/x-freearc"
    } else if path.ends_with(".azw") {
        "application/vnd.amazon.ebook"
    } else if path.ends_with(".bin") {
        "application/octet-stream"
    } else if path.ends_with(".bz") {
        "application/x-bzip"
    } else if path.ends_with(".bz2") {
        "application/x-bzip2"
    } else if path.ends_with(".cda") {
        "application/x-cdf"
    } else if path.ends_with(".csh") {
        "application/x-csh"
    } else if path.ends_with(".doc") {
        "application/msword"
    } else if path.ends_with(".docx") {
        "application/vnd.openxmlformats-officedocument.wordprocessingml.document"
    } else if path.ends_with(".eot") {
        "application/vnd.ms-fontobject"
    } else if path.ends_with(".epub") {
        "application/epub+zip"
    } else if path.ends_with(".gz") {
        "application/gzip"
    } else if path.ends_with(".jar") {
        "application/java-archive"
    } else if path.ends_with(".json") {
        "application/json"
    } else if path.ends_with(".jsonld") {
        "application/ld+json"
    } else if path.ends_with(".mpkg") {
        "application/vnd.apple.installer+xml"
    } else if path.ends_with(".odp") {
        "application/vnd.oasis.opendocument.presentation"
    } else if path.ends_with(".ods") {
        "application/vnd.oasis.opendocument.spreadsheet"
    } else if path.ends_with(".odt") {
        "application/vnd.oasis.opendocument.text"
    } else if path.ends_with(".ogx") {
        "application/ogg"
    } else if path.ends_with(".pdf") {
        "application/pdf"
    } else if path.ends_with(".php") {
        "application/x-httpd-php"
    } else if path.ends_with(".ppt") {
        "application/vnd.ms-powerpoint"
    } else if path.ends_with(".pptx") {
        "application/vnd.openxmlformats-officedocument.presentationml.presentation"
    } else if path.ends_with(".rar") {
        "application/vnd.rar"
    } else if path.ends_with(".rtf") {
        "application/rtf"
    } else if path.ends_with(".sh") {
        "application/x-sh"
    } else if path.ends_with(".tar") {
        "application/x-tar"
    } else if path.ends_with(".vsd") {
        "application/vnd.visio"
    } else if path.ends_with(".xhtml") {
        "application/xhtml+xml"
    } else if path.ends_with(".xls") {
        "application/vnd.ms-excel"
    } else if path.ends_with(".xlsx") {
        "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"
    } else if path.ends_with(".xml") {
        "application/xml"
    } else if path.ends_with(".xul") {
        "application/vnd.mozilla.xul+xml"
    } else if path.ends_with(".zip") {
        "application/zip"
    } else if path.ends_with(".7z") {
        "application/x-7z-compressed"
    }
    // image/
    else if path.ends_with(".png") {
        "image/png"
    } else if path.ends_with(".jpg") || path.ends_with(".jpeg") {
        "image/jpeg"
    } else if path.ends_with(".gif") {
        "image/gif"
    } else if path.ends_with(".svg") {
        "image/svg+xml"
    } else if path.ends_with(".apng") {
        "image/apng"
    } else if path.ends_with(".avif") {
        "image/avif"
    } else if path.ends_with(".bmp") {
        "image/bmp"
    } else if path.ends_with(".ico") {
        "image/vnd.microsoft.icon"
    } else if path.ends_with(".tif") || path.ends_with(".tiff") {
        "image/tiff"
    } else if path.ends_with(".webp") {
        "image/webp"
    }
    // audio/
    else if path.ends_with(".flac") {
        "audio/flac"
    } else if path.ends_with(".ogg") {
        "audio/ogg"
    } else if path.ends_with(".aac") {
        "audio/aac"
    } else if path.ends_with(".mid") || path.ends_with(".midi") {
        "audio/midi"
    } else if path.ends_with(".mp3") {
        "audio/mpeg"
    } else if path.ends_with(".oga") {
        "audio/ogg"
    } else if path.ends_with(".opus") {
        "audio/ogg"
    } else if path.ends_with(".wav") {
        "audio/wav"
    } else if path.ends_with(".weba") {
        "audio/webm"
    }
    // video/
    else if path.ends_with(".avi") {
        "video/x-msvideo"
    } else if path.ends_with(".mp4") {
        "video/mp4"
    } else if path.ends_with(".mpeg") {
        "video/mpeg"
    } else if path.ends_with(".webm") {
        "video/webm"
    } else if path.ends_with(".ogv") {
        "video/ogg"
    } else if path.ends_with(".ts") {
        "video/mp2t"
    } else if path.ends_with(".3gp") {
        "video/3gpp"
    } else if path.ends_with(".3g2") {
        "video/3gpp2"
    }
    // font/
    else if path.ends_with(".otf") {
        "font/otf"
    } else if path.ends_with(".woff") {
        "font/woff"
    } else if path.ends_with(".woff2") {
        "font/woff2"
    } else {
        "application/octet-stream"
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
    response.headers.insert("Content-Type".to_string(), get_mime_type(&r.path).to_string());
    response.headers.insert("Server".to_string(), "kizuna-rs/pre-release".to_string());

    response
}
        
