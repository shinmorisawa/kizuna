#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>
#include "file.hpp"

namespace fs = std::filesystem;
fs::path base_path = "/var/www/kizuna";

std::string File::getFile(std::string path) {
	if (!path.empty() && path[0] == '/') path = path.substr(1);
	fs::path full_path = base_path / path;

	std::ifstream file(full_path);

	if (!file) {
		std::cout << "couldn't open file " << full_path << std::endl;
		return "failed";
	}

	std::stringstream buf;
	buf << file.rdbuf();
	file.close();
	return buf.str();
}

std::string File::getMIMEType(std::string path) {
	/* text/ */
	if (path.ends_with(".htm")) return "text/html";
	if (path.ends_with(".html")) return "text/html";
	if (path.ends_with(".css")) return "text/css";
	if (path.ends_with(".txt")) return "text/plain";
	if (path.ends_with(".csv")) return "text/csv";
	if (path.ends_with(".ics")) return "text/calendar";
	if (path.ends_with(".mjs")) return "text/javascript";
	/* application/ */
	if (path.ends_with(".js")) return "application/javascript";
	if (path.ends_with(".abw")) return "application/x-abiword";
	if (path.ends_with(".arc")) return "application/x-freearc";
	if (path.ends_with(".azw")) return "application/vnd.amazon.ebook";
	if (path.ends_with(".bin")) return "application/octet-stream";
	if (path.ends_with(".bz")) return "application/x-bzip";
	if (path.ends_with(".bz2")) return "application/x-bzip2";
	if (path.ends_with(".cda")) return "application/x-cdf";
	if (path.ends_with(".csh")) return "application/x-csh";
	if (path.ends_with(".doc")) return "application/msword";
	if (path.ends_with(".docx")) return "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	if (path.ends_with(".eot")) return "application/vnd.ms-fontobject";
	if (path.ends_with(".epub")) return "application/epub+zip";
	if (path.ends_with(".gz")) return "application/gzip";
	if (path.ends_with(".jar")) return "application/java-archive";
	if (path.ends_with(".json")) return "application/json";
	if (path.ends_with(".jsonld")) return "application/ld+json";
	if (path.ends_with(".mpkg")) return "application/vnd.apple.installer+xml";
	if (path.ends_with(".odp")) return "application/vnd.oasis.opendocument.presentation";	
	if (path.ends_with(".ods")) return "application/vnd.oasis.opendocument.spreadsheet";
	if (path.ends_with(".odt")) return "application/vnd.oasis.opendocument.text";
	if (path.ends_with(".ogx")) return "application/ogg";
	if (path.ends_with(".pdf")) return "application/pdf";
	if (path.ends_with(".php")) return "application/x-httpd-php";
	if (path.ends_with(".ppt")) return "application/vnd.ms-powerpoint";
	if (path.ends_with(".pptx")) return "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	if (path.ends_with(".rar")) return "application/vnd.rar";
	if (path.ends_with(".rtf")) return "application/rtf";
	if (path.ends_with(".sh")) return "application/x-sh";
	if (path.ends_with(".tar")) return "application/x-tar";
	if (path.ends_with(".vsd")) return "application/vnd.visio";
	if (path.ends_with(".xhtml")) return "application/xhtml+xml";
	if (path.ends_with(".xls")) return "application/vnd.ms-excel";
	if (path.ends_with(".xlsx")) return "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	if (path.ends_with(".xml")) return "application/xml";
	if (path.ends_with(".xul")) return "application/vnd.mozilla.xul+xml";
	if (path.ends_with(".zip")) return "application/zip";
	if (path.ends_with(".7z")) return "application/x-7z-compressed";
	/* image/ */
	if (path.ends_with(".png")) return "image/png";
	if (path.ends_with(".jpg") || path.ends_with(".jpeg")) return "image/jpeg";
	if (path.ends_with(".gif")) return "image/gif";
	if (path.ends_with(".svg")) return "image/svg+xml";
	if (path.ends_with(".apng")) return "image/apng";
	if (path.ends_with(".avif")) return "image/avif";
	if (path.ends_with(".bmp")) return "image/bmp";
	if (path.ends_with(".ico")) return "image/vnd.microsoft.icon";
	if (path.ends_with(".tif") || path.ends_with(".tiff")) return "image/tiff";
	if (path.ends_with(".webp")) return "image/webp";
	/* audio/ */
	if (path.ends_with(".flac")) return "audio/flac";
	if (path.ends_with(".ogg")) return "audio/ogg";
	if (path.ends_with(".aac")) return "audio/aac";
	if (path.ends_with(".mid") || path.ends_with(".midi")) return "audio/midi";
	if (path.ends_with(".mp3")) return "audio/mpeg";
	if (path.ends_with(".oga")) return "audio/ogg";
	if (path.ends_with(".opus")) return "audio/ogg";
	if (path.ends_with(".wav")) return "audio/wav";
	if (path.ends_with(".weba")) return "audio/webm";
	/* video/ */
	if (path.ends_with(".avi")) return "video/x-msvideo";
	if (path.ends_with(".mp4")) return "video/mp4";
	if (path.ends_with(".mpeg")) return "video/mpeg";
	if (path.ends_with(".webm")) return "video/webm";
	if (path.ends_with(".ogv")) return "video/ogg";
	if (path.ends_with(".ts")) return "video/mp2t";
	if (path.ends_with(".3gp")) return "video/3gpp";
	if (path.ends_with(".3g2")) return "video/3gpp2";
	/* font/ */
	if (path.ends_with(".otf")) return "font/otf";
	if (path.ends_with(".woff")) return "font/woff";
	if (path.ends_with(".woff2")) return "font/woff2";
	/* anything else */
	return "application/octet-stream";
}


