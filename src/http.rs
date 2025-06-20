use std::collections::HashMap;

pub struct Request {
    pub method: String,
    pub path: String,
    pub version: String,
    pub headers: HashMap<String, String>,
    pub body: String,
}

pub struct Response {
    pub version: String,
    pub status_code: u16,
    pub status_text: String,
    pub headers: HashMap<String, String>,
    pub body: Vec<u8>,
}

impl Response {
    pub fn to_string(&self) -> String {
        let mut res = format!("{} {} {}\r\n", self.version, self.status_code, self.status_text);

        for (k, v) in &self.headers {
            res.push_str(&format!("{}: {}\r\n", k, v));
        }

        res.push_str("\r\n");

        res
    }
}

pub fn parse_request(r: String) -> Request {
    /* request line */
    let mut request_line: String = r.clone();
    let mut end: usize = request_line.find("\r\n").expect("oh no invalid request line!");
    request_line = request_line[0..end].to_string();

    /* request headers */
    let mut request_headers: String = r.clone();
    let mut start: usize = request_headers.find("\r\n").expect("oh no request headers is invalid!");
    end = request_headers.find("\r\n\r\n").expect("oh no rnrn leading to issues :(");
    request_headers = request_headers[start..end].to_string();

    /* request body */
    let mut request_body: String = r.clone();
    start = request_body.find("\r\n\r\n").expect("oh no it failed");
    end = request_body.find('\0').unwrap_or(request_body.len());
    request_body = request_body[start..end].to_string();

    /* parser */
    let mut request: Request = Request {
        method: "".to_string(),
        path: "".to_string(),
        version: "".to_string(),
        headers: HashMap::new(),
        body: "".to_string()
    };

    end = request_line.find(" ").expect("oh no it failed");
    request.method = request_line[0..end].to_string();
    request_line.drain(0..end+1);

    end = request_line.find(" ").expect("oh no it failed");
    request.path = request_line[0..end].to_string();
    request.path = url_decode(request.path);
    request_line.drain(0..end+1);

    end = request_line.find("\r\n").unwrap_or(request_line.len());
    request.version = request_line[0..end].to_string();
    request_line.drain(0..end);

    /* header magic */
    let mut headers: HashMap<String, String> = HashMap::new();
    
    while !request_headers.is_empty() {
        let s = request_headers.find(": ").expect("oh no it failed");
        let key = request_headers[0..s].to_string();
        request_headers.drain(0..s+2);
        end = request_headers.find("\r\n").unwrap_or(request_headers.len());
        let value = request_headers[0..end].to_string();
        request_headers.drain(0..end);

        headers.insert(key, value);
    }

    request.headers = headers;
    request.body = request_body;

    request
}

pub fn url_decode(input: String) -> String {
    let mut parsed = String::new();
    let bytes = input.as_bytes();
    let mut i = 0;

    while i < bytes.len() {
        if bytes[i] == b'%' && i + 2 < bytes.len() {
            let hex_str = &input[i+1..i+3];
            if let Ok(val) = u8::from_str_radix(hex_str, 16) {
                parsed.push(val as char);
                i += 3;
                continue;
            } else {
                parsed.push('%');
            }
        } else if bytes[i] == b'+' {
            parsed.push(' ');
            i += 1;
            continue;
        } else {
            parsed.push(bytes[i] as char);
        }
        i += 1;
    }

    parsed
}
