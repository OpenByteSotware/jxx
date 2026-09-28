# HTTP and HTTPS server implementation status

The JXX server runtime provides the Java 8 `com.sun.net.httpserver` public surface and a private cross-platform implementation for HTTP/1.0, HTTP/1.1, and HTTP over TLS.

Implemented runtime areas include context routing, filters, authentication, immutable request headers, fixed-length and chunked request framing, trailers and parser limits, fixed-length and chunked responses, HTTP/1.0 persistence differences, `Expect: 100-continue`, per-connection HTTPS configuration, SSL session exposure, a private default executor, active-connection tracking, and graceful terminal shutdown.

Known closest-practical deviation: request bodies are fully decoded and bounded before handler dispatch and are exposed through an in-memory input stream. They are not yet connection-backed streaming request streams.

Provider discovery currently uses the process-wide default provider. Property-driven and service-provider discovery are not implemented by this runtime.

Server parity certification requires the focused GoogleTest matrix to pass on both Windows and Linux. This status file supersedes the original early-overlay note that described chunked decoding, contexts, filters, authentication, and HTTPS as incomplete.
