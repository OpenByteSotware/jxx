# HTTP server API and HTTP/1.1 engine overlay

This focused overlay adds the remaining Java 8 API declarations and a private cross-platform HTTP/1.1 parser/engine foundation. The parser supports request lines, headers, Content-Length bodies, and persistent-connection selection. Chunked request decoding, public facade wiring, concrete contexts/exchanges, filters/authentication implementations, and HTTPS are explicitly not yet complete in this overlay.
